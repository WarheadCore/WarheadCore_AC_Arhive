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

struct GuildCriteriaStruct
{
    uint32 CriteriaID;
    uint32 ItemID1;
    uint32 ItemCount1;
    uint32 ItemID2;
    uint32 ItemCount2;
    uint32 ItemID3;
    uint32 ItemCount3;
    float Coef;
    uint32 RewardItemID;
    uint32 RewardItemCount;
    uint32 RewardChooseItemID1;
    uint32 RewardChooseItemCount1;
    uint32 RewardChooseItemID2;
    uint32 RewardChooseItemCount2;
    uint32 RewardChooseItemID3;
    uint32 RewardChooseItemCount3;
};

struct GuildCriteriaProgressStruct
{
    uint32 GuildID;
    uint32 CriteriaID;
    uint32 ItemCount1;
    uint32 ItemCount2;
    uint32 ItemCount3;
};

enum GuildSettingFlag
{
    GUILD_SETTING_FLAG_NONE            = 0,
    GUILD_SETTING_FLAG_ADM_CHAT        = 1,
    GUILD_SETTING_FLAG_ASSISTANT_CHAT  = 2
};

class GuildCriteria
{
public:
    GuildCriteria(uint32 guildID) : _guildID(guildID) { }
    ~GuildCriteria() = default;

    void AddProgress(uint32 criteriaID, GuildCriteriaProgressStruct& _data);
    void AddEmptyProgress(uint32 criteriaID);
    void AddItemProgess(uint32 criteriaID, uint8 itemType, uint32 itemCount);
    uint32 GetItemCountProgress(uint32 criteriaID, uint8 itemType);

private:
    std::unordered_map<uint32 /*criteria id*/, GuildCriteriaProgressStruct> _guildCriteriaProgress; // for history
    uint32 _guildID = 0;
};

class GuildLevelSystem
{
public:
    static GuildLevelSystem* instance();

    void Init();

    //void LoadExpGuildForLevel();
    //void LoadGuildSetting();
    //void LoadGuildSpellReward();
    //void LoadGuildLevels();
    //void SetGuildLevels();

    // Criteria
    void LoadCriteria();
    void LoadCriteriaProgress();
    void AddEmptyGuildCriteria(uint32 guildID);
    void InvestItem(Player* player, Creature* creature, uint32 action, uint8 sender, uint32 itemCount); // from gossip
    void InvestItemFull(Player* player, Creature* creature, uint32 action, uint8 sender); // from gossip
    uint32 GetCriteriaItemID(uint32 criteriaID, uint8 itemType);
    uint32 GetMaxCriteriaItemCount(uint32 criteriaID, uint8 itemType);
    GuildCriteria* GetCriteriaProgress(uint32 guildid);
    GuildCriteriaStruct* GetCriteria(uint32 criteriaID);

    void ShowCritera(Player* player, Creature* creature);
    void ShowInvestedMenu(Player* player, Creature* creature, uint32 action, uint32 sender);

    //void SetFullName(uint32 guildID);
    //void SetFullNameFirstLevel(Guild* guild);

    // Config
    //uint32 GetMaxLevel() { return _maxLevel; }
    //uint32 GetExpItemID() { return _expItemID; }

    //uint32 GetExpForNextLevel(uint32 level);
    //void UpdateGuildVisibleLevel(uint32 guildID);
    //void UpdateRewardForNewLevel(uint32 guildID);
    //void AddGuildExp(Guild* guild, uint32 exp);
    //void GuildInfo(Player* player);
    //void InvestExpFull(Player* player);

    //uint32 GetCountExpItem(Player* player);
    //bool IsAsseptSetting(GuildSettingFlag Flag, Guild* guild);

    //std::string IsAdmChat(Guild* guild);
    //std::string IsAssistantChat(Guild* guild);

    //void ChangeGuildSetting(GuildSettingFlag Flag, Guild* guild, bool Apply);
    //void ChangeSettingChat(Guild* guild, bool IsAdmChat);
    /*void RewardSpellGuildMember(Guild* guild, Player* player);
    void UnRewardSpellGuildMember(Guild* guild, Player* player);*/
    
    /*void AddGLS(Guild* guild, uint32 level, uint32 exp);
    void AddEmptyGLS(Guild* guild);
    void DeleteGLS(Guild* guild);*/

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
    std::unordered_map<uint32 /*criteria id*/, GuildCriteriaStruct> _guildCriteria;
    std::unordered_map<uint32 /*guild id*/, GuildCriteria*> _guildCriteriaProgress;

    void SendGuildMessage(Guild* guild, std::string&& message);

    uint32 _maxLevel = 0;
    uint32 _expItemID = 0;
};

#define sGuildLevelSystem GuildLevelSystem::instance()

#endif /* _GUILD_LEVEL_SYSTEM_H_ */
