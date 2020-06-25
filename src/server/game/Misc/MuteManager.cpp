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

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_MUTE_TIME);

    if (targetSession)
    {
        // Target is online, mute will be in effect right away.
        int64 muteTime = GameTime::GetGameTime() + notSpeakTime * MINUTE;
        SetMuteTime(targetSession, muteTime);
        stmt->setInt64(0, muteTime);

        ChatHandler(targetSession).PSendSysMessage(LANG_YOUR_CHAT_DISABLED, notSpeakTime, muteBy.c_str(), muteReason.c_str());
    }
    else
    {
        // Target is offline, mute will be in effect starting from the next login.
        int32 muteTime = -int32(notSpeakTime * MINUTE);
        stmt->setInt64(0, muteTime);
    }

    uint32 accountId = targetSession ? targetSession->GetAccountId() : sObjectMgr->GetPlayerAccountIdByPlayerName(targetName);

    stmt->setString(1, muteReason);
    stmt->setString(2, muteBy);
    stmt->setUInt32(3, accountId);
    LoginDatabase.Execute(stmt);

    stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT_MUTE);
    stmt->setUInt32(0, accountId);
    stmt->setUInt32(1, notSpeakTime);
    stmt->setString(2, muteBy);
    stmt->setString(3, muteReason);
    LoginDatabase.Execute(stmt);

    auto GetPlayerLink = [&]() -> std::string
    {
        return "|cffffffff|Hplayer:" + targetName + "|h[" + targetName + "]|h|r";
    };

    if (CONF_GET_BOOL("ShowMuteInWorld"))
        sWorld->SendWorldText(LANG_COMMAND_MUTEMESSAGE_WORLD, muteBy.c_str(), GetPlayerLink().c_str(), notSpeakTime, muteReason.c_str());
}

void MuteManager::SetMuteTime(WorldSession* sess, uint32 muteTime)
{
    // Check exist
    auto itr = _listSessions.find(sess);
    if (itr != _listSessions.end())
        return;

    _listSessions.insert(std::make_pair(sess, muteTime));
}

uint32 MuteManager::GetMuteTime(WorldSession* sess)
{
    // Check exist option
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
}

void MuteManager::CheckMuteExpired(WorldSession* sess)
{
    uint32 _muteTime = GetMuteTime(sess);
    uint32 timeNow = GameTime::GetGameTime();

    if (!_muteTime || _muteTime > timeNow)
        return;

    DeleteMuteTime(sess);

    // If mute expired, remove it from the DB
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_MUTE_TIME);
    stmt->setInt64(0, 0); // Set the mute time to 0
    stmt->setString(1, "");
    stmt->setString(2, "");
    stmt->setUInt32(3, sess->GetAccountId());
    LoginDatabase.Execute(stmt);
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
