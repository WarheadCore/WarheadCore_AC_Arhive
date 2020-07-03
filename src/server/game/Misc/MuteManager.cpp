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

#include "MuteManager.h"
#include "DatabaseEnv.h"
#include "GameTime.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Language.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "World.h"

MuteManager* MuteManager::instance()
{
    static MuteManager instance;
    return &instance;
}

void MuteManager::MutePlayer(std::string const targetName, uint32 notSpeakTime, std::string const muteBy, std::string const muteReason)
{
    auto targetSession = sWorld->FindSession(sObjectMgr->GetPlayerAccountIdByPlayerName(targetName));
    uint32 accountId = targetSession ? targetSession->GetAccountId() : sObjectMgr->GetPlayerAccountIdByPlayerName(targetName);

    // INSERT INTO `account_muted` (`accountid`, `mutedate`, `mutetime`, `mutedby`, `mutereason`, `active`) VALUES (?, ?, ?, ?, ?, 1)
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT_MUTE);
    stmt->setUInt32(0, accountId);

    uint32 muteDate = GameTime::GetGameTime() + notSpeakTime * MINUTE;

    /*
     * Mute will be in effect right away.
     * If Mute.AddAfterLogin.Enable mute will be in effect starting from the next login.
     */
    int32 muteTime = CONF_GET_BOOL("Mute.AddAfterLogin.Enable") && !targetSession ? -int32(notSpeakTime * MINUTE) : notSpeakTime * MINUTE;

    if (targetSession)
        AddMuteTime(accountId, muteDate);
    
    stmt->setUInt32(1, static_cast<uint32>(GameTime::GetGameTime()));
    stmt->setInt64(2, muteTime);
    stmt->setString(3, muteBy);
    stmt->setString(4, muteReason);
    LoginDatabase.Execute(stmt);

    auto GetPlayerLink = [&]() -> std::string
    {
        return "|cffffffff|Hplayer:" + targetName + "|h[" + targetName + "]|h|r";
    };

    if (CONF_GET_BOOL("ShowMuteInWorld"))
        sWorld->SendWorldText(LANG_COMMAND_MUTEMESSAGE_WORLD, muteBy.c_str(), GetPlayerLink().c_str(), notSpeakTime, muteReason.c_str());

    if (targetSession)
        ChatHandler(targetSession).PSendSysMessage(LANG_YOUR_CHAT_DISABLED, notSpeakTime, muteBy.c_str(), muteReason.c_str());
}

void MuteManager::UnMutePlayer(std::string const targetName)
{
    uint32 accID = sObjectMgr->GetPlayerAccountIdByPlayerName(targetName);

    DeleteMuteTime(accID);

    if (auto targetSession = sWorld->FindSession(accID))
        ChatHandler(targetSession).PSendSysMessage(LANG_YOUR_CHAT_ENABLED);   
}

void MuteManager::AddMuteTime(uint32 accountID, uint32 muteTime)
{
    // Check exist
    auto itr = _listSessions.find(accountID);
    if (itr != _listSessions.end())
        return;

    _listSessions.insert(std::make_pair(accountID, muteTime));
}

void MuteManager::SetMuteTime(uint32 accountID, uint32 muteTime)
{
    // Check empty
    auto itr = _listSessions.find(accountID);
    if (itr == _listSessions.end())
        return;

    LOG_WARN("module", "SetMuteTime %u", muteTime);

    _listSessions[accountID] = muteTime;
}

uint32 MuteManager::GetMuteTime(uint32 accountID)
{
    // Check exist
    auto itr = _listSessions.find(accountID);
    if (itr == _listSessions.end())
        return 0;

    return _listSessions[accountID];
}

void MuteManager::DeleteMuteTime(uint32 accountID, bool delFromDB /*= true*/)
{
    if (delFromDB)
    {
        // UPDATE `account_muted` SET `active` = 0 WHERE `active` = 1 AND `accountid` = ?
        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_ACCOUNT_MUTE);
        stmt->setUInt32(0, accountID);
        LoginDatabase.Execute(stmt);
    }

    // Check exist account muted
    auto itr = _listSessions.find(accountID);
    if (itr == _listSessions.end())
        return;

    _listSessions.erase(accountID);
}

void MuteManager::CheckMuteExpired(uint32 accountID)
{
    uint32 _muteTime = GetMuteTime(accountID);
    uint32 timeNow = GameTime::GetGameTime();

    if (!_muteTime || _muteTime > timeNow)
        return;

    DeleteMuteTime(accountID);
}

std::string const MuteManager::GetMuteTimeString(uint32 accountID)
{
    uint32 _muteTime = GetMuteTime(accountID);

    if (!_muteTime)
        return "";

    return secsToTimeString(_muteTime - GameTime::GetGameTime());
}

bool MuteManager::CanSpeak(uint32 accountID)
{
    return GetMuteTime(accountID) <= GameTime::GetGameTime();
}

void MuteManager::LoginAccount(uint32 accountID)
{
    // Set inactive if expired
    // UPDATE `account_muted` SET `active` = 0 WHERE `active` = 1 AND `mutetime` > 0 AND `accountid` = ? AND UNIX_TIMESTAMP() >= `mutedate` + `mutetime`
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_MUTE_EXPIRED);
    stmt->setUInt32(0, accountID);
    LoginDatabase.Execute(stmt);

    // Get info about mute time after update active
    // SELECT `mutedate`, `mutetime` FROM `account_muted` WHERE `accountid` = ? AND `active` = 1 AND UNIX_TIMESTAMP() <= `mutedate` + ABS(`mutetime`) ORDER BY `mutedate` + ABS(`mutetime`) DESC LIMIT 1
    stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_MUTE);
    stmt->setUInt32(0, accountID);
    
    PreparedQueryResult result = LoginDatabase.Query(stmt);
    if (!result)
        return; // If no info - no mute time :)

    Field* fields = result->Fetch();
    uint32 mutedate = fields[0].GetUInt32();
    int32 mutetime = fields[1].GetInt32();

    AddMuteTime(accountID, mutedate + std::abs(mutetime));

    //! Negative mutetime indicates amount of seconds to be muted effective on next login - which is now.
    if (mutetime < 0)
        UpdateMuteAccount(accountID, mutedate, std::abs(mutetime));
}

void MuteManager::UpdateMuteAccount(uint32 accountID, uint32 muteDate, int32 muteTime)
{
    // UPDATE `account_muted` SET `mutedate` = ?, `mutetime` = ? WHERE `accountid` = ?
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_MUTE);
    stmt->setUInt32(0, muteDate);
    stmt->setInt32(1, muteTime);
    stmt->setUInt32(2, accountID);
    LoginDatabase.Execute(stmt);

    auto session = sWorld->FindSession(accountID);
    if (session)
        SetMuteTime(accountID, muteDate);
}
