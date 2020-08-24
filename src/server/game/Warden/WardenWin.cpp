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

#include "Common.h"
#include "HMACSHA1.h"
#include "WardenKeyGeneration.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Log.h"
#include "Opcodes.h"
#include "ByteBuffer.h"
#include "DatabaseEnv.h"
#include "Player.h"
#include "Util.h"
#include "WardenWin.h"
#include "WardenModuleWin.h"
#include "WardenCheckMgr.h"
#include "AccountMgr.h"
#include "GameTime.h"
#include "GameConfig.h"
#include "SmartEnum.h"
#include "Containers.h"

WardenWin::WardenWin() : Warden(), _serverTicks(0)
{
    _memChecks = sWardenCheckMgr->GetAvailableMemoryChecks();
    warhead::Containers::RandomShuffle(_memChecks);
    _memChecksIt = _memChecks.begin();
    _otherChecks = sWardenCheckMgr->GetAvailableOtherChecks();
    warhead::Containers::RandomShuffle(_otherChecks);
    _otherChecksIt = _otherChecks.begin();
}

void WardenWin::Init(WorldSession* session, BigNumber *k)
{
    _session = session;
    // Generate Warden Key
    SessionKeyGenerator<warhead::SHA1> WK(K);
    WK.Generate(_inputKey.data(), _inputKey.size());
    WK.Generate(_outputKey.data(), _outputKey.size());

    _seed = Module.Seed;

    _inputCrypto.Init(_inputKey);
    _outputCrypto.Init(_outputKey);
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
    LOG_DEBUG("warden", "Server side warden for client %u initializing...", session->GetAccountId());
    LOG_DEBUG("warden", "C->S Key: %s", ByteArrayToHexStr(_inputKey, 16).c_str());
    LOG_DEBUG("warden", "S->C Key: %s", ByteArrayToHexStr(_outputKey, 16).c_str());
    LOG_DEBUG("warden", "  Seed: %s", ByteArrayToHexStr(_seed, 16).c_str());
    LOG_DEBUG("warden", "Loading Module...");
#endif

    MakeModuleForClient();

#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
    LOG_DEBUG("warden", "Module Key: %s", ByteArrayToHexStr(_module->Key, 16).c_str());
    LOG_DEBUG("warden", "Module ID: %s", ByteArrayToHexStr(_module->Id, 16).c_str());
#endif
    RequestModule();
}

void WardenWin::InitializeModuleForClient(ClientWardenModule& module)
{
    // data assign
    module.CompressedData = Module.Module.data();
    module.CompressedSize = Module.Module.size();
    module.Key = Module.ModuleKey;
}

void WardenWin::InitializeModule()
{
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
    LOG_DEBUG("warden", "Initialize module");
#endif

    // Create packet structure
    WardenInitModuleRequest Request;
    Request.Command1 = WARDEN_SMSG_MODULE_INITIALIZE;
    Request.Size1 = 20;
    Request.Unk1 = 1;
    Request.Unk2 = 0;
    Request.Type = 1;
    Request.String_library1 = 0;
    Request.Function1[0] = 0x00024F80;                      // 0x00400000 + 0x00024F80 SFileOpenFile
    Request.Function1[1] = 0x000218C0;                      // 0x00400000 + 0x000218C0 SFileGetFileSize
    Request.Function1[2] = 0x00022530;                      // 0x00400000 + 0x00022530 SFileReadFile
    Request.Function1[3] = 0x00022910;                      // 0x00400000 + 0x00022910 SFileCloseFile
    Request.CheckSumm1 = BuildChecksum(&Request.Unk1, 20);

    Request.Command2 = WARDEN_SMSG_MODULE_INITIALIZE;
    Request.Size2 = 8;
    Request.Unk3 = 4;
    Request.Unk4 = 0;
    Request.String_library2 = 0;
    Request.Function2 = 0x00419D40;                         // 0x00400000 + 0x00419D40 FrameScript::GetText
    Request.Function2_set = 1;
    Request.CheckSumm2 = BuildChecksum(&Request.Unk2, 8);

    Request.Command3 = WARDEN_SMSG_MODULE_INITIALIZE;
    Request.Size3 = 8;
    Request.Unk5 = 1;
    Request.Unk6 = 1;
    Request.String_library3 = 0;
    Request.Function3 = 0x0046AE20;                         // 0x00400000 + 0x0046AE20 PerformanceCounter
    Request.Function3_set = 1;
    Request.CheckSumm3 = BuildChecksum(&Request.Unk5, 8);

    EndianConvert(Request.Size1);
    EndianConvert(Request.CheckSumm1);
    EndianConvert(Request.Function1[0]);
    EndianConvert(Request.Function1[1]);
    EndianConvert(Request.Function1[2]);
    EndianConvert(Request.Function1[3]);
    EndianConvert(Request.Size2);
    EndianConvert(Request.CheckSumm2);
    EndianConvert(Request.Function2);
    EndianConvert(Request.Size3);
    EndianConvert(Request.CheckSumm3);
    EndianConvert(Request.Function3);

    // Encrypt with warden RC4 key.
    EncryptData(reinterpret_cast<uint8*>(&Request), sizeof(WardenInitModuleRequest));

    WorldPacket pkt(SMSG_WARDEN_DATA, sizeof(WardenInitModuleRequest));
    pkt.append(reinterpret_cast<uint8*>(&Request), sizeof(WardenInitModuleRequest));
    _session->SendPacket(&pkt);
}

void WardenWin::RequestHash()
{
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
    LOG_DEBUG("warden", "Request hash");
#endif

    // Create packet structure
    WardenHashRequest Request;
    Request.Command = WARDEN_SMSG_HASH_REQUEST;
    Request.Seed = _seed;

    // Encrypt with warden RC4 key.
    EncryptData(reinterpret_cast<uint8*>(&Request), sizeof(WardenHashRequest));

    WorldPacket pkt(SMSG_WARDEN_DATA, sizeof(WardenHashRequest));
    pkt.append(reinterpret_cast<uint8*>(&Request), sizeof(WardenHashRequest));
    _session->SendPacket(&pkt);
}

void WardenWin::HandleHashResult(ByteBuffer &buff)
{
    warhead::SHA1::Digest response;
    buff.read(response);
    if (response != Module.ClientKeySeedHash)
    {
        LOG_DEBUG("warden", "Request hash reply: failed");
        char const* penalty = ApplyPenalty(nullptr);
        LOG_WARN("warden", "%s failed hash reply. Action: %s", _session->GetPlayerInfo().c_str(), penalty);
        return;
    }

    LOG_DEBUG("warden", "Request hash reply: succeed");

    // Change keys here
    _inputKey = Module.ClientKeySeed;
    _outputKey = Module.ServerKeySeed;

    _inputCrypto.Init(_inputKey);
    _outputCrypto.Init(_outputKey);

    _initialized = true;
}

void WardenWin::RequestChecks()
{
    LOG_DEBUG("warden", "Request data");

    // If all checks were done, fill the todo list again
    if (_memChecksIt == _memChecks.end())
    {
        LOG_DEBUG("warden", "Finished all mem checks, re-shuffling");
        warhead::Containers::RandomShuffle(_memChecks);
        _memChecksIt = _memChecks.begin();
    }

    if (_otherChecksIt == _otherChecks.end())
    {
        LOG_DEBUG("warden", "Finished all other checks, re-shuffling");
        warhead::Containers::RandomShuffle(_otherChecks);
        _otherChecksIt = _otherChecks.begin();
    }

    _serverTicks = GameTime::GetGameTimeMS();

    _currentChecks.clear();

    // Build check request
    ByteBuffer buff;
    buff << uint8(WARDEN_SMSG_CHEAT_CHECKS_REQUEST);
    for (int32 i = 0; i < sGameConfig->GetIntConfig("Warden.NumMemChecks"); ++i)
    {
        // If todo list is done break loop (will be filled on next Update() run)
        if (_memChecksIt == _memChecks.end())
            break;

        _currentChecks.push_back(*(_memChecksIt++));
    }
    _currentChecks.push_back(786); _currentChecks.push_back(209);

    for (int32 i = 0; i < sGameConfig->GetIntConfig("Warden.NumOtherChecks"); ++i)
    {
        // If todo list is done break loop (will be filled on next Update() run)
        if (_otherChecksIt == _otherChecks.end())
            break;

        uint16 const id = *(_otherChecksIt++);

        WardenCheck const& check = sWardenCheckMgr->GetCheckDataById(id);
        if (!check.Str.empty())
        {
            buff << uint8(check.Str.size());
            buff.append(check.Str.data(), check.Str.size());
        }
        _currentChecks.push_back(id);
    }

    uint8 xorByte = _inputKey[0];

    // Add TIMING_CHECK
    buff << uint8(0x00);
    buff << uint8(TIMING_CHECK ^ xorByte);

    uint8 index = 1;

    for (uint16 const id : _currentChecks)
    {
        WardenCheck const& check = sWardenCheckMgr->GetCheckDataById(id);

        WardenCheckType const type = check.Type;
        buff << uint8(type ^ xorByte);
        switch (type)
        {
            case MEM_CHECK:
            {
                buff << uint8(0x00);
                buff << uint32(check.Address);
                buff << uint8(check.Length);
                break;
            }
            case PAGE_CHECK_A:
            case PAGE_CHECK_B:
            {
                buff.append(check.Data.data(), check.Data.size());
                buff << uint32(check.Address);
                buff << uint8(check.Length);
                break;
            }
            case MPQ_CHECK:
            case LUA_STR_CHECK:
            {
                buff << uint8(index++);
                break;
            }
            case DRIVER_CHECK:
            {
                buff.append(check.Data.data(), check.Data.size());
                buff << uint8(index++);
                break;
            }
            case MODULE_CHECK:
            {
                std::array<uint8, 4> seed = warhead::GetRandomBytes<4>();
                buff.append(seed);
                buff.append(warhead::HMAC_SHA1::GetDigestOf(seed, wd->Str));
                buff.append(warhead::HMAC_SHA1::GetDigestOf(seed, check.Str));
                break;
            }
            /*case PROC_CHECK:
            {
                buff.append(wd->i.AsByteArray(0, false).get(), wd->i.GetNumBytes());
                buff << uint8(index++);
                buff << uint8(index++);
                buff << uint32(wd->Address);
                buff << uint8(wd->Length);
                break;
            }*/
            default:
                break;                                      // Should never happen
        }
    }
    buff << uint8(xorByte);
    buff.hexlike();

    // Encrypt with warden RC4 key
    EncryptData(buff.contents(), buff.size());

    WorldPacket pkt(SMSG_WARDEN_DATA, buff.size());
    pkt.append(buff);
    _session->SendPacket(&pkt);

    _dataSent = true;

    std::stringstream stream;
    stream << "Sent check id's: ";
    for (std::list<uint16>::iterator itr = _currentChecks.begin(); itr != _currentChecks.end(); ++itr)
        stream << *itr << " ";

#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
    LOG_DEBUG("warden", "%s", stream.str().c_str());
#endif
}

void WardenWin::HandleData(ByteBuffer &buff)
{
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
    LOG_DEBUG("warden", "Handle data");
#endif

    _dataSent = false;
    _clientResponseTimer = 0;

    uint16 Length;
    buff >> Length;
    uint32 Checksum;
    buff >> Checksum;

    if (!IsValidCheckSum(Checksum, buff.contents() + buff.rpos(), Length))
    {
        buff.rpos(buff.wpos());
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
        LOG_DEBUG("warden", "CHECKSUM FAIL");
#endif
        Penalty();
        return;
    }

    // TIMING_CHECK
    {
        uint8 result;
        buff >> result;
        // TODO: test it.
        if (result == 0x00)
        {
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
            LOG_DEBUG("warden", "TIMING CHECK FAIL result 0x00");
#endif
            Penalty();
            return;
        }

        uint32 newClientTicks;
        buff >> newClientTicks;

#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
        uint32 ticksNow = GameTime::GetGameTimeMS();
        uint32 ourTicks = newClientTicks + (ticksNow - _serverTicks);

        LOG_DEBUG("warden", "ServerTicks %u", ticksNow);         // Now
        LOG_DEBUG("warden", "RequestTicks %u", _serverTicks);    // At request
        LOG_DEBUG("warden", "Ticks %u", newClientTicks);         // At response
        LOG_DEBUG("warden", "Ticks diff %u", ourTicks - newClientTicks);
#endif
    }

    WardenCheckResult *rs;
    WardenCheck *rd;
    uint8 type;
    uint16 checkFailed = 0;

    ACE_READ_GUARD(ACE_RW_Mutex, g, sWardenCheckMgr->_checkStoreLock);

    for (std::list<uint16>::iterator itr = _currentChecks.begin(); itr != _currentChecks.end(); ++itr)
    {
        rd = sWardenCheckMgr->GetWardenDataById(*itr);
        rs = sWardenCheckMgr->GetWardenResultById(*itr);

        type = rd->Type;
        switch (type)
        {
            case MEM_CHECK:
            {
                uint8 Mem_Result;
                buff >> Mem_Result;

                if (Mem_Result != 0)
                {
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
                    LOG_DEBUG("warden", "RESULT MEM_CHECK not 0x00, CheckId %u account Id %u", *itr, _session->GetAccountId());
#endif
                    checkFailed = *itr;
                    continue;
                }

                if (memcmp(buff.contents() + buff.rpos(), rs->Result.AsByteArray(0, false).get(), rd->Length) != 0)
                {
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
                    LOG_DEBUG("warden", "RESULT MEM_CHECK fail CheckId %u account Id %u", *itr, _session->GetAccountId());
#endif
                    checkFailed = *itr;
                    buff.rpos(buff.rpos() + rd->Length);
                    continue;
                }

                buff.rpos(buff.rpos() + rd->Length);
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
                LOG_DEBUG("warden", "RESULT MEM_CHECK passed CheckId %u account Id %u", *itr, _session->GetAccountId());
#endif
                break;
            }
            case PAGE_CHECK_A:
            case PAGE_CHECK_B:
            case DRIVER_CHECK:
            case MODULE_CHECK:
            {
                const uint8 byte = 0xE9;
                if (memcmp(buff.contents() + buff.rpos(), &byte, sizeof(uint8)) != 0)
                {
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
                    if (type == PAGE_CHECK_A || type == PAGE_CHECK_B)
                        LOG_DEBUG("warden", "RESULT PAGE_CHECK fail, CheckId %u account Id %u", *itr, _session->GetAccountId());

                    if (type == MODULE_CHECK)
                        LOG_DEBUG("warden", "RESULT MODULE_CHECK fail, CheckId %u account Id %u", *itr, _session->GetAccountId());

                    if (type == DRIVER_CHECK)
                        LOG_DEBUG("warden", "RESULT DRIVER_CHECK fail, CheckId %u account Id %u", *itr, _session->GetAccountId());
#endif
                    checkFailed = *itr;
                    buff.rpos(buff.rpos() + 1);
                    continue;
                }

                buff.rpos(buff.rpos() + 1);

#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
            if (type == PAGE_CHECK_A || type == PAGE_CHECK_B)
                LOG_DEBUG("warden", "RESULT PAGE_CHECK passed CheckId %u account Id %u", *itr, _session->GetAccountId());
            else if (type == MODULE_CHECK)
                LOG_DEBUG("warden", "RESULT MODULE_CHECK passed CheckId %u account Id %u", *itr, _session->GetAccountId());
            else if (type == DRIVER_CHECK)
                LOG_DEBUG("warden", "RESULT DRIVER_CHECK passed CheckId %u account Id %u", *itr, _session->GetAccountId());
#endif
                break;
            }
            case LUA_STR_CHECK:
            {
                uint8 Lua_Result;
                buff >> Lua_Result;

                if (Lua_Result != 0)
                {
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
                    LOG_DEBUG("warden", "RESULT LUA_STR_CHECK fail, CheckId %u account Id %u", *itr, _session->GetAccountId());
#endif
                    checkFailed = *itr;
                    continue;
                }

                uint8 luaStrLen;
                buff >> luaStrLen;

                if (luaStrLen != 0)
                {
                    char *str = new char[luaStrLen + 1];
                    memcpy(str, buff.contents() + buff.rpos(), luaStrLen);
                    str[luaStrLen] = '\0'; // null terminator
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
                    LOG_DEBUG("warden", "Lua string: %s", str);
#endif
                    delete[] str;
                }
                buff.rpos(buff.rpos() + luaStrLen);         // Skip string
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
                LOG_DEBUG("warden", "RESULT LUA_STR_CHECK passed, CheckId %u account Id %u", *itr, _session->GetAccountId());
#endif
                break;
            }
            case MPQ_CHECK:
            {
                uint8 Mpq_Result;
                buff >> Mpq_Result;

                if (Mpq_Result != 0)
                {
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
                    LOG_DEBUG("warden", "RESULT MPQ_CHECK not 0x00 account id %u", _session->GetAccountId());
#endif
                    checkFailed = *itr;
                    continue;
                }

                if (memcmp(buff.contents() + buff.rpos(), rs->Result.AsByteArray(0, false).get(), 20) != 0) // SHA1
                {
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
                    LOG_DEBUG("warden", "RESULT MPQ_CHECK fail, CheckId %u account Id %u", *itr, _session->GetAccountId());
#endif
                    checkFailed = *itr;
                    buff.rpos(buff.rpos() + 20);            // 20 bytes SHA1
                    continue;
                }

                buff.rpos(buff.rpos() + 20);                // 20 bytes SHA1
#if defined(ENABLE_EXTRAS) && defined(ENABLE_EXTRA_LOGS)
                LOG_DEBUG("warden", "RESULT MPQ_CHECK passed, CheckId %u account Id %u", *itr, _session->GetAccountId());
#endif
                break;
            }
            default:                                        // Should never happen
                break;
        }
    }

    if (checkFailed > 0)
    {
        WardenCheck* check = sWardenCheckMgr->GetWardenDataById(checkFailed);
        Penalty(check, checkFailed);
    }

    // Set hold off timer, minimum timer should at least be 1 second
    uint32 holdOff = sGameConfig->GetIntConfig("Warden.ClientCheckHoldOff");
    _checkTimer = (holdOff < 1 ? 1 : holdOff) * IN_MILLISECONDS;
}
