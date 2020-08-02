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

#include "CFBG.h"
#include "Log.h"
#include "ScriptMgr.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Player.h"
#include "GroupMgr.h"
#include "BattlegroundMgr.h"
#include "Opcodes.h"

class CFBG_BG : public BGScript
{
public:
    CFBG_BG() : BGScript("CFBG_BG") { }

    void OnBattlegroundBeforeAddPlayer(Battleground* bg, Player* player) override
    {
        if (!bg || bg->isArena() || !player)
            return;

        if (!sCFBG->IsEnableSystem())
            return;

        TeamId teamid = player->GetTeamId(true);
        Group* group = player->GetOriginalGroup();
        uint32 PlayerCountInBG = sCFBG->GetAllPlayersCountInBG(bg);

        if (PlayerCountInBG)
            teamid = sCFBG->GetLowerTeamIdInBG(bg);

        if (!group)
            sCFBG->ValidatePlayerForBG(bg, player, teamid);
        else
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
            {
                Player* member = itr->GetSource();
                if (!member)
                    continue;

                if (bg->IsPlayerInBattleground(member->GetGUID()))
                    continue;

                sCFBG->ValidatePlayerForBG(bg, member, teamid);
            }
        }
    }

    void OnBattlegroundAddPlayer(Battleground* bg, Player* player) override
    {
        if (!sCFBG->IsEnableSystem())
            return;

        sCFBG->FitPlayerInTeam(player, true, bg);
    }

    void OnBattlegroundEndReward(Battleground* bg, Player* player, TeamId /*winnerTeamId*/) override
    {
        if (!bg || !player || bg->isArena())
            return;

        if (!sCFBG->IsEnableSystem())
            return;

        if (sCFBG->IsPlayerFake(player))
            sCFBG->ClearFakePlayer(player);
    }

    void OnBattlegroundRemovePlayerAtLeave(Battleground* bg, Player* player) override
    {
        if (!sCFBG->IsEnableSystem())
            return;

        sCFBG->FitPlayerInTeam(player, false, bg);

        if (sCFBG->IsPlayerFake(player))
            sCFBG->ClearFakePlayer(player);
    }

    void OnAddGroup(BattlegroundQueue* queue, GroupQueueInfo* ginfo, uint32& index, Player* /*leader*/, Group* /*grp*/, PvPDifficultyEntry const* /*bracketEntry*/, bool /*isPremade*/) override
    {
        if (!queue)
            return;

        if (sCFBG->IsEnableSystem() && !ginfo->ArenaType && !ginfo->IsRated)
            index = BG_QUEUE_CFBG;
    }

    bool CanFillPlayersToBG(BattlegroundQueue* queue, Battleground* bg, const int32 aliFree, const int32 hordeFree, BattlegroundBracketId bracket_id) override
    {
        if (!sCFBG->IsEnableSystem())
            return true;

        if (!bg->isArena() && sCFBG->FillPlayersToCFBG(queue, bg, aliFree, hordeFree, bracket_id))
            return false;

        return true;
    }

    bool CanFillPlayersToBGWithSpecific(BattlegroundQueue* queue, Battleground* bg, const int32 aliFree, const int32 hordeFree,
        BattlegroundBracketId thisBracketId, BattlegroundQueue* specificQueue, BattlegroundBracketId specificBracketId) override
    {
        if (!sCFBG->IsEnableSystem())
            return true;

        if (!bg->isArena() && sCFBG->FillPlayersToCFBGWithSpecific(queue, bg, aliFree, hordeFree, thisBracketId, specificQueue, specificBracketId))
            return false;

        return true;
    }

    void OnCheckNormalMatch(BattlegroundQueue* /*queue*/, uint32& Coef, Battleground* /*bgTemplate*/, BattlegroundBracketId /*bracket_id*/, uint32& /*minPlayers*/, uint32& /*maxPlayers*/) override
    {
        if (!sCFBG->IsEnableSystem())
            return;

        Coef = 2;
    }

    bool CanSendMessageQueue(BattlegroundQueue* queue, Player* leader, Battleground* bg, PvPDifficultyEntry const* bracketEntry) override
    {
        if (!bg->isArena() && sCFBG->IsEnableSystem() && sCFBG->SendMessageQueue(queue, bg, bracketEntry, leader))
            return false;

        return true;
    }
};

class CFBG_Player : public PlayerScript
{
public:
    CFBG_Player() : PlayerScript("CFBG_Player") { }

    void OnLogin(Player* player) override
    {
        if (!sCFBG->IsEnableSystem())
            return;

        if (player->GetTeamId(true) != player->GetBgTeamId())
            sCFBG->FitPlayerInTeam(player, player->GetBattleground() && !player->GetBattleground()->isArena() ? true : false, player->GetBattleground());
    }

    bool CanJoinInBattlegroundQueue(Player* player, uint64 /*BattlemasterGuid*/, BattlegroundTypeId /*BGTypeID*/, uint8 joinAsGroup, GroupJoinBattlegroundResult& err) override
    {
        if (!sCFBG->IsEnableSystem())
            return true;

        if (joinAsGroup)
        {
            Group* group = player->GetGroup();
            if (!group)
                return true;

            if (group->isRaidGroup() || group->GetMembersCount() > sCFBG->GetMaxPlayersCountInGroup())
                err = ERR_BATTLEGROUND_JOIN_FAILED;

            return false;
        }

        return true;
    }

    void OnBeforeUpdate(Player* player, uint32 diff) override
    {
        if (timeCheck <= diff)
        {
            sCFBG->UpdateForget(player);
            timeCheck = 10000;
        }
        else
            timeCheck -= diff;
    }

    void OnBeforeSendChatMessage(Player* player, uint32& type, uint32& lang, std::string& msg) override
    {
        if (!player || !sCFBG->IsEnableSystem())
            return;

        Battleground* bg = player->GetBattleground();

        if (!bg || bg->isArena())
            return;

        // skip addon lang and universal
        if (lang == LANG_UNIVERSAL || lang == LANG_ADDON)
            return;

        // skip addon and system message
        if (type == CHAT_MSG_ADDON || type == CHAT_MSG_SYSTEM)
            return;

        // to gm lang
        lang = LANG_UNIVERSAL;
    }

private:
    uint32 timeCheck = 10000;
};

class CFBG_World : public WorldScript
{
public:
    CFBG_World() : WorldScript("CFBG_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sCFBG->LoadConfig();
    }
};

// Group all custom scripts
void AddSC_CFBG()
{
    new CFBG_BG();
    new CFBG_Player();
    new CFBG_World();
}
