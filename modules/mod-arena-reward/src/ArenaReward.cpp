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

#include "ArenaReward.h"
#include "Log.h"
#include "GameConfig.h"
#include "ArenaTeamMgr.h"
#include "ObjectAccessor.h"

/*static*/ ArenaReward* ArenaReward::instance()
{
    static ArenaReward instance;
    return &instance;
}

void ArenaReward::SendRewardArena(Battleground* bg, TeamId winnerTeamId)
{
    uint32 rewardItemID = 0;
    uint32 rewardItemCountWinner = 0;
    uint32 rewardItemCountLoser = 0;

    bool isRated = bg->isRated();

    auto SendItems = [&](uint32 itemID, uint32 itemCountWinner, uint32 itemCountLoser)
    {
        if (isRated)
        {
            auto winnerArenaTeam = sArenaTeamMgr->GetArenaTeamById(bg->GetArenaTeamIdForTeam(winnerTeamId == TEAM_NEUTRAL ? TEAM_HORDE : winnerTeamId));
            auto loserArenaTeam = sArenaTeamMgr->GetArenaTeamById(bg->GetArenaTeamIdForTeam(winnerTeamId == TEAM_NEUTRAL ? TEAM_ALLIANCE : bg->GetOtherTeamId(winnerTeamId)));

            for (auto const& member : winnerArenaTeam->GetMembers())
            {
                Player* player = sObjectAccessor->FindPlayer(member.Guid);
                if (!player)
                    continue;

                player->AddItem(itemID, itemCountWinner);
            }

            for (auto const& member : loserArenaTeam->GetMembers())
            {
                Player* player = sObjectAccessor->FindPlayer(member.Guid);
                if (!player)
                    continue;

                player->AddItem(itemID, itemCountLoser);
            }
        }
        else // !isRated
        {
            for (auto const& playerMap : bg->GetPlayers())
            {
                Player* player = playerMap.second;
                if (!player)
                    continue;

                if (player->GetBgTeamId() == winnerTeamId)
                    player->AddItem(itemID, itemCountWinner);
                else
                    player->AddItem(itemID, itemCountLoser);
            }
        }        
    };

    if (isRated)
    {
        rewardItemID = CONF_GET_INT("ArenaReward.Reward.Rating.ItemID");
        rewardItemCountWinner = CONF_GET_INT("ArenaReward.Reward.Rating.ItemCount.WinnerTeam");
        rewardItemCountLoser = CONF_GET_INT("ArenaReward.Reward.Rating.ItemCount.LoserTeam");
    }
    else // !isRated
    {
        rewardItemID = CONF_GET_INT("ArenaReward.Reward.Skirmish.ItemID");
        rewardItemCountWinner = CONF_GET_INT("ArenaReward.Reward.Skirmish.ItemCount.WinnerTeam");
        rewardItemCountLoser = CONF_GET_INT("ArenaReward.Reward.Skirmish.ItemCount.LoserTeam");
    }

    SendItems(rewardItemID, rewardItemCountWinner, rewardItemCountLoser);
}
