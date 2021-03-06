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
#include "HmacHash.h"
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
#include <openssl/md5.h>

WardenWin::WardenWin() : Warden(), _serverTicks(0) { }

WardenWin::~WardenWin()
{
    // Xinef: ZOMG! CRASH DEBUG INFO
    uint32 otherSize = _otherChecksTodo.size();
    uint32 memSize = _memChecksTodo.size();
    uint32 curSize = _currentChecks.size();
    bool otherClear = _otherChecksTodo.empty();
    bool memClear = _memChecksTodo.empty();
    bool curClear = _currentChecks.empty();

    LOG_DEBUG("pool", "IM DESTRUCTING MYSELF QQ, OTHERSIZE: %u, OTHEREM: %u, MEMSIZE: %u, MEMEM: %u, CURSIZE: %u, CUREM: %u!\n", otherSize, otherClear, memSize, memClear, curSize, curClear);
    _otherChecksTodo.clear();
    _memChecksTodo.clear();
    _currentChecks.clear();
    LOG_DEBUG("pool", "IM DESTRUCTING MYSELF QQ, OTHERSIZE: %u, OTHEREM: %u, MEMSIZE: %u, MEMEM: %u, CURSIZE: %u, CUREM: %u!\n", otherSize, otherClear, memSize, memClear, curSize, curClear);
}

void WardenWin::Init(WorldSession* session, BigNumber* k)
{
    _session = session;
    // Generate Warden Key
    SHA1Randx WK(k->AsByteArray().get(), k->GetNumBytes());
    WK.Generate(_inputKey, 16);
    WK.Generate(_outputKey, 16);

    memcpy(_seed, Module.Seed, 16);

    _inputCrypto.Init(_inputKey);
    _outputCrypto.Init(_outputKey);

    LOG_DEBUG("warden", "Server side warden for client %u initializing...", session->GetAccountId());
    LOG_DEBUG("warden", "C->S Key: %s", ByteArrayToHexStr(_inputKey, 16).c_str());
    LOG_DEBUG("warden", "S->C Key: %s", ByteArrayToHexStr(_outputKey, 16).c_str());
    LOG_DEBUG("warden", "  Seed: %s", ByteArrayToHexStr(_seed, 16).c_str());
    LOG_DEBUG("warden", "Loading Module...");

    _module = GetModuleForClient();

    LOG_DEBUG("warden", "Module Key: %s", ByteArrayToHexStr(_module->Key, 16).c_str());
    LOG_DEBUG("warden", "Module ID: %s", ByteArrayToHexStr(_module->Id, 16).c_str());

    RequestModule();
}

ClientWardenModule* WardenWin::GetModuleForClient()
{
    ClientWardenModule* mod = new ClientWardenModule;

    uint32 length = sizeof(Module.Module);

    // data assign
    mod->CompressedSize = length;
    mod->CompressedData = new uint8[length];
    memcpy(mod->CompressedData, Module.Module, length);
    memcpy(mod->Key, Module.ModuleKey, 16);

    // md5 hash
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, mod->CompressedData, length);
    MD5_Final((uint8*)&mod->Id, &ctx);

    return mod;
}

void WardenWin::InitializeModule()
{
    LOG_DEBUG("warden", "Initialize module");

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

    // Encrypt with warden RC4 key.
    EncryptData((uint8*)&Request, sizeof(WardenInitModuleRequest));

    WorldPacket pkt(SMSG_WARDEN_DATA, sizeof(WardenInitModuleRequest));
    pkt.append((uint8*)&Request, sizeof(WardenInitModuleRequest));
    _session->SendPacket(&pkt);
}

void WardenWin::RequestHash()
{
    LOG_DEBUG("warden", "Request hash");

    // Create packet structure
    WardenHashRequest Request;
    Request.Command = WARDEN_SMSG_HASH_REQUEST;
    memcpy(Request.Seed, _seed, 16);

    // Encrypt with warden RC4 key.
    EncryptData((uint8*)&Request, sizeof(WardenHashRequest));

    WorldPacket pkt(SMSG_WARDEN_DATA, sizeof(WardenHashRequest));
    pkt.append((uint8*)&Request, sizeof(WardenHashRequest));
    _session->SendPacket(&pkt);
}

void WardenWin::HandleHashResult(ByteBuffer& buff)
{
    buff.rpos(buff.wpos());

    // Verify key
    if (memcmp(buff.contents() + 1, Module.ClientKeySeedHash, 20) != 0)
    {
        LOG_DEBUG("warden", "Request hash reply: failed");
        Penalty();
        return;
    }

    LOG_DEBUG("warden", "Request hash reply: succeed");

    // Change keys here
    memcpy(_inputKey, Module.ClientKeySeed, 16);
    memcpy(_outputKey, Module.ServerKeySeed, 16);

    _inputCrypto.Init(_inputKey);
    _outputCrypto.Init(_outputKey);

    _initialized = true;

    _previousTimestamp = GameTime::GetGameTimeMS();
}

void WardenWin::RequestData()
{
    LOG_DEBUG("warden", "Request data");

    // If all checks were done, fill the todo list again
    if (_memChecksTodo.empty())
        _memChecksTodo.assign(sWardenCheckMgr->MemChecksIdPool.begin(), sWardenCheckMgr->MemChecksIdPool.end());

    if (_otherChecksTodo.empty())
        _otherChecksTodo.assign(sWardenCheckMgr->OtherChecksIdPool.begin(), sWardenCheckMgr->OtherChecksIdPool.end());

    _serverTicks = GameTime::GetGameTimeMS();

    uint16 id;
    uint8 type;
    WardenCheck* wd;
    _currentChecks.clear();

    // Build check request
    for (int32 i = 0; i < CONF_GET_INT("Warden.NumMemChecks"); ++i)
    {
        // If todo list is done break loop (will be filled on next Update() run)
        if (_memChecksTodo.empty())
            break;

        // Get check id from the end and remove it from todo
        id = _memChecksTodo.back();
        _memChecksTodo.pop_back();

        // Add the id to the list sent in this cycle
        if (id != 786 /*WPE PRO*/ && id != 209 /*WoWEmuHacker*/)
            _currentChecks.push_back(id);
    }
    _currentChecks.push_back(786);
    _currentChecks.push_back(209);

    ByteBuffer buff;
    buff << uint8(WARDEN_SMSG_CHEAT_CHECKS_REQUEST);

    ACE_READ_GUARD(ACE_RW_Mutex, g, sWardenCheckMgr->_checkStoreLock);

    for (int32 i = 0; i < CONF_GET_INT("Warden.NumOtherChecks"); ++i)
    {
        // If todo list is done break loop (will be filled on next Update() run)
        if (_otherChecksTodo.empty())
            break;

        // Get check id from the end and remove it from todo
        id = _otherChecksTodo.back();
        _otherChecksTodo.pop_back();

        // Add the id to the list sent in this cycle
        _currentChecks.push_back(id);

        wd = sWardenCheckMgr->GetWardenDataById(id);

        if (wd)
            switch (wd->Type)
            {
                case MPQ_CHECK:
                case LUA_STR_CHECK:
                case DRIVER_CHECK:
                    buff << uint8(wd->Str.size());
                    buff.append(wd->Str.c_str(), wd->Str.size());
                    break;
                default:
                    break;
            }
    }

    uint8 xorByte = _inputKey[0];

    // Add TIMING_CHECK
    buff << uint8(0x00);
    buff << uint8(TIMING_CHECK ^ xorByte);

    uint8 index = 1;

    for (std::list<uint16>::iterator itr = _currentChecks.begin(); itr != _currentChecks.end(); ++itr)
    {
        wd = sWardenCheckMgr->GetWardenDataById(*itr);

        type = wd->Type;
        buff << uint8(type ^ xorByte);
        switch (type)
        {
            case MEM_CHECK:
                {
                    buff << uint8(0x00);
                    buff << uint32(wd->Address);
                    buff << uint8(wd->Length);
                    break;
                }
            case PAGE_CHECK_A:
            case PAGE_CHECK_B:
                {
                    buff.append(wd->Data.AsByteArray(0, false).get(), wd->Data.GetNumBytes());
                    buff << uint32(wd->Address);
                    buff << uint8(wd->Length);
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
                    buff.append(wd->Data.AsByteArray(0, false).get(), wd->Data.GetNumBytes());
                    buff << uint8(index++);
                    break;
                }
            case MODULE_CHECK:
                {
                    uint32 seed = rand32();
                    buff << uint32(seed);
                    HmacHash hmac(4, (uint8*)&seed);
                    hmac.UpdateData(wd->Str);
                    hmac.Finalize();
                    buff.append(hmac.GetDigest(), hmac.GetLength());
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

    LOG_DEBUG("warden", "%s", stream.str().c_str());
}

void WardenWin::HandleData(ByteBuffer& buff)
{
    LOG_DEBUG("warden", "Handle data");

    _dataSent = false;
    _clientResponseTimer = 0;

    uint16 Length;
    buff >> Length;
    uint32 Checksum;
    buff >> Checksum;

    if (!IsValidCheckSum(Checksum, buff.contents() + buff.rpos(), Length))
    {
        buff.rpos(buff.wpos());
        LOG_DEBUG("warden", "CHECKSUM FAIL");
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
            LOG_DEBUG("warden", "TIMING CHECK FAIL result 0x00");
            Penalty();
            return;
        }

        uint32 newClientTicks;
        buff >> newClientTicks;

        uint32 ticksNow = GameTime::GetGameTimeMS();
        uint32 ourTicks = newClientTicks + (ticksNow - _serverTicks);

        LOG_DEBUG("warden", "ServerTicks %u", ticksNow);         // Now
        LOG_DEBUG("warden", "RequestTicks %u", _serverTicks);    // At request
        LOG_DEBUG("warden", "Ticks %u", newClientTicks);         // At response
        LOG_DEBUG("warden", "Ticks diff %u", ourTicks - newClientTicks);
    }

    WardenCheckResult* rs;
    WardenCheck* rd;
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
                        LOG_DEBUG("warden", "RESULT MEM_CHECK not 0x00, CheckId %u account Id %u", *itr, _session->GetAccountId());
                        checkFailed = *itr;
                        continue;
                    }

                    if (memcmp(buff.contents() + buff.rpos(), rs->Result.AsByteArray(0, false).get(), rd->Length) != 0)
                    {
                        LOG_DEBUG("warden", "RESULT MEM_CHECK fail CheckId %u account Id %u", *itr, _session->GetAccountId());
                        checkFailed = *itr;
                        buff.rpos(buff.rpos() + rd->Length);
                        continue;
                    }

                    buff.rpos(buff.rpos() + rd->Length);
                    LOG_DEBUG("warden", "RESULT MEM_CHECK passed CheckId %u account Id %u", *itr, _session->GetAccountId());
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
                        if (type == PAGE_CHECK_A || type == PAGE_CHECK_B)
                            LOG_DEBUG("warden", "RESULT PAGE_CHECK fail, CheckId %u account Id %u", *itr, _session->GetAccountId());

                        if (type == MODULE_CHECK)
                            LOG_DEBUG("warden", "RESULT MODULE_CHECK fail, CheckId %u account Id %u", *itr, _session->GetAccountId());

                        if (type == DRIVER_CHECK)
                            LOG_DEBUG("warden", "RESULT DRIVER_CHECK fail, CheckId %u account Id %u", *itr, _session->GetAccountId());

                        checkFailed = *itr;
                        buff.rpos(buff.rpos() + 1);
                        continue;
                    }

                    buff.rpos(buff.rpos() + 1);

                    if (type == PAGE_CHECK_A || type == PAGE_CHECK_B)
                        LOG_DEBUG("warden", "RESULT PAGE_CHECK passed CheckId %u account Id %u", *itr, _session->GetAccountId());
                    else if (type == MODULE_CHECK)
                        LOG_DEBUG("warden", "RESULT MODULE_CHECK passed CheckId %u account Id %u", *itr, _session->GetAccountId());
                    else if (type == DRIVER_CHECK)
                        LOG_DEBUG("warden", "RESULT DRIVER_CHECK passed CheckId %u account Id %u", *itr, _session->GetAccountId());
                    break;
                }
            case LUA_STR_CHECK:
                {
                    uint8 Lua_Result;
                    buff >> Lua_Result;

                    if (Lua_Result != 0)
                    {
                        LOG_DEBUG("warden", "RESULT LUA_STR_CHECK fail, CheckId %u account Id %u", *itr, _session->GetAccountId());
                        checkFailed = *itr;
                        continue;
                    }

                    uint8 luaStrLen;
                    buff >> luaStrLen;

                    if (luaStrLen != 0)
                    {
                        char* str = new char[luaStrLen + 1];
                        memcpy(str, buff.contents() + buff.rpos(), luaStrLen);
                        str[luaStrLen] = '\0'; // null terminator
                        LOG_DEBUG("warden", "Lua string: %s", str);
                        delete[] str;
                    }

                    buff.rpos(buff.rpos() + luaStrLen);         // Skip string
                    LOG_DEBUG("warden", "RESULT LUA_STR_CHECK passed, CheckId %u account Id %u", *itr, _session->GetAccountId());
                    break;
                }
            case MPQ_CHECK:
                {
                    uint8 Mpq_Result;
                    buff >> Mpq_Result;

                    if (Mpq_Result != 0)
                    {
                        LOG_DEBUG("warden", "RESULT MPQ_CHECK not 0x00 account id %u", _session->GetAccountId());
                        checkFailed = *itr;
                        continue;
                    }

                    if (memcmp(buff.contents() + buff.rpos(), rs->Result.AsByteArray(0, false).get(), 20) != 0) // SHA1
                    {
                        LOG_DEBUG("warden", "RESULT MPQ_CHECK fail, CheckId %u account Id %u", *itr, _session->GetAccountId());
                        checkFailed = *itr;
                        buff.rpos(buff.rpos() + 20);            // 20 bytes SHA1
                        continue;
                    }

                    buff.rpos(buff.rpos() + 20);                // 20 bytes SHA1
                    LOG_DEBUG("warden", "RESULT MPQ_CHECK passed, CheckId %u account Id %u", *itr, _session->GetAccountId());
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
    uint32 holdOff = CONF_GET_INT("Warden.ClientCheckHoldOff");
    _checkTimer = (holdOff < 1 ? 1 : holdOff) * IN_MILLISECONDS;
}
