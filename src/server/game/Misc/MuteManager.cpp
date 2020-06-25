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

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT_MUTE);
    stmt->setUInt32(0, accountId);

    uint32 muteDate = GameTime::GetGameTime() + notSpeakTime * MINUTE;

    /*
     * Mute will be in effect right away.
     * If Mute.AddAfterLogin.Enable mute will be in effect starting from the next login.
     */
    int32 muteTime = CONF_GET_BOOL("Mute.AddAfterLogin.Enable") ? -int32(notSpeakTime * MINUTE) : notSpeakTime;

    if (targetSession)
        AddMuteTime(targetSession, muteDate);
    
    stmt->setUInt32(1, muteDate);
    stmt->setInt64(2, muteTime);
    stmt->setString(2, muteBy);
    stmt->setString(3, muteReason);
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
    auto targetSession = sWorld->FindSession(accID);

    if (targetSession)
        DeleteMuteTime(targetSession);

    if (targetSession)
        ChatHandler(targetSession).PSendSysMessage(LANG_YOUR_CHAT_ENABLED);   
}

void MuteManager::AddMuteTime(WorldSession* sess, uint32 muteTime)
{
    // Check exist
    auto itr = _listSessions.find(sess);
    if (itr != _listSessions.end())
        return;

    _listSessions.insert(std::make_pair(sess, muteTime));
}

void MuteManager::SetMuteTime(WorldSession* sess, uint32 muteTime)
{
    // Check empty
    auto itr = _listSessions.find(sess);
    if (itr == _listSessions.end())
        return;

    _listSessions[sess] = muteTime;
}

uint32 MuteManager::GetMuteTime(WorldSession* sess)
{
    // Check exist
    auto itr = _listSessions.find(sess);
    if (itr == _listSessions.end())
        return 0;

    return _listSessions[sess];
}

void MuteManager::DeleteMuteTime(WorldSession* sess)
{
    // Check exist option
    auto itr = _listSessions.find(sess);
    if (itr == _listSessions.end())
        return;

    _listSessions.erase(sess);

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_MUTE_EXPIRED);
    stmt->setUInt32(0, sess->GetAccountId());
    LoginDatabase.Execute(stmt);
}

void MuteManager::CheckMuteExpired(WorldSession* sess)
{
    uint32 _muteTime = GetMuteTime(sess);
    uint32 timeNow = GameTime::GetGameTime();

    if (!_muteTime || _muteTime > timeNow)
        return;

    DeleteMuteTime(sess);
}

std::string const MuteManager::GetMuteTimeString(WorldSession* sess)
{
    uint32 _muteTime = GetMuteTime(sess);

    if (!_muteTime)
        return "";

    return secsToTimeString(_muteTime - GameTime::GetGameTime());
}

bool MuteManager::CanSpeak(WorldSession* sess)
{
    return GetMuteTime(sess) <= GameTime::GetGameTime();
}

void MuteManager::LoginAccount(uint32 accountID)
{
    // Set inactive if expired
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_MUTE_EXPIRED);
    stmt->setUInt32(0, accountID);
    LoginDatabase.Execute(stmt);

    // Get info about mute time after update active
    stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_MUTE);
    stmt->setUInt32(0, accountID);
    
    PreparedQueryResult result = LoginDatabase.Query(stmt);
    if (!result)
        return; // If no info - no mute time:)

    Field* fields = result->Fetch();
    int32 mutetime = fields[1].GetInt32();

    //! Negative mutetime indicates amount of seconds to be muted effective on next login - which is now.
    if (mutetime < 0)
        UpdateMuteAccount(accountID, GameTime::GetGameTime() + std::abs(mutetime), std::abs(mutetime));
}

void MuteManager::UpdateMuteAccount(uint32 accountID, uint32 muteDate, int32 muteTime)
{
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_MUTE);
    stmt->setUInt32(0, muteDate);
    stmt->setInt32(1, muteTime);
    stmt->setUInt32(2, accountID);
    LoginDatabase.Execute(stmt);

    auto session = sWorld->FindSession(accountID);
    if (session)
        AddMuteTime(session, muteDate);
}
