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

#ifndef _GUILD_LEVEL_SYSTEM_H_
#define _GUILD_LEVEL_SYSTEM_H_

#include "Common.h"
#include "GuildMgr.h"
#include "Player.h"
#include "Creature.h"
#include <unordered_map>

uint32 constexpr GLS_GOSSIP_FULL_INVEST = 100000;
uint32 constexpr GLS_GOSSIP_CHOOSE_INVEST = 10000;
uint32 constexpr GLS_ITEMS_COUNT = 3;
uint32 constexpr GLS_ITEMS_REWARD_CHOOSE_COUNT = 3;

struct GuildCriteriaStruct
{
    uint32 CriteriaID;
    uint32 ItemID[GLS_ITEMS_COUNT];
    uint32 ItemCount[GLS_ITEMS_COUNT];
    uint32 MinPlayersCount;
    float Coef;
    uint32 RewardItemID;
    uint32 RewardItemCount;
    uint32 RewardChooseItemID[GLS_ITEMS_REWARD_CHOOSE_COUNT];
    uint32 RewardChooseItemCount[GLS_ITEMS_REWARD_CHOOSE_COUNT];
};

struct GuildCriteriaProgressStruct
{
    uint32 GuildID;
    uint32 CriteriaID;
    uint32 ItemCount[GLS_ITEMS_COUNT];
};

typedef std::unordered_map<uint32 /*criteria id*/, GuildCriteriaStruct> GuildCriteriaBase;

class GuildCriteria
{
public:
    GuildCriteria(uint32 guildID);
    ~GuildCriteria() = default;

    // Progress
    void AddProgress(uint32 criteriaID, GuildCriteriaProgressStruct& _data);
    void AddEmptyProgress(uint32 criteriaID);
    void AddItemProgess(uint32 criteriaID, uint8 itemType, uint32 itemCount);
    uint32 GetItemCountProgress(uint32 criteriaID, uint8 itemType);

    // Base
    void AddBaseCriterias();
    void RescalingCriterias();
    uint32 GetMaxCriteriaItemCount(uint32 criteriaID, uint8 itemType);
    uint32 GetCriteriaItemID(uint32 criteriaID, uint8 itemType);

private:
    std::unordered_map<uint32 /*criteria id*/, GuildCriteriaProgressStruct> _guildCriteriaProgress; // for history
    std::unordered_map<uint32 /*criteria id*/, GuildCriteriaStruct> _guildCriteria;
    uint32 _guildID = 0;
};

class GuildLevelSystem
{
public:
    static GuildLevelSystem* instance();

    void Init();

    // Criteria
    void LoadBaseCriteria();
    void LoadCriteriaProgress();
    void AddEmptyGuildCriteria(uint32 guildID);
    void InvestItem(Player* player, Creature* creature, uint32 action, uint8 sender, uint32 itemCount); // from gossip
    void InvestItemFull(Player* player, Creature* creature, uint32 action, uint8 sender); // from gossip
    GuildCriteria* GetCriteriaProgress(uint32 guildid, bool forceCreate = false);
    uint32 GetMaxCriteriaItemCountBase(uint32 criteriaID, uint8 itemType);
    GuildCriteriaStruct* GetCriteria(uint32 criteriaID);
    const GuildCriteriaBase& GetBaseCriterias() { return _guildCriteriaBase; }
    void RescaleCriterias(uint32 guildID);

    void ShowCritera(Player* player, Creature* creature);
    void ShowInvestedMenu(Player* player, Creature* creature, uint32 action, uint32 sender);

    template<typename Format, typename... Args>
    inline void SendGuildFormat(Guild* guild, Format&& fmt, Args&& ... args)
    {
        SendGuildMessage(guild, warhead::StringFormat(std::forward<Format>(fmt), std::forward<Args>(args)...));
    }

    std::string const GetItemLocale(uint32 ItemID, int8 index_loc = 8);
    std::string const GetItemLink(uint32 itemid, int8 index_loc = 8);

private:
    std::unordered_map<uint32, uint32> _guildExpForLevelStore;
    std::unordered_map<uint32, uint32> _guildSettingStore;
    std::unordered_map<uint32, std::vector<uint32>> _guildSpellRewardStore;
    //std::unordered_map<uint32 /*guild id*/, GuildCriteria*> _guildstore;

    // Criteria
    GuildCriteriaBase _guildCriteriaBase;
    std::unordered_map<uint32 /*guild id*/, GuildCriteria*> _guildCriteriaProgress;

    void SendGuildMessage(Guild* guild, std::string&& message);

    uint32 _maxLevel = 0;
    uint32 _expItemID = 0;
};

#define sGuildLevelSystem GuildLevelSystem::instance()

#endif /* _GUILD_LEVEL_SYSTEM_H_ */
