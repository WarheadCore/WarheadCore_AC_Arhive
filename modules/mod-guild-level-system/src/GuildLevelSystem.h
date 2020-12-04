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

uint32 constexpr GLS_ITEMS_COUNT = 3;
uint32 constexpr GLS_SPELLS_REWARD_COUNT = 3;
uint32 constexpr GLS_GOSSIP_CRITERIA_ID_FULL = 100000;
uint32 constexpr GLS_GOSSIP_CRITERIA_ID = 10000;
uint32 constexpr GLS_GOSSIP_SHOW_CRITERIA_SENDER = GLS_ITEMS_COUNT + 1;
uint32 constexpr GLS_GOSSIP_SHOW_REWARDS_SENDER = GLS_GOSSIP_SHOW_CRITERIA_SENDER + 1;
uint32 constexpr GLS_GOSSIP_GET_REWARDS_SENDER = GLS_GOSSIP_SHOW_REWARDS_SENDER + 1;
uint32 constexpr GLS_GOSSIP_CHOOSE_REWARD_SENDER = GLS_GOSSIP_GET_REWARDS_SENDER + GLS_SPELLS_REWARD_COUNT;
uint32 constexpr GLS_GOSSIP_GET_ALL_REWARDS_SENDER = GLS_GOSSIP_CHOOSE_REWARD_SENDER + GLS_SPELLS_REWARD_COUNT;
uint32 constexpr GLS_GOSSIP_GET_ALL_LINK = 1000;

struct GuildCriteriaStruct
{
    uint32 CriteriaID;
    uint32 StageID;
    uint32 ItemID[GLS_ITEMS_COUNT];
    uint32 ItemCount[GLS_ITEMS_COUNT];
    uint32 MinPlayersCount;
    float Coef;
    uint32 RewardSpells[GLS_SPELLS_REWARD_COUNT];
};

struct GuildCriteriaProgressStruct
{
    uint32 GuildID;
    uint32 CriteriaID;
    uint32 ItemCount[GLS_ITEMS_COUNT];
    uint32 SelectedSpell;
    bool IsDone;
};

typedef std::unordered_map<uint32 /*criteria id*/, GuildCriteriaStruct> GuildCriteriaBase;
typedef std::map<uint32 /*stage id*/, std::vector<uint32> /*criteria id*/> GuildStages; // Need ordered

class GuildCriteria
{
public:
    GuildCriteria(uint32 guildID, uint32 stageID);
    ~GuildCriteria() = default;

    // Progress
    GuildCriteriaProgressStruct* GetProgress(uint32 criteriaID, bool forceCreate = false);
    GuildCriteriaStruct* GetCriteria(uint32 criteriaID);
    void AddProgress(uint32 criteriaID, GuildCriteriaProgressStruct& _data);
    void AddEmptyProgress(uint32 criteriaID);
    void AddItemProgess(uint32 criteriaID, uint8 itemType, uint32 itemCount);
    uint32 GetItemCountProgress(uint32 criteriaID, uint8 itemType);
    uint32 GetCountProgressDone(uint32 criteriaID);
    uint32 GetMaxCountProgressDone(uint32 criteriaID);
    void SetSelectedSpell(uint32 criteriaID, uint32 spellID);

    // Base
    void AddBaseCriterias();
    void RescalingCriterias();
    uint32 GetMaxCriteriaItemCount(uint32 criteriaID, uint8 itemType);
    uint32 GetCriteriaItemID(uint32 criteriaID, uint8 itemType);
    uint32 GetRewardSpellID(uint32 criteriaID, uint8 spellType);

    void SaveToDB(uint32 criteriaID);
    void SetProgressDone(uint32 criteriaID, bool isDone = true);
    bool IsProgressDone(uint32 criteriaID);
    uint32 GetCountCriteriaProgressDone();
    void UnLearnSpells(uint32 lowGuid);
    void LearnSpells(Player* player);

    // Stage
    uint32 GetStageID() { return _stageID; }
    void SetStageID(uint32 stage, bool saveDB = true);

private:
    std::unordered_map<uint32 /*criteria id*/, GuildCriteriaProgressStruct> _guildCriteriaProgress; // for history
    std::unordered_map<uint32 /*criteria id*/, GuildCriteriaStruct> _guildCriteria;
    uint32 _guildID = 0;
    uint32 _stageID = 1;
};

class GuildLevelSystem
{
public:
    static GuildLevelSystem* instance();

    void Init();

    // Criteria
    void LoadBaseCriteria();
    void LoadCriteriaProgress();
    void LoadStageProgress();
    void AddEmptyGuildCriteria(uint32 guildID);
    void InvestItem(Player* player, Creature* creature, uint32 sender, uint32 action, uint32 itemCount); // from gossip
    void InvestItemFull(Player* player, Creature* creature, uint32 sender, uint32 action); // from gossip
    GuildCriteria* GetCriteriaProgress(uint32 guildid, bool forceCreate = false);
    uint32 GetMaxCriteriaItemCountBase(uint32 criteriaID, uint8 itemType);
    GuildCriteriaStruct* GetCriteriaBase(uint32 criteriaID);
    const GuildCriteriaBase& GetBaseCriterias() { return _guildCriteriaBase; }
    void RescaleCriterias(uint32 guildID);

    // Stages
    uint32 GetCountCriteriaInStage(uint32 stageID);
    const GuildStages& GetStages() { return _guildStages; }
    uint32 GetNextStage(uint32 currStage);

    // Gossip criteria
    void ShowAllCriteriaInfo(Player* player, Creature* creature);
    void ShowCriteriaInfo(Player* player, Creature* creature, uint32 sender, uint32 action);
    void ShowInvestedMenu(Player* player, Creature* creature, uint32 sender, uint32 action);
    void ShowRewardInfo(Player* player, Creature* creature, uint32 sender, uint32 action);
    void GetRewardsCriteria(Player* player, Creature* creature, uint32 sender, uint32 action);
    void GetAllLink(Player* player, Creature* creature, uint32 sender, uint32 action);
    void SetNextStage(Player* player);

    // Spells
    void UnLearnSpellsForPlayer(uint32 lowGuid, uint32 guildID);
    void LearnSpellsForPlayer(Player* player, uint32 guildID);

    template<typename Format, typename... Args>
    inline void SendGuildFormat(uint32 guildID, Format&& fmt, Args&& ... args)
    {
        SendGuildMessage(guildID, Warhead::StringFormat(std::forward<Format>(fmt), std::forward<Args>(args)...));
    }

private:
    // Criteria
    GuildCriteriaBase _guildCriteriaBase;
    GuildStages _guildStages;

    std::unordered_map<uint32 /*guild id*/, GuildCriteria*> _guildCriteriaProgress;

    void SendGuildMessage(uint32 guildID, std::string&& message);
};

#define sGuildLevelSystem GuildLevelSystem::instance()

#endif /* _GUILD_LEVEL_SYSTEM_H_ */
