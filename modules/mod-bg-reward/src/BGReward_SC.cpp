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

#include "Log.h"
#include "ScriptMgr.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Player.h"
#include "Battleground.h"

class BGReward_Player : public BGScript
{
public:
    BGReward_Player() : BGScript("BGReward_Player") { }

    void OnBattlegroundEndReward(Battleground* bg, Player* player, TeamId winnerTeamId) override
    {
        if (!CONF_GET_BOOL("BGReward.Enable"))
            return;

        // Not reward on end arena
        if (bg->isArena())
            return;

        TeamId bgTeamId = player->GetBgTeamId();
        uint32 RewardCount = 0;

        bgTeamId == winnerTeamId ? RewardCount = CONF_GET_INT("BGReward.WinnerTeam.Count") : RewardCount = CONF_GET_INT("BGReward.LoserTeam.Count");

        switch (player->GetZoneId())
        {
            case 3277: // Warsong Gulch
                player->AddItem(CONF_GET_INT("BGReward.ItemID.WSG"), RewardCount);
                break;
            case 3358: // Arathi Basin
                player->AddItem(CONF_GET_INT("BGReward.ItemID.Arathi"), RewardCount);
                break;
            case 3820: // Eye of the Storm
                player->AddItem(CONF_GET_INT("BGReward.ItemID.Eye"), RewardCount);
                break;
            case 4710: // Isle of Conquest
                player->AddItem(CONF_GET_INT("BGReward.ItemID.Isle"), RewardCount);
                break;
            case 4384: // Strand of the Ancients
                player->AddItem(CONF_GET_INT("BGReward.ItemID.Ancients"), RewardCount);
                break;
            case 2597: // Alterac Valley
                player->AddItem(CONF_GET_INT("BGReward.ItemID.Alterac"), RewardCount);
                break;
            default:
                break;
        }
    }
};

class BGReward_World : public WorldScript
{
public:
    BGReward_World() : WorldScript("BGReward_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddOption<bool>("BGReward.Enable");
        sGameConfig->AddOption<int32>("BGReward.ItemID.WSG", ITEM_WS_MARK_OF_HONOR);
        sGameConfig->AddOption<int32>("BGReward.ItemID.Arathi", ITEM_AB_MARK_OF_HONOR);
        sGameConfig->AddOption<int32>("BGReward.ItemID.Alterac", ITEM_AV_MARK_OF_HONOR);
        sGameConfig->AddOption<int32>("BGReward.ItemID.Isle", ITEM_IC_MARK_OF_HONOR);
        sGameConfig->AddOption<int32>("BGReward.ItemID.Ancients", ITEM_SA_MARK_OF_HONOR);
        sGameConfig->AddOption<int32>("BGReward.ItemID.Eye", ITEM_EY_MARK_OF_HONOR);
        sGameConfig->AddOption<int32>("BGReward.WinnerTeam.Count", 3);
        sGameConfig->AddOption<int32>("BGReward.LoserTeam.Count", 1);
    }
};

// Group all custom scripts
void AddSC_BGReward()
{
    new BGReward_Player();
    new BGReward_World();
}
