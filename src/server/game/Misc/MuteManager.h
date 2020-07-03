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

#ifndef _MUTE_MANAGER_H_
#define _MUTE_MANAGER_H_

#include "Common.h"
#include <unordered_map>

class MuteManager
{
public:

    static MuteManager* instance();

    void MutePlayer(std::string const targetName, uint32 notSpeakTime, std::string const muteBy, std::string const muteReason);
    void UnMutePlayer(std::string const targetName);
    void UpdateMuteAccount(uint32 accountID, uint32 muteDate, int32 muteTime);
    void SetMuteTime(uint32 accountID, uint32 muteTime);
    void AddMuteTime(uint32 accountID, uint32 muteTime);
    uint32 GetMuteTime(uint32 accountID);
    std::string const GetMuteTimeString(uint32 accountID);
    void DeleteMuteTime(uint32 accountID, bool delFromDB = true);

    void CheckMuteExpired(uint32 accountID);
    bool CanSpeak(uint32 accountID);
    void LoginAccount(uint32 accountID);

private:
    std::unordered_map<uint32, uint32> _listSessions;
};

#define sMute MuteManager::instance()

#endif // _MUTE_MANAGER_H_
