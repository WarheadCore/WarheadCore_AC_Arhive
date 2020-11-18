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

#ifndef _CFIICC_H_
#define _CFIICC_H_

#include "Common.h"
#include "Player.h"

class CrossFactionInstanceICC
{
public:
    static CrossFactionInstanceICC* instance();

    void SetGroupFakeFaction(uint32 instanceID, Player* player, TeamId teamid);
    void SetNormalGroup(uint32 instanceID);

private:
    struct FakePlayer
    {
        // Fake
        uint8   FakeRace;
        uint32  FakeMorph;
        TeamId  FakeTeamID;

        // Real
        uint8   RealRace;
        uint32  RealMorph;
        TeamId  RealTeamID;

        //
        uint32 InstanceID;
        uint64 PlayerGuid;
    };

    std::unordered_map<Player*, FakePlayer> _fakePlayers;

    // Functions from CFBG module
    void SetFakeRaceAndMorph(uint32 instanceID, Player* player, TeamId teamid);
    void SetFactionForRace(Player* player, uint8 race);
    void ClearFakePlayer(Player* player);
    FakePlayer const* GetFakePlayer(Player* player);
    void AddFakePlayer(Player* player, FakePlayer& fakePlayer);
    void DeleteFakePlayer(Player* player);
};

#define CFI CrossFactionInstanceICC::instance()

#endif /* _CFIICC_H_ */
