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

#include "ScriptMgr.h"
#include "Log.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ArenaTeamMgr.h"
#include "BattlegroundMgr.h"
#include "DisableMgr.h"
#include "Language.h"

class npc_1v1arena : public CreatureScript
{
public:
    npc_1v1arena() : CreatureScript("npc_1v1arena") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (!player || !creature)
            return true;

        if (!CONF_GET_BOOL("Arena1v1.Enable"))
        {
            ChatHandler(player->GetSession()).SendSysMessage("Module has disabled!");
            return true;
        }

        if (player->InBattlegroundQueueForBattlegroundQueueType(BATTLEGROUND_QUEUE_5v5))
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Queue leave 1v1 Arena", GOSSIP_SENDER_MAIN, 3, "Are you sure?", 0, false);
        else
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Queue enter 1v1 Arena(UnRated)", GOSSIP_SENDER_MAIN, 20);

        if (!player->GetArenaTeamId(ArenaTeam::GetSlotByType(ARENA_TEAM_5v5)))
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Create new 1v1 Arena Team", GOSSIP_SENDER_MAIN, 1, "Are you sure?", CONF_GET_INT("Arena1v1Costs"), false);
        else
        {
            if (!player->InBattlegroundQueueForBattlegroundQueueType(BATTLEGROUND_QUEUE_5v5))
            {
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Queue enter 1v1 Arena (Rated)", GOSSIP_SENDER_MAIN, 2);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Arenateam Clear", GOSSIP_SENDER_MAIN, 5, "Are you sure?", 0, false);
            }

            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Shows your statistics", GOSSIP_SENDER_MAIN, 4);
        }

        SendGossipMenuFor(player, 68, creature);
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        if (!player || !creature)
            return true;

        ClearGossipMenuFor(player);

        ChatHandler handler(player->GetSession());

        switch (action)
        {
        case 1: // Create new Arenateam
        {
            if (CONF_GET_INT("Arena1v1MinLevel") <= player->getLevel())
            {
                if (player->GetMoney() >= CONF_GET_INT("Arena1v1Costs") && CreateArenateam(player, creature))
                    player->ModifyMoney(-CONF_GET_INT("Arena1v1Costs"));
            }
            else
            {
                handler.PSendSysMessage("You have to be level %u + to create a 1v1 arena team.", CONF_GET_INT("Arena1v1MinLevel"));
                CloseGossipMenuFor(player);
                return true;
            }
        }
        break;

        case 2: // Join Queue Arena (rated)
        {
            if (Arena1v1CheckTalents(player) && !JoinQueueArena(player, creature, true))
                handler.SendSysMessage("Something went wrong when joining the queue.");

            CloseGossipMenuFor(player);
            return true;
        }
        break;

        case 20: // Join Queue Arena (unrated)
        {
            if (Arena1v1CheckTalents(player) && !JoinQueueArena(player, creature, false))
                handler.SendSysMessage("Something went wrong when joining the queue.");

            CloseGossipMenuFor(player);
            return true;
        }
        break;

        case 3: // Leave Queue
        {
            uint8 arenaType = ARENA_TYPE_5v5;

            if (!player->InBattlegroundQueueForBattlegroundQueueType(BATTLEGROUND_QUEUE_5v5))
                return true;

            WorldPacket data;
            data << arenaType << (uint8)0x0 << (uint32)BATTLEGROUND_AA << (uint16)0x0 << (uint8)0x0;
            player->GetSession()->HandleBattleFieldPortOpcode(data);
            CloseGossipMenuFor(player);
            return true;
        }
        break;

        case 4: // get statistics
        {
            ArenaTeam* at = sArenaTeamMgr->GetArenaTeamById(player->GetArenaTeamId(ArenaTeam::GetSlotByType(ARENA_TEAM_5v5)));
            if (at)
            {
                std::stringstream s;
                s << "Rating: " << at->GetStats().Rating;
                s << "\nRank: " << at->GetStats().Rank;
                s << "\nSeason Games: " << at->GetStats().SeasonGames;
                s << "\nSeason Wins: " << at->GetStats().SeasonWins;
                s << "\nWeek Games: " << at->GetStats().WeekGames;
                s << "\nWeek Wins: " << at->GetStats().WeekWins;

                ChatHandler(player->GetSession()).PSendSysMessage(SERVER_MSG_STRING, s.str().c_str());
            }
        }
        break;

        case 5: // Disband arenateam
        {
            WorldPacket Data;
            Data << (uint32)player->GetArenaTeamId(ArenaTeam::GetSlotByType(ARENA_TEAM_5v5));
            player->GetSession()->HandleArenaTeamLeaveOpcode(Data);
            handler.SendSysMessage("Arenateam deleted!");
            CloseGossipMenuFor(player);
            return true;
        }
        break;
        }

        OnGossipHello(player, creature);
        return true;
    }

private:
    bool JoinQueueArena(Player* player, Creature* me, bool isRated)
    {
        if (!player || !me)
            return false;

        if (CONF_GET_INT("Arena1v1MinLevel") > player->getLevel())
            return false;

        uint8 arenaslot = ArenaTeam::GetSlotByType(ARENA_TEAM_5v5);
        uint8 arenatype = ARENA_TYPE_5v5;
        uint32 arenaRating = 0;
        uint32 matchmakerRating = 0;

        // ignore if we already in BG or BG queue
        if (player->InBattleground())
            return false;

        //check existance
        Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
        if (!bg)
        {
            sLog->outString("Battleground: template bg (all arenas) not found");
            return false;
        }

        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, nullptr))
        {
            ChatHandler(player->GetSession()).PSendSysMessage(LANG_ARENA_DISABLED);
            return false;
        }

        BattlegroundTypeId bgTypeId = bg->GetBgTypeID();
        BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, arenatype);
        PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
        if (!bracketEntry)
            return false;

        // check if already in queue
        if (player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
            return false; // //player is already in this queue

        // check if has free queue slots
        if (!player->HasFreeBattlegroundQueueId())
            return false;

        uint32 ateamId = 0;

        if (isRated)
        {
            ateamId = player->GetArenaTeamId(arenaslot);
            ArenaTeam* at = sArenaTeamMgr->GetArenaTeamById(ateamId);
            if (!at)
            {
                player->GetSession()->SendNotInArenaTeamPacket(arenatype);
                return false;
            }

            // get the team rating for queueing
            arenaRating = at->GetRating();
            matchmakerRating = arenaRating;
            // the arenateam id must match for everyone in the group

            if (arenaRating <= 0)
                arenaRating = 1;
        }

        BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
        bg->SetRated(isRated);

        GroupQueueInfo* ginfo = bgQueue.AddGroup(player, nullptr, bracketEntry, isRated, false, arenaRating, matchmakerRating, ateamId);
        uint32 avgTime = bgQueue.GetAverageQueueWaitTime(ginfo);
        uint32 queueSlot = player->AddBattlegroundQueueId(bgQueueTypeId);

        // send status packet (in queue)
        WorldPacket data;
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, queueSlot, STATUS_WAIT_QUEUE, avgTime, 0, arenatype, TEAM_NEUTRAL, isRated);
        player->GetSession()->SendPacket(&data);

        sBattlegroundMgr->ScheduleArenaQueueUpdate(matchmakerRating, bgQueueTypeId, bracketEntry->GetBracketId());

        return true;
    }

    bool CreateArenateam(Player* player, Creature* me)
    {
        if (!player || !me)
            return false;

        uint8 slot = ArenaTeam::GetSlotByType(ARENA_TEAM_5v5);
        if (slot >= MAX_ARENA_SLOT)
            return false;

        // Check if player is already in an arena team
        if (player->GetArenaTeamId(slot))
        {
            player->GetSession()->SendArenaTeamCommandResult(ERR_ARENA_TEAM_CREATE_S, player->GetName(), "You are already in an arena team!", ERR_ALREADY_IN_ARENA_TEAM);
            return false;
        }

        // Teamname = playername
        // if teamname exist, we have to choose another name (playername + number)
        int i = 1;
        std::stringstream teamName;
        teamName << player->GetName();

        do
        {
            if (sArenaTeamMgr->GetArenaTeamByName(teamName.str())) // teamname exist, so choose another name
            {
                teamName.str(std::string());
                teamName << player->GetName() << (i++);
            }
            else
                break;
        } while (i < 100); // should never happen

        // Create arena team
        ArenaTeam* arenaTeam = new ArenaTeam();
        if (!arenaTeam->Create(player->GetGUID(), ARENA_TEAM_5v5, teamName.str(), 4283124816, 45, 4294242303, 5, 4294705149))
        {
            delete arenaTeam;
            return false;
        }

        // Register arena team
        sArenaTeamMgr->AddArenaTeam(arenaTeam);

        ChatHandler(player->GetSession()).SendSysMessage("1v1 Arenateam successfully created!");

        return true;
    }

    bool Arena1v1CheckTalents(Player* player)
    {
        if (!player)
            return false;

        if (!CONF_GET_BOOL("Arena1v1BlockForbiddenTalents"))
            return true;

        /*
            @TODO need rework
        */

        //uint32 count = 0;

        for (auto itr : sTalentStore)
        {
            for (int8 rank = MAX_TALENT_RANK - 1; rank >= 0; --rank)
                if (itr->RankID[rank] == 0)
                    continue;
        }        

        /*if (count >= 36)
        {
            ChatHandler(player->GetSession()).SendSysMessage("You can not join because you have too many talent points in a forbidden tree. (Heal / Tank)");
            return false;
        }*/

        return true;
    }
};

class Arena1v1_World : public WorldScript
{
public:
    Arena1v1_World() : WorldScript("Arena1v1_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        // Add bool
        sGameConfig->AddBoolConfig("Arena1v1.Enable");
        sGameConfig->AddBoolConfig("Arena1v1.BlockForbiddenTalents");

        // Add int
        sGameConfig->AddIntConfig("Arena1v1.MinLevel", 80);
        sGameConfig->AddIntConfig("Arena1v1.Costs", 400000);
    }
};

// Group all custom scripts
void AddSC_1v1Arena()
{
    new Arena1v1_World();
    new npc_1v1arena();
}
