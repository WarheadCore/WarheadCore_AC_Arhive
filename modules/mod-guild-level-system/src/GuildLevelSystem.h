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
#include <unordered_map>

enum GuildSettingFlag
{
    GUILD_SETTING_FLAG_NONE            = 0,
    GUILD_SETTING_FLAG_ADM_CHAT        = 1,
    GUILD_SETTING_FLAG_ASSISTANT_CHAT  = 2
};

class GuildLevel
{
public:
    GuildLevel::GuildLevel(uint32 level, uint32 exp, Guild* _source)
        : Level(level), Exp(exp), source(_source) { }

    // Get
    uint32 GetLevel() { return Level; }
    uint32 GetExp() { return Exp; }
    Guild* GetSource() { return source; }    

    // Add
    void AddExp(uint32 exp);

private:
    uint32 Level;
    uint32 Exp;
    Guild* source;
};

class GuildLevelSystem
{
public:
    static GuildLevelSystem* instance();

    void Init();

    void LoadExpGuildForLevel();
    void LoadGuildSetting();
    void LoadGuildSpellReward();
    void LoadGuildLevels();
    void SetGuildLevels();

    void SetFullName(uint32 guildID);

    uint32 GetMaxLevel() { return _maxLevel; }
    uint32 GetExpItemID() { return _expItemID; }

    uint32 GetExpForNextLevel(uint32 level);
    void UpdateGuildVisibleLevel(uint32 guildID);
    void UpdateRewardForNewLevel(uint32 guildID);
    void AddGuildExp(Guild* guild, uint32 exp);
    void GuildInfo(Player* player);
    void InvestExpFull(Player* player);
    void InvestExpChoice(Player* player, char const* code);

    uint32 GetCountExpItem(Player* player);
    bool IsAsseptSetting(GuildSettingFlag Flag, Guild* guild);

    std::string IsAdmChat(Guild* guild);
    std::string IsAssistantChat(Guild* guild);

    void ChangeGuildSetting(GuildSettingFlag Flag, Guild* guild, bool Apply);
    void ChangeSettingChat(Guild* guild, bool IsAdmChat);
    void RewardSpellGuildMember(Guild* guild, Player* player);
    void UnRewardSpellGuildMember(Guild* guild, Player* player);

    GuildLevel* GetGLS(uint32 guildid, bool needCheck = true);
    void AddGLS(Guild* guild, uint32 level, uint32 exp);
    void AddEmptyGLS(Guild* guild);
    void DeleteGLS(Guild* guild);

    template<typename Format, typename... Args>
    inline void SendGuildFormat(Guild* guild, Format&& fmt, Args&& ... args)
    {
        SendGuildMessage(guild, warhead::StringFormat(std::forward<Format>(fmt), std::forward<Args>(args)...));
    }

private:
    std::unordered_map<uint32, uint32> _guildExpForLevelStore;
    std::unordered_map<uint32, uint32> _guildSettingStore;
    std::unordered_map<uint32, std::vector<uint32>> _guildSpellRewardStore;

    std::unordered_map<uint32 /*guild id*/, GuildLevel*> _guildstore;

    std::string const GetItemLocale(uint32 ItemID, int8 index_loc = 8);
    std::string const GetItemLink(uint32 itemid, int8 index_loc = 8);    

    void SendGuildMessage(Guild* guild, std::string&& message);

    uint32 _maxLevel = 0;
    uint32 _expItemID = 0;
};

#define sGuildLevelSystem GuildLevelSystem::instance()

#endif /* _GUILD_LEVEL_SYSTEM_H_ */
