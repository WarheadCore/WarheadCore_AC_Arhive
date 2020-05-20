/*
 * This file is part of the WarheadCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/// \addtogroup Trinityd Trinity Daemon
/// @{
/// \file

#include "Common.h"
#include "DatabaseEnv.h"
#include "Config.h"
#include "Log.h"
#include "Logo.h"
#include "GitRevision.h"
#include "SignalHandler.h"
#include "WorldSocket.h"
#include "WorldSocketMgr.h"
#include "CliRunnable.h"
#include "RARunnable.h"
#include "ACSoap.h"
#include "RealmList.h"
#include "ScriptMgr.h"
#include "BigNumber.h"
#include "OpenSSLCrypto.h"
#include "World.h"
#include "BattlegroundMgr.h"
#include "MapManager.h"
#include "OutdoorPvPMgr.h"
#include "AvgDiffTracker.h"
#include "AsyncAuctionListing.h"
#include "DatabaseLoader.h"
#include "ScriptLoader.h"
#include "GameConfig.h"
#include <ace/Sig_Handler.h>

#ifdef ELUNA
#include "LuaEngine.h"
#endif

#if WH_PLATFORM == WH_PLATFORM_WINDOWS
#include "ServiceWin32.h"
char serviceName[] = "worldserver";
char serviceLongName[] = "WarheadCore world service";
char serviceDescription[] = "WarheadCore World of Warcraft emulator world service";
/*
 * -1 - not in service mode
 *  0 - stopped
 *  1 - running
 *  2 - paused
 */
int m_ServiceStatus = -1;
#endif

#if WH_PLATFORM == WH_PLATFORM_UNIX
#include <sched.h>
#include <sys/resource.h>
#define PROCESS_HIGH_PRIORITY -15 // [-20, 19], default is 0
#endif

#ifndef _ACORE_CORE_CONFIG
#define _ACORE_CORE_CONFIG  "worldserver.conf"
#endif

#define WORLD_SLEEP_CONST 10

/// Print out the usage string for this program on the console.
void usage(const char* prog)
{
    SYS_LOG_INFO("Usage:\n");
    SYS_LOG_INFO(" %s [<options>]\n", prog);
    SYS_LOG_INFO("    -c config_file           use config_file as configuration file\n");
#if WH_PLATFORM == WH_PLATFORM_WINDOWS
    SYS_LOG_INFO("    Running as service functions:\n");
    SYS_LOG_INFO("    --service                run as service\n");
    SYS_LOG_INFO("    -s install               install service\n");
    SYS_LOG_INFO("    -s uninstall             uninstall service\n");
#endif
}

/// Handle worldservers's termination signals
class WorldServerSignalHandler : public warhead::SignalHandler
{
public:
    virtual void HandleSignal(int sigNum)
    {
        switch (sigNum)
        {
        case SIGINT:
            World::StopNow(RESTART_EXIT_CODE);
            break;
        case SIGTERM:
#if WH_PLATFORM == WH_PLATFORM_WINDOWS
        case SIGBREAK:
            if (m_ServiceStatus != 1)
#endif
                World::StopNow(SHUTDOWN_EXIT_CODE);
            break;
            /*case SIGSEGV:
                sLog->outString("ZOMG! SIGSEGV handled!");
                World::StopNow(SHUTDOWN_EXIT_CODE);
                break;*/
        }
    }
};

class FreezeDetectorRunnable : public warhead::Runnable
{
private:
    uint32 _loops;
    uint32 _lastChange;
    uint32 _delayTime;

public:
    FreezeDetectorRunnable(uint32 freezeDelay) : _loops(0), _lastChange(0), _delayTime(freezeDelay) {}

    void run()
    {
        if (!_delayTime)
            return;

        sLog->outString("Starting up anti-freeze thread (%u seconds max stuck time)...", _delayTime / 1000);

        while (!World::IsStopped())
        {
            uint32 curtime = getMSTime();
            if (_loops != World::m_worldLoopCounter)
            {
                _lastChange = curtime;
                _loops = World::m_worldLoopCounter;
            }
            else if (getMSTimeDiff(_lastChange, curtime) > _delayTime)
            {
                sLog->outString("World Thread hangs, kicking out server!");
                ABORT();
            }

            warhead::Thread::Sleep(1000);
        }

        sLog->outString("Anti-freeze thread exiting without problems.");
    }
};

bool _StartDB();
void _StopDB();
void ClearOnlineAccounts();

/// Heartbeat thread for the World
class WorldRunnable : public warhead::Runnable
{
public:
    /// Heartbeat for the World
    void run()
    {
        uint32 realCurrTime = 0;
        uint32 realPrevTime = getMSTime();

        ///- While we have not World::m_stopEvent, update the world
        while (!World::IsStopped())
        {
            ++World::m_worldLoopCounter;
            realCurrTime = getMSTime();

            uint32 diff = getMSTimeDiff(realPrevTime, realCurrTime);

            sWorld->Update(diff);
            realPrevTime = realCurrTime;

            uint32 executionTimeDiff = getMSTimeDiff(realCurrTime, getMSTime());
            devDiffTracker.Update(executionTimeDiff);
            avgDiffTracker.Update(executionTimeDiff > WORLD_SLEEP_CONST ? executionTimeDiff : WORLD_SLEEP_CONST);

            if (executionTimeDiff < WORLD_SLEEP_CONST)
                warhead::Thread::Sleep(WORLD_SLEEP_CONST - executionTimeDiff);

#if WH_PLATFORM == WH_PLATFORM_WINDOWS
            if (m_ServiceStatus == 0)
                World::StopNow(SHUTDOWN_EXIT_CODE);

            while (m_ServiceStatus == 2)
                Sleep(1000);
#endif
        }

        sLog->SetRealmID(0, false);

        sScriptMgr->OnShutdown();

        sWorld->KickAll();                                       // save and kick all players
        sWorld->UpdateSessions(1);                             // real players unload required UpdateSessions call

        // unload battleground templates before different singletons destroyed
        sBattlegroundMgr->DeleteAllBattlegrounds();
        sWorldSocketMgr->StopNetwork();
        sMapMgr->UnloadAll();                     // unload all grids (including locked in memory)
        sObjectAccessor->UnloadAll();             // unload 'i_player2corpse' storage and remove from world
        sScriptMgr->Unload();
        sOutdoorPvPMgr->Die();
#ifdef ELUNA
        Eluna::Uninitialize();
#endif
    }
};

class AuctionListRunnable : public warhead::Runnable
{
public:
    void run()
    {
        LOG_INFO("auctionHouse", "Starting up Auction House Listing thread...");

        while (!World::IsStopped())
        {
            if (AsyncAuctionListingMgr::IsAuctionListingAllowed())
            {
                uint32 diff = AsyncAuctionListingMgr::GetDiff();
                AsyncAuctionListingMgr::ResetDiff();

                if (AsyncAuctionListingMgr::GetTempList().size() || AsyncAuctionListingMgr::GetList().size())
                {
                    ACORE_GUARD(ACE_Thread_Mutex, AsyncAuctionListingMgr::GetLock());

                    {
                        ACORE_GUARD(ACE_Thread_Mutex, AsyncAuctionListingMgr::GetTempLock());

                        for (auto itr : AsyncAuctionListingMgr::GetTempList())
                            AsyncAuctionListingMgr::GetList().push_back(itr);

                        AsyncAuctionListingMgr::GetTempList().clear();
                    }

                    for (auto itr : AsyncAuctionListingMgr::GetList())//std::list<AuctionListItemsDelayEvent>::iterator itr = AsyncAuctionListingMgr::GetList().begin(); itr != AsyncAuctionListingMgr::GetList().end(); ++itr)
                    {
                        if (itr._msTimer <= diff)
                            itr._msTimer = 0;
                        else
                            itr._msTimer -= diff;
                    }

                    for (std::list<AuctionListItemsDelayEvent>::iterator itr = AsyncAuctionListingMgr::GetList().begin(); itr != AsyncAuctionListingMgr::GetList().end(); ++itr)
                    if ((*itr)._msTimer == 0)
                    {
                        if ((*itr).Execute())
                            AsyncAuctionListingMgr::GetList().erase(itr);

                        break;
                    }
                }
            }

            warhead::Thread::Sleep(1);
        }

        LOG_INFO("auctionHouse", "Auction House Listing thread exiting without problems.");
    }
};

/// Launch the server
extern int main(int argc, char** argv)
{
    ///- Command line parsing to get the configuration file name
    std::string configFile = sConfigMgr->GetConfigPath() + std::string(_ACORE_CORE_CONFIG);
    int c = 1;
    bool isImportDBOnly = false;

    while (c < argc)
    {
        if (strcmp(argv[c], "--dry-run") == 0)
        {
            sConfigMgr->setDryRun(true);
        }

        if (!strcmp(argv[c], "--import-db"))
            isImportDBOnly = true;

        if (!strcmp(argv[c], "-c"))
        {
            if (++c >= argc)
            {
                SYS_LOG_INFO("Runtime-Error: -c option requires an input argument");
                usage(argv[0]);
                return 1;
            }
            else
                configFile = argv[c];
        }

        #if WH_PLATFORM == WH_PLATFORM_WINDOWS
        if (strcmp(argv[c], "-s") == 0) // Services
        {
            if (++c >= argc)
            {
                SYS_LOG_INFO("Runtime-Error: -s option requires an input argument");
                usage(argv[0]);
                return 1;
            }

            if (strcmp(argv[c], "install") == 0)
            {
                if (WinServiceInstall())
                    SYS_LOG_INFO("Installing service\n");
                return 1;
            }
            else if (strcmp(argv[c], "uninstall") == 0)
            {
                if (WinServiceUninstall())
                    SYS_LOG_INFO("Uninstalling service\n");
                return 1;
            }
            else
            {
                SYS_LOG_INFO("Runtime-Error: unsupported option %s", argv[c]);
                usage(argv[0]);
                return 1;
            }
        }

        if (strcmp(argv[c], "--service") == 0)
            WinServiceRun();
        #endif
        ++c;
    }

    sConfigMgr->SetConfigList(configFile, std::string(CONFIG_FILE_LIST));

    if (!sConfigMgr->LoadAppConfigs())
        return 1;

    // Init all logs
    sLog->Initialize();

    warhead::Logo::Show("worldserver", configFile.c_str(),
        [](char const* text)
        {
            LOG_INFO("server.worldserver", "%s", text);
        }
    );

    sConfigMgr->LoadModulesConfigs();

    ///- and run the 'Master'
    /// @todo Why do we need this 'Master'? Can't all of this be in the Main as for Realmd?
    OpenSSLCrypto::threadsSetup();
    BigNumber seed1;
    seed1.SetRand(16 * 8);

    /// worldserver PID file creation
    std::string pidFile = sConfigMgr->GetStringDefault("PidFile", "");
    if (!pidFile.empty())
    {
        if (uint32 pid = CreatePIDFile(pidFile))
            sLog->outString("Daemon PID: %u\n", pid);
        else
        {
            sLog->outString("Cannot create PID file %s.\n", pidFile.c_str());
            return 1;
        }
    }

    ///- Start the databases
    if (!_StartDB())
        return 1;

    if (isImportDBOnly)
        exit(0);

    // set server offline (not connectable)
    LoginDatabase.DirectPExecute("UPDATE realmlist SET flag = (flag & ~%u) | %u WHERE id = '%d'", REALM_FLAG_OFFLINE, REALM_FLAG_INVALID, realmID);

    ///- Initialize the World
    sScriptMgr->SetScriptLoader(AddScripts);
    sWorld->SetInitialWorldSettings();

    sScriptMgr->OnStartup();

    ///- Initialize the signal handlers
    WorldServerSignalHandler signalINT, signalTERM; //, signalSEGV
#if WH_PLATFORM == WH_PLATFORM_WINDOWS
    WorldServerSignalHandler signalBREAK;
#endif /* WH_PLATFORM == WH_PLATFORM_WINDOWS */

    ///- Register worldserver's signal handlers
    ACE_Sig_Handler handle;
    handle.register_handler(SIGINT, &signalINT);
    handle.register_handler(SIGTERM, &signalTERM);
#if WH_PLATFORM == WH_PLATFORM_WINDOWS
    handle.register_handler(SIGBREAK, &signalBREAK);
#endif
    //handle.register_handler(SIGSEGV, &signalSEGV);

    ///- Launch Runnable's thread
    warhead::Thread worldThread(new WorldRunnable);
    warhead::Thread rarThread(new RARunnable);
    warhead::Thread auctionLising_thread(new AuctionListRunnable);
    warhead::Thread* cliThread = nullptr;   
    warhead::Thread* soapThread = nullptr;
    warhead::Thread* freezeThread = nullptr;

    // Set thread priority
    worldThread.setPriority(warhead::Priority_Highest);
    auctionLising_thread.setPriority(warhead::Priority_High);

#if WH_PLATFORM == WH_PLATFORM_WINDOWS
    if (sConfigMgr->GetBoolDefault("Console.Enable", true) && (m_ServiceStatus == -1)/* need disable console in service mode*/)
#else
    if (sConfigMgr->GetBoolDefault("Console.Enable", true))
#endif
    {
        ///- Launch CliRunnable thread
        cliThread = new warhead::Thread(new CliRunnable);
    }    

#if defined(WH_PLATFORM_WINDOWS) || defined(WH_PLATFORM_UNIX)

    ///- Handle affinity for multiple processors and process priority
    uint32 affinity = sConfigMgr->GetIntDefault("UseProcessors", 0);
    bool highPriority = sConfigMgr->GetBoolDefault("ProcessPriority", false);

#if WH_PLATFORM == WH_PLATFORM_WINDOWS // Windows

    HANDLE hProcess = GetCurrentProcess();

    if (affinity > 0)
    {
        ULONG_PTR appAff;
        ULONG_PTR sysAff;

        if (GetProcessAffinityMask(hProcess, &appAff, &sysAff))
        {
            ULONG_PTR currentAffinity = affinity & appAff;            // remove non accessible processors

            if (!currentAffinity)
                sLog->outError("Processors marked in UseProcessors bitmask (hex) %x are not accessible for the worldserver. Accessible processors bitmask (hex): %x", affinity, appAff);
            else if (SetProcessAffinityMask(hProcess, currentAffinity))
                sLog->outString("Using processors (bitmask, hex): %x", currentAffinity);
            else
                sLog->outError("Can't set used processors (hex): %x", currentAffinity);
        }
    }

    if (highPriority)
    {
        if (SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS))
            sLog->outString("worldserver process priority class set to HIGH");
        else
            sLog->outError("Can't set worldserver process priority class.");
    }

#else // Linux

    if (affinity > 0)
    {
        cpu_set_t mask;
        CPU_ZERO(&mask);

        for (unsigned int i = 0; i < sizeof(affinity) * 8; ++i)
            if (affinity & (1 << i))
                CPU_SET(i, &mask);

        if (sched_setaffinity(0, sizeof(mask), &mask))
            sLog->outError("Can't set used processors (hex): %x, error: %s", affinity, strerror(errno));
        else
        {
            CPU_ZERO(&mask);
            sched_getaffinity(0, sizeof(mask), &mask);
            sLog->outString("Using processors (bitmask, hex): %lx", *(__cpu_mask*)(&mask));
        }
    }

    if (highPriority)
    {
        if (setpriority(PRIO_PROCESS, 0, PROCESS_HIGH_PRIORITY))
            sLog->outError("Can't set worldserver process priority class, error: %s", strerror(errno));
        else
            sLog->outString("worldserver process priority class set to %i", getpriority(PRIO_PROCESS, 0));
    }

#endif
#endif
    // Start soap serving thread    
    if (sConfigMgr->GetBoolDefault("SOAP.Enabled", false))
    {
        ACSoapRunnable* runnable = new ACSoapRunnable();
        runnable->SetListenArguments(sConfigMgr->GetStringDefault("SOAP.IP", "127.0.0.1"), uint16(sConfigMgr->GetIntDefault("SOAP.Port", 7878)));
        soapThread = new warhead::Thread(runnable);
    }

    // Start up freeze catcher thread    
    if (uint32 freezeDelay = sConfigMgr->GetIntDefault("MaxCoreStuckTime", 0))
    {
        FreezeDetectorRunnable* runnable = new FreezeDetectorRunnable(freezeDelay * 1000);
        freezeThread = new warhead::Thread(runnable);
        freezeThread->setPriority(warhead::Priority_Highest);
    }

    ///- Launch the world listener socket
    uint16 worldPort = uint16(sGameConfig->GetIntConfig("WorldServerPort"));
    std::string bindIp = sConfigMgr->GetStringDefault("BindIP", "0.0.0.0");
    if (sWorldSocketMgr->StartNetwork(worldPort, bindIp.c_str()) == -1)
    {
        sLog->outError("Failed to start network");
        World::StopNow(ERROR_EXIT_CODE); // go down and shutdown the server        
    }

    // set server online (allow connecting now)
    LoginDatabase.DirectPExecute("UPDATE realmlist SET flag = flag & ~%u, population = 0 WHERE id = '%u'", REALM_FLAG_INVALID, realmID);

    sLog->outString("%s (worldserver-daemon) ready...", GitRevision::GetFullVersion());

    // when the main thread closes the singletons get unloaded
    // since worldrunnable uses them, it will crash if unloaded after master
    worldThread.wait();
    rarThread.wait();
    auctionLising_thread.wait();

    if (soapThread)
    {
        soapThread->wait();
        soapThread->destroy();
        delete soapThread;
    }

    if (freezeThread)
    {
        freezeThread->wait();
        delete freezeThread;
    }

    // set server offline
    LoginDatabase.DirectPExecute("UPDATE realmlist SET flag = flag | %u WHERE id = '%d'", REALM_FLAG_OFFLINE, realmID);

    ///- Clean database before leaving
    ClearOnlineAccounts();

    _StopDB();

    sLog->outString("Halting process...");

    if (cliThread)
    {
#if WH_PLATFORM == WH_PLATFORM_WINDOWS
        // this only way to terminate CLI thread exist at Win32 (alt. way exist only in Windows Vista API)
        //_exit(1);
        // send keyboard input to safely unblock the CLI thread
        INPUT_RECORD b[4];
        HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
        b[0].EventType = KEY_EVENT;
        b[0].Event.KeyEvent.bKeyDown = TRUE;
        b[0].Event.KeyEvent.uChar.AsciiChar = 'X';
        b[0].Event.KeyEvent.wVirtualKeyCode = 'X';
        b[0].Event.KeyEvent.wRepeatCount = 1;

        b[1].EventType = KEY_EVENT;
        b[1].Event.KeyEvent.bKeyDown = FALSE;
        b[1].Event.KeyEvent.uChar.AsciiChar = 'X';
        b[1].Event.KeyEvent.wVirtualKeyCode = 'X';
        b[1].Event.KeyEvent.wRepeatCount = 1;

        b[2].EventType = KEY_EVENT;
        b[2].Event.KeyEvent.bKeyDown = TRUE;
        b[2].Event.KeyEvent.dwControlKeyState = 0;
        b[2].Event.KeyEvent.uChar.AsciiChar = '\r';
        b[2].Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
        b[2].Event.KeyEvent.wRepeatCount = 1;
        b[2].Event.KeyEvent.wVirtualScanCode = 0x1c;

        b[3].EventType = KEY_EVENT;
        b[3].Event.KeyEvent.bKeyDown = FALSE;
        b[3].Event.KeyEvent.dwControlKeyState = 0;
        b[3].Event.KeyEvent.uChar.AsciiChar = '\r';
        b[3].Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
        b[3].Event.KeyEvent.wVirtualScanCode = 0x1c;
        b[3].Event.KeyEvent.wRepeatCount = 1;
        DWORD numb;
        WriteConsoleInput(hStdIn, b, 4, &numb);

        cliThread->wait();
#else
        cliThread->destroy();
#endif
        delete cliThread;
    }

    // for some unknown reason, unloading scripts here and not in worldrunnable
    // fixes a memory leak related to detaching threads from the module
    //UnloadScriptingModule();

    OpenSSLCrypto::threadsCleanup();

    // Exit the process with specified return value
    // 0 - normal shutdown
    // 1 - shutdown at error
    // 2 - restart command used, this code can be used by restarter for restart Trinityd
    return World::GetExitCode();
}

/// Initialize connection to the databases
bool _StartDB()
{
    MySQL::Library_Init();

    // Load databases
    DatabaseLoader loader("server.worldserver", DatabaseLoader::DATABASE_NONE);
    loader
        .AddDatabase(LoginDatabase, "Login")
        .AddDatabase(CharacterDatabase, "Character")
        .AddDatabase(WorldDatabase, "World");

    if (!loader.Load())
        return false;

    ///- Get the realm Id from the configuration file
    realmID = sConfigMgr->GetIntDefault("RealmID", 0);
    if (!realmID)
    {
        sLog->outError("Realm ID not defined in configuration file");
        return false;
    }
    else if (realmID > 255)
    {
        /*
         * Due to the client only being able to read a realmID
         * with a size of uint8 we can "only" store up to 255 realms
         * anything further the client will behave anormaly
        */
        sLog->outError("Realm ID must range from 1 to 255");
        return false;
    }

    LOG_INFO("server.loading", "Loading world information...");
    LOG_INFO("server.loading", "> RealmID:              %u", realmID);

    ///- Clean the database before starting
    ClearOnlineAccounts();

    ///- Insert version info into DB
    WorldDatabase.PExecute("UPDATE version SET core_version = '%s', core_revision = '%s'", GitRevision::GetFullVersion(), GitRevision::GetHash());        // One-time query

    sWorld->LoadDBVersion();

    LOG_INFO("server.loading", "> Version DB world:     %s", sWorld->GetDBVersion());
    LOG_INFO("server.loading", "");

    return true;
}

/// Closing connection to the databases
void _StopDB()
{
    CharacterDatabase.Close();
    WorldDatabase.Close();
    LoginDatabase.Close();

    MySQL::Library_End();
}

/// Clear 'online' status for all accounts with characters in this realm
void ClearOnlineAccounts()
{
    // Reset online status for all accounts with characters on the current realm
    // pussywizard: tc query would set online=0 even if logged in on another realm >_>
    LoginDatabase.DirectPExecute("UPDATE account SET online = 0 WHERE online = %u", realmID);

    // Reset online status for all characters
    CharacterDatabase.DirectExecute("UPDATE characters SET online = 0 WHERE online <> 0");
}

/// @}
