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

#include "crossfaction_instance.h"
#include "GameConfig.h"
#include "Group.h"
#include "ObjectAccessor.h"

enum FakeMorphs
{
    FAKE_F_TAUREN   = 20584,
    FAKE_M_TAUREN   = 20585,
    FAKE_M_NELF     = 20318,
    FAKE_F_DRAENEI  = 20323
};

/*static*/ CrossFactionInstanceICC* CrossFactionInstanceICC::instance()
{
    static CrossFactionInstanceICC instance;
    return &instance;
}

void CrossFactionInstanceICC::SetGroupFakeFaction(uint32 instanceID, Player* player, TeamId teamid)
{
    if (!CONF_GET_BOOL("AllowTwoSide.Interaction.Group"))
        return;

    if (!player)
        return;

    if (player->GetTeamId(true) == teamid)
        return;

    auto group = player->GetGroup();
    if (!group)
        return;

    SetFakeRaceAndMorph(instanceID, player, teamid);
}

void CrossFactionInstanceICC::SetNormalGroup(uint32 instanceID)
{
    if (!CONF_GET_BOOL("AllowTwoSide.Interaction.Group"))
        return;

    for (auto const& itr : _fakePlayers)
    {
        if (itr.second.InstanceID != instanceID)
            continue;

        Player* player = sObjectAccessor->FindPlayer(itr.second.PlayerGuid);
        if (!player)
            continue;

        ClearFakePlayer(player);
    }    
}

// Functions from CFBG module
void CrossFactionInstanceICC::SetFakeRaceAndMorph(uint32 instanceID, Player* player, TeamId teamid)
{
    if (GetFakePlayer(player))
        return;

    uint8 FakeRace;
    uint32 FakeMorph;

    if (player->getClass() == CLASS_DRUID)
    {
        if (player->GetTeamId(true) == TEAM_ALLIANCE)
        {
            FakeMorph = player->getGender() == GENDER_MALE ? FAKE_M_TAUREN : FAKE_F_TAUREN;
            FakeRace = RACE_TAUREN;
        }
        else if (player->getGender() == GENDER_MALE) // HORDE PLAYER, ONLY HAVE MALE Night Elf ID
        {
            FakeMorph = FAKE_M_NELF;
            FakeRace = RACE_NIGHTELF;
        }
        else
            FakeRace = player->GetTeamId(true) == TEAM_ALLIANCE ? RACE_BLOODELF : RACE_HUMAN;

        if (player->GetTeamId(true) == TEAM_HORDE)
        {
            if (player->getGender() == GENDER_MALE)
                FakeMorph = 19723;
            else
                FakeMorph = 19724;
        }
        else
        {
            if (player->getGender() == GENDER_MALE)
                FakeMorph = 20578;
            else
                FakeMorph = 20579;
        }
    }
    else if (player->getClass() == CLASS_SHAMAN && player->GetTeamId(true) == TEAM_HORDE && player->getGender() == GENDER_FEMALE)
    {
        FakeMorph = FAKE_F_DRAENEI; // Female Draenei
        FakeRace = RACE_DRAENEI;
    }
    else
    {
        FakeRace = player->GetTeamId(true) == TEAM_ALLIANCE ? RACE_BLOODELF : RACE_HUMAN;

        if (player->GetTeamId(true) == TEAM_HORDE)
        {
            if (player->getGender() == GENDER_MALE)
                FakeMorph = 19723;
            else
                FakeMorph = 19724;
        }
        else
        {
            if (player->getGender() == GENDER_MALE)
                FakeMorph = 20578;
            else
                FakeMorph = 20579;
        }
    }

    FakePlayer fakePlayer;
    fakePlayer.FakeMorph = FakeMorph;
    fakePlayer.FakeRace = FakeRace;
    fakePlayer.FakeTeamID = player->TeamIdForRace(FakeRace);
    fakePlayer.RealMorph = player->GetDisplayId();
    fakePlayer.RealRace = player->getRace(true);
    fakePlayer.RealTeamID = player->GetTeamId(true);
    fakePlayer.InstanceID = instanceID;

    AddFakePlayer(player, fakePlayer);

    player->setRace(FakeRace);
    SetFactionForRace(player, FakeRace);
    player->SetDisplayId(FakeMorph);
    player->SetNativeDisplayId(FakeMorph);
}

void CrossFactionInstanceICC::SetFactionForRace(Player* player, uint8 race)
{
    player->setTeamId(player->TeamIdForRace(race));

    ChrRacesEntry const* DBCRace = sChrRacesStore.LookupEntry(race);
    player->setFaction(DBCRace ? DBCRace->FactionID : 0);
}

void CrossFactionInstanceICC::ClearFakePlayer(Player* player)
{
    auto fakePlayer = GetFakePlayer(player);
    if (!fakePlayer)
        return;

    player->setRace(fakePlayer->RealRace);
    player->SetDisplayId(fakePlayer->RealMorph);
    player->SetNativeDisplayId(fakePlayer->RealMorph);
    SetFactionForRace(player, fakePlayer->RealRace);

    DeleteFakePlayer(player);
}

CrossFactionInstanceICC::FakePlayer const* CrossFactionInstanceICC::GetFakePlayer(Player* player)
{
    auto const& itr = _fakePlayers.find(player);
    if (itr != _fakePlayers.end())
        return &itr->second;

    return nullptr;
}

void CrossFactionInstanceICC::AddFakePlayer(Player* player, CrossFactionInstanceICC::FakePlayer& fakePlayer)
{
    if (GetFakePlayer(player))
        return;

    _fakePlayers.insert(std::make_pair(player, fakePlayer));
}

void CrossFactionInstanceICC::DeleteFakePlayer(Player* player)
{
    if (!GetFakePlayer(player))
        return;

    _fakePlayers.erase(player);
}
