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

#include "GuildLevelSystem.h"
#include "Log.h"
#include "GameConfig.h"
#include "Chat.h"
#include "ScriptedGossip.h"
#include "GameLocale.h"
#include "GameTime.h"
#include "World.h"

//void GuildLevel::AddExp(uint32 exp)
//{
//    if (!CONF_GET_BOOL("GLS.Enable") || exp < 1)
//        return;
//
//    uint32 level = GetLevel();
//    uint32 curXP = GetExp();
//    uint32 nextLvlXP = sGuildLevelSystem->GetExpForNextLevel(level);
//    uint32 newXP = curXP + exp;
//
//    while (newXP >= nextLvlXP && _level < CONF_GET_INT("GLS.MaxLevel"))
//    {
//        newXP -= nextLvlXP;
//
//        if (level < sGuildLevelSystem->GetMaxLevel())
//        {
//            sGuildLevelSystem->SendGuildFormat(_source, "|cffff0000#|r |cff6C8CD5Гильдия|r %s |cff6C8CD5достигла|r %u-го |cff6C8CD5уровня.|r", _source->GetName().c_str(), level + 1);
//
//            _level = level + 1;
//            sGuildLevelSystem->UpdateGuildVisibleLevel(_source->GetId());
//            sGuildLevelSystem->UpdateRewardForNewLevel(_source->GetId());
//
//            CharacterDatabase.PExecute("UPDATE `gls_guild_level` SET `Level` = %u WHERE `GuildID` = %u", level + 1, _source->GetId());
//        }
//
//        level = GetLevel();
//        nextLvlXP = sGuildLevelSystem->GetExpForNextLevel(level);
//    }
//
//    _exp = newXP;
//    CharacterDatabase.PExecute("UPDATE `gls_guild_level` SET `Exp` = %u WHERE `GuildID` = %u", newXP, _source->GetId());
//}

GuildCriteria::GuildCriteria(uint32 guildID)
    : _guildID(guildID)
{
    AddBaseCriterias();
    RescalingCriterias();
}

void GuildCriteria::AddBaseCriterias()
{
    for (auto const& itr : sGuildLevelSystem->GetBaseCriterias())
        _guildCriteria.insert(std::make_pair(itr.first, itr.second));
}

void GuildCriteria::RescalingCriterias()
{
    auto guild = sGuildMgr->GetGuildById(_guildID);
    uint32 members = guild ? guild->GetMemberCount() : 0;

    if (!members)
        ABORT_MSG("> GLS: No members count in guild (%u)", _guildID);

    for (auto& itr : _guildCriteria)
    {
        auto& criteria = itr.second;

        float coef = criteria.Coef;
        uint32 criteriaID = itr.first;
        uint32 minPlayers = criteria.MinPlayersCount;
        uint32 diff = members > minPlayers ? members - minPlayers : 0;

        if (diff)
        {
            float totalCoef = 1.0f + diff * coef;

            for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
                criteria.ItemCount[i] = sGuildLevelSystem->GetMaxCriteriaItemCountBase(criteriaID, i) * totalCoef;
        }
    }
}

void GuildCriteria::AddProgress(uint32 criteriaID, GuildCriteriaProgressStruct& _data)
{
    _guildCriteriaProgress.insert(std::make_pair(criteriaID, _data));
}

void GuildCriteria::AddEmptyProgress(uint32 criteriaID)
{
    GuildCriteriaProgressStruct _data;
    _data.CriteriaID = criteriaID;
    _data.GuildID = _guildID;

    for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
        _data.ItemCount[i] = 0;

    AddProgress(criteriaID, _data);
}

uint32 GuildCriteria::GetMaxCriteriaItemCount(uint32 criteriaID, uint8 itemType)
{
    auto const& itr = _guildCriteria.find(criteriaID);
    if (itr != _guildCriteria.end())
        return itr->second.ItemCount[itemType];

    ABORT_MSG("> GLS: Not found criteria (%u) for guild id (%u)", criteriaID, _guildID);

    return 0;
}

uint32 GuildCriteria::GetItemCountProgress(uint32 criteriaID, uint8 itemType)
{
    auto const& itr = _guildCriteriaProgress.find(criteriaID);
    if (itr != _guildCriteriaProgress.end())
        return itr->second.ItemCount[itemType];

    return 0;
}

void GuildCriteria::AddItemProgess(uint32 criteriaID, uint8 itemType, uint32 itemCount)
{
    auto& progress = _guildCriteriaProgress.find(criteriaID);
    if (progress == _guildCriteriaProgress.end())
        AddEmptyProgress(criteriaID);

    progress = _guildCriteriaProgress.find(criteriaID);
    if (progress == _guildCriteriaProgress.end())
        ABORT_MSG("> GLS: Not found criteria progress (%u) after insert empty", criteriaID);

    // Add progress count
    progress->second.ItemCount[itemType] += itemCount;

    std::string itemsCount;

    for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
        itemsCount += std::to_string(progress->second.ItemCount[i]) + ",";

    // Delete last (,)
    if (!itemsCount.empty())
        itemsCount.erase(itemsCount.end() - 1, itemsCount.end());

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    trans->PAppend("DELETE FROM `gls_criteria_progress` WHERE `GuildID` = %u AND `CriteriaID` = %u", progress->second.GuildID, progress->second.CriteriaID);
    trans->PAppend("INSERT INTO `gls_criteria_progress`(`GuildID`, `CriteriaID`, `ItemCount`) VALUES (%u, %u, '%s')", progress->second.GuildID, progress->second.CriteriaID, itemsCount.c_str());

    CharacterDatabase.CommitTransaction(trans);
}

uint32 GuildCriteria::GetCriteriaItemID(uint32 criteriaID, uint8 itemType)
{
    auto& progress = _guildCriteria.find(criteriaID);
    if (progress == _guildCriteria.end())
        ABORT_MSG("> GLS: Not found criteria (%u) for guild id (%u)", criteriaID, _guildID);

    return progress->second.ItemID[itemType];
}

GuildLevelSystem* GuildLevelSystem::instance()
{
    static GuildLevelSystem instance;
    return &instance;
}

void GuildLevelSystem::Init()
{
    LOG_INFO("module", "Loading Guild Level System");

    /*_expItemID = CONF_GET_INT("GLS.Exp.ItemID");

    ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(_expItemID);
    if (!itemTemplate)
    {
        LOG_ERROR("module.gls", "> ItemID (%u) for GLS is not valid. Set default (37711)", CONF_GET_INT("GLS.Exp.ItemID"));
        _expItemID = 37711;
    }

    _maxLevel = CONF_GET_INT("GLS.MaxLevel");*/

    //LoadExpGuildForLevel();
    //LoadGuildSetting();
    //LoadGuildSpellReward();
    //LoadGuildLevels();
    //SetGuildLevels();

    LOG_INFO("modules.gls", ">> Loading gls criteria");
    LoadBaseCriteria();

    LOG_INFO("modules.gls", ">> Loading gls criteria progress");
    LoadCriteriaProgress();

    LOG_INFO("modules", "");
}

//void GuildLevelSystem::SetGuildLevels()
//{
//    for (auto const& itr : sGuildMgr->GetAllGuilds())
//        SetFullName(itr.second->GetId());
//}
//
//void GuildLevelSystem::LoadGuildLevels()
//{
//    uint32 oldMSTime = getMSTime();
//
//    _guildstore.clear();
//
//    //                                                    0          1
//    QueryResult result = CharacterDatabase.Query("SELECT `GuildID`, `Level`, `Exp` FROM `gls_guild_level`");
//
//    if (!result)
//    {
//        LOG_ERROR("sql.sql", "> Загружено 0 GLS уровней гильдий. Таблица `gls_guild_level` возможно пустая.");
//        return;
//    }
//
//    do
//    {
//        Field* fields = result->Fetch();
//
//        uint32 guildid = fields[0].GetUInt32();
//        uint32 level = fields[1].GetUInt32();
//        uint32 exp = fields[2].GetUInt32();
//
//        Guild* guild = sGuildMgr->GetGuildById(guildid);
//        if (!guild)
//        {
//            LOG_ERROR("sql.sql", "> Не сущвествует гильдии под номером (%u)", guildid);
//            continue;            
//        }
//
//        AddGLS(guild, level, exp);
//
//    } while (result->NextRow());
//
//    LOG_INFO("module.gls", ">> Загружено %u GLS уровней гильдий за %u мс", static_cast<uint32>(_guildstore.size()), GetMSTimeDiffToNow(oldMSTime));
//}
//
//void GuildLevelSystem::LoadExpGuildForLevel()
//{
//    uint32 oldMSTime = getMSTime();
//
//    _guildExpForLevelStore.clear();
//
//    //                                                    0        1
//    QueryResult result = CharacterDatabase.Query("SELECT `Level`, `Exp` FROM `gls_exp_for_level`");
//
//    if (!result)
//    {
//        LOG_ERROR("sql.sql", "> Загружено 0 уровней для системы гильдий. Таблица `gls_exp_for_level` возможно пустая.");
//        return;
//    }
//
//    do
//    {
//        Field* fields = result->Fetch();
//
//        uint32 current_level = fields[0].GetUInt32();
//        uint32 current_xp = fields[1].GetUInt32();
//        uint32 MaxLevelGuild = GetMaxLevel();
//
//        if ((current_level + 1) > MaxLevelGuild)
//        {
//            LOG_ERROR("module.gls", "> Уровень гильдии (%u) больше, чем максимальный (%u)", current_level + 1, MaxLevelGuild);
//            continue;
//        }
//
//        if (!current_xp)
//        {
//            LOG_ERROR("module.gls", "> Опыт для следующего уровня на уровне (%u) не может быть равен нулю", current_level);
//            continue;
//        }
//
//        _guildExpForLevelStore.insert(std::make_pair(current_level, current_xp));
//
//    } while (result->NextRow());
//
//    LOG_INFO("module.gls", ">> Загружено %u уровней для системы гильдий за %u мс", static_cast<uint32>(_guildExpForLevelStore.size()), GetMSTimeDiffToNow(oldMSTime));
//}
//
//void GuildLevelSystem::LoadGuildSetting()
//{
//    uint32 oldMSTime = getMSTime();
//
//    _guildSettingStore.clear();
//
//    //                                                    0          1
//    QueryResult result = CharacterDatabase.Query("SELECT `GuildID`, `Flag` FROM `gls_guild_setting`");
//
//    if (!result)
//    {
//        LOG_ERROR("sql.sql", "> Загружено 0 настроек гильдий. Таблица `gls_guild_setting` возможно пустая.");
//        return;
//    }
//
//    do
//    {
//        Field* fields = result->Fetch();
//        _guildSettingStore.insert(std::make_pair(fields[0].GetUInt32(), fields[1].GetUInt32()));
//
//    } while (result->NextRow());
//
//    LOG_INFO("module.gls", ">> Загружено %u настроек гильдий за %u мс", static_cast<uint32>(_guildSettingStore.size()), GetMSTimeDiffToNow(oldMSTime));
//}
//
//void GuildLevelSystem::LoadGuildSpellReward()
//{
//    uint32 oldMSTime = getMSTime();
//
//    _guildSpellRewardStore.clear();
//
//    //                                                    0             1
//    QueryResult result = CharacterDatabase.Query("SELECT `GuildLevel`, `ListSpells` FROM `gls_reward_spell` ORDER BY GuildLevel");
//
//    if (!result)
//    {
//        LOG_ERROR("sql.sql", "> Загружено 0 наградных спеллов гильдий. Таблица `gls_reward_spell` возможно пустая.");
//        return;
//    }
//
//    uint32 spellCount = 0;
//
//    do
//    {
//        Field* fields = result->Fetch();
//
//        uint32 GuildLevel = fields[0].GetUInt32();
//        std::string ListSpells = fields[1].GetString();
//
//        Tokenizer SpellList(ListSpells, ',');
//        for (uint8 i = 0; i < SpellList.size(); i++)
//        {
//            uint32 SpellID = (uint32)atoi(SpellList[i]);
//
//            auto spell = sSpellStore.LookupEntry(SpellID);
//            if (!spell)
//            {
//                LOG_ERROR("sql.sql", "Заклинания под номером (%u) не существует. Пропуск.", SpellID);
//                continue;
//            }
//
//            _guildSpellRewardStore[GuildLevel].push_back(SpellID);
//            spellCount++;
//        }
//
//        if (_guildSpellRewardStore[GuildLevel].empty())
//        {
//            LOG_ERROR("sql.sql", "Для уровня (%u) нет заклинаний. Пропуск.", GuildLevel);
//            _guildSpellRewardStore[GuildLevel].clear();
//            continue;
//        }
//
//    } while (result->NextRow());
//
//    LOG_INFO("module.gls", ">> Загружено %u наградных спеллов для %u уровней гильдий за %u мс", spellCount, static_cast<uint32>(_guildSpellRewardStore.size()), GetMSTimeDiffToNow(oldMSTime));
//}
//
//GuildCriteria* GuildLevelSystem::GetCriteria(uint32 guildid, bool needCheck /*= true*/)
//{
//    auto const& itr = _guildstore.find(guildid);
//    if (itr != _guildstore.end())
//        return itr->second;
//
//    if (needCheck)
//        ABORT_MSG("> Guild id (%u) is not found in GLS", guildid);
//
//    return nullptr;
//}
//
//void GuildLevelSystem::AddGLS(Guild* guild, uint32 level, uint32 exp)
//{
//    if (GetCriteria(guild->GetId(), false))
//        ABORT_MSG("> Guild id (%u) is exist in GLS", guild->GetId());
//
//    _guildstore.insert(std::make_pair(guild->GetId(), new GuildLevel(level, exp, guild)));
//}
//
//void GuildLevelSystem::AddEmptyGLS(Guild* guild)
//{
//    if (GetCriteria(guild->GetId(), false))
//        ABORT_MSG("> Guild id (%u) is exist in GLS", guild->GetId());
//
//    SQLTransaction trans = CharacterDatabase.BeginTransaction();
//
//    trans->PAppend("INSERT INTO `gls_guild_level`(`GuildID`, `Level`) VALUES (%u, 1)", guild->GetId());
//    trans->PAppend("INSERT INTO `gls_guild_setting`(`GuildID`, `Flag`) VALUES (%u, 0)", guild->GetId());
//
//    CharacterDatabase.CommitTransaction(trans);
//
//    AddGLS(guild, 1, 0);
//}
//
//void GuildLevelSystem::DeleteGLS(Guild* guild)
//{
//    uint32 guildID = guild->GetId();
//
//    SQLTransaction trans = CharacterDatabase.BeginTransaction();
//
//    trans->PAppend("DELETE FROM `gls_guild_level` WHERE `GuildID` = %u", guildID);
//    trans->PAppend("DELETE FROM `gls_guild_setting` WHERE `GuildID` = %u", guildID);
//
//    CharacterDatabase.CommitTransaction(trans);
//
//    auto const& itr = _guildstore.find(guildID);
//    if (itr == _guildstore.end())
//        return;
//
//    _guildstore.erase(guildID);
//}
//
//uint32 GuildLevelSystem::GetExpForNextLevel(uint32 level)
//{
//    if (!CONF_GET_BOOL("GLS.Enable"))
//        return 0;
//
//    auto const& itr = _guildExpForLevelStore.find(level);
//    if (itr != _guildExpForLevelStore.end())
//        return itr->second;
//
//    return 0;
//}
//
//void GuildLevelSystem::UpdateGuildVisibleLevel(uint32 guildID)
//{
//    Guild* guild = sGuildMgr->GetGuildById(guildID);
//    if (!guild)
//        return;
//
//    SetFullName(guildID);
//
//    for (auto const& itr : sWorld->GetAllSessions())
//    {
//        if (!itr.second)
//            continue;
//
//        Player* player = itr.second->GetPlayer();
//        if (!player)
//            continue;
//
//        if (!player->IsInWorld())
//            continue;
//
//        if (player->GetGuildId() != guild->GetId())
//            continue;
//
//        guild->HandleQuery(itr.second);
//    }
//}
//
//void GuildLevelSystem::UpdateRewardForNewLevel(uint32 guildID)
//{
//    Guild* guild = sGuildMgr->GetGuildById(guildID);
//    if (!guild)
//        return;
//
//    for (auto const& itr : sWorld->GetAllSessions())
//    {
//        if (!itr.second)
//            continue;
//
//        Player* player = itr.second->GetPlayer();
//        if (!player)
//            continue;
//
//        if (!player->IsInWorld())
//            continue;
//
//        if (player->GetGuildId() != guild->GetId())
//            continue;
//
//        RewardSpellGuildMember(guild, player);
//    }
//}
//
//void GuildLevelSystem::AddGuildExp(Guild* guild, uint32 exp)
//{
//    auto gls = GetCriteria(guild->GetId());
//    if (!gls)
//        return;
//
//    gls->AddExp(exp);
//}
//
//void GuildLevelSystem::GuildInfo(Player* player)
//{
//    auto gls = GetCriteria(player->GetGuildId());
//    ChatHandler handler(player->GetSession());
//    //uint32 level = gls->GetLevel();
//    //uint32 exp = gls->GetExp();
//    //uint32 nextLevelExp = GetExpForNextLevel(level);    
//    //uint32 remainedExp = nextLevelExp - exp;
//
//    //if (!nextLevelExp)
//        //remainedExp = 0;
//
//    //std::string info1 = warhead::StringFormat("|cff790C06Уровень -|r |cFF1E90FF%u|r", level);
//    //std::string info2 = warhead::StringFormat("|cff790C06Опыт гильдии:|r |cFF1E90FF%u|r |cff790C06/|r |cFF1E90FF%u|r", exp, nextLevelExp);
//    //std::string info3 = warhead::StringFormat("|cff790C06Осталось до|r |cFF1E90FF%u-го|r |cff790C06уровня -|r |cFF1E90FF%u|r |cff790C06опыта|r", level + 1, remainedExp);
//
//    //AddGossipItemFor(player, 10, info1, GOSSIP_SENDER_MAIN, 999);
//    //AddGossipItemFor(player, 10, info2, GOSSIP_SENDER_MAIN, 999);
//    //AddGossipItemFor(player, 10, info3, GOSSIP_SENDER_MAIN, 999);
//    AddGossipItemFor(player, 10, "Обновить меню", GOSSIP_SENDER_MAIN, 1);
//    AddGossipItemFor(player, 10, "<- Назад", GOSSIP_SENDER_MAIN, 101);
//}
//
//void GuildLevelSystem::InvestExpFull(Player* player)
//{
//    ChatHandler handler(player->GetSession());
//    Guild* guild = player->GetGuild();
//    std::string PlayerName = player->GetName();
//    int8 Locale_index = handler.GetSessionDbLocaleIndex();
//    uint32 guildExpItemID = GetExpItemID();
//    std::string const& ItemName = GetItemLocale(guildExpItemID, Locale_index);
//    std::string const& ItemLink = GetItemLink(guildExpItemID, Locale_index);
//    uint32 ItemCount = player->GetItemCount(guildExpItemID);
//
//    if (!ItemCount)
//    {
//        handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 У вас нет|r %s", ItemLink.c_str());
//        return;
//    }
//
//    player->DestroyItemCount(guildExpItemID, ItemCount, true);
//    AddGuildExp(guild, ItemCount);
//    CharacterDatabase.PExecute("INSERT INTO `gls_invested`(`GuildID`, `PlayerName`, `InvestExp`, `UnixDate`) VALUES (%u, '%s', %u, %u)", guild->GetId(), PlayerName.c_str(), ItemCount, GameTime::GetGameTime());
//}
//
//uint32 GuildLevelSystem::GetCountExpItem(Player* player)
//{
//    return player->GetItemCount(GetExpItemID());
//}
//
//bool GuildLevelSystem::IsAsseptSetting(GuildSettingFlag Flag, Guild* guild)
//{
//    auto const& itr = _guildSettingStore.find(guild->GetId());
//    if (itr != _guildSettingStore.end())
//    {
//        if (itr->second & Flag)
//            return true;
//    }
//
//    return false;
//}
//
//std::string GuildLevelSystem::IsAdmChat(Guild* guild)
//{
//    if (IsAsseptSetting(GUILD_SETTING_FLAG_ADM_CHAT, guild))
//        return "|cff008000[Включено]|r";
//
//    return "|cffff0000[Выключено]|r";
//}
//
//std::string GuildLevelSystem::IsAssistantChat(Guild* guild)
//{
//    if (IsAsseptSetting(GUILD_SETTING_FLAG_ASSISTANT_CHAT, guild))
//        return "|cff008000[Включено]|r";
//
//    return "|cffff0000[Выключено]|r";
//}
//
//void GuildLevelSystem::ChangeGuildSetting(GuildSettingFlag Flag, Guild* guild, bool Apply)
//{
//    uint32 guildID = guild->GetId();
//
//    if (Apply)
//    {
//        _guildSettingStore[guildID] = _guildSettingStore[guildID] |= Flag;
//        CharacterDatabase.PExecute("UPDATE `gls_guild_setting` SET `FlagSetting` = FlagSetting | %u WHERE `GuildID` = %u", uint32(Flag), guildID);
//    }
//    else
//    {
//        _guildSettingStore[guildID] = _guildSettingStore[guildID] &= ~Flag;
//        CharacterDatabase.PExecute("UPDATE `gls_guild_setting` SET `FlagSetting` = FlagSetting & ~%u WHERE `GuildID` = %u", uint32(Flag), guildID);
//    }
//}
//
//void GuildLevelSystem::ChangeSettingChat(Guild* guild, bool IsAdmChat)
//{
//    uint32 GuildID = guild->GetId();
//
//    if (IsAdmChat)
//    {
//        if (IsAsseptSetting(GUILD_SETTING_FLAG_ADM_CHAT, guild))
//            ChangeGuildSetting(GUILD_SETTING_FLAG_ADM_CHAT, guild, false);
//        else
//            ChangeGuildSetting(GUILD_SETTING_FLAG_ADM_CHAT, guild, true);
//    }
//    else
//    {
//        if (IsAsseptSetting(GUILD_SETTING_FLAG_ASSISTANT_CHAT, guild))
//            ChangeGuildSetting(GUILD_SETTING_FLAG_ASSISTANT_CHAT, guild, false);
//        else
//            ChangeGuildSetting(GUILD_SETTING_FLAG_ASSISTANT_CHAT, guild, true);
//    }
//}
//
//void GuildLevelSystem::RewardSpellGuildMember(Guild* guild, Player* player)
//{
//    ASSERT(player && guild);
//
//    if (_guildSpellRewardStore.empty())
//        return;
//
//    auto gls = GetCriteria(guild->GetId());
//
//    uint32 guildLevel = gls->GetLevel();
//
//    for (uint32 level = 1; level < guildLevel + 1; level++)
//    {
//        auto const& itr = _guildSpellRewardStore.find(level);
//        if (itr != _guildSpellRewardStore.end())
//        {
//            for (auto const& i : itr->second)
//            {
//                if (!player->HasSpell(i))
//                    player->learnSpell(i);
//            }
//        }
//    }
//}
//
//void GuildLevelSystem::UnRewardSpellGuildMember(Guild* guild, Player* player)
//{
//    ASSERT(player && guild);
//
//    if (_guildSpellRewardStore.empty())
//        return;
//
//    for (uint32 level = 1; level < GetMaxLevel(); level++)
//    {
//        auto const& itr = _guildSpellRewardStore.find(level);
//        if (itr != _guildSpellRewardStore.end())
//        {
//            for (auto const& i : itr->second)
//            {
//                if (player->HasSpell(i))
//                    player->removeSpell(i, SPEC_MASK_ALL, false);
//            }
//        }
//    }
//}

void GuildLevelSystem::SendGuildMessage(Guild* guild, std::string&& message)
{
    WorldPacket data;
    ChatHandler::BuildChatPacket(data, CHAT_MSG_SYSTEM, LANG_UNIVERSAL, nullptr, nullptr, message);

    for (auto const& itr : sWorld->GetAllSessions())
    {
        Player* player = itr.second->GetPlayer();
        if (!player)
            continue;

        if (!player->IsInWorld())
            continue;

        if (player->GetGuild() != guild)
            continue;

        itr.second->SendPacket(&data);
    }
}

std::string const GuildLevelSystem::GetItemLocale(uint32 ItemID, int8 index_loc)
{
    ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(ItemID);
    ItemLocale const* itemLocale = sGameLocale->GetItemLocale(ItemID);
    std::string name;

    if (itemLocale)
        name = itemLocale->Name[index_loc];

    if (name.empty() && itemTemplate)
        name = itemTemplate->Name1;

    return name;
}

std::string const GuildLevelSystem::GetItemLink(uint32 itemid, int8 index_loc)
{
    ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemid);
    std::string name = GetItemLocale(itemid, index_loc);

    std::string color = "cffffffff";

    switch (itemTemplate->Quality)
    {
    case 0:
        color = "cff9d9d9d";
        break;
    case 1:
        color = "cffffffff";
        break;
    case 2:
        color = "cff1eff00";
        break;
    case 3:
        color = "cff0070dd";
        break;
    case 4:
        color = "cffa335ee";
        break;
    case 5:
        color = "cffff8000";
        break;
    case 6:
    case 7:
        color = "cffe6cc80";
        break;
    default:
        ABORT();
        break;
    }

    return warhead::StringFormat("|%s|Hitem:%u:0:0:0:0:0:0:0:0|h[%s]|h|r", color.c_str(), itemid, name.c_str());
}

//void GuildLevelSystem::SetFullName(uint32 guildID)
//{
//    Guild* guild = sGuildMgr->GetGuildById(guildID);
//    if (!guild)
//        return;
//
//    std::string const& fullName = warhead::StringFormat("%s (%u level)", guild->GetName().c_str(), GetCriteria(guild->GetId())->GetLevel());
//
//    guild->SetName(fullName, true);
//}
//
//void GuildLevelSystem::SetFullNameFirstLevel(Guild* guild)
//{
//    ASSERT(guild);
//
//    std::string const& fullName = warhead::StringFormat("%s (1 level)", guild->GetName().c_str());
//
//    guild->SetName(fullName, true);
//}

// Criteria
void GuildLevelSystem::LoadBaseCriteria()
{
    uint32 oldMSTime = getMSTime();

    _guildCriteriaBase.clear();

    QueryResult result = CharacterDatabase.Query("SELECT "
        "`CriteriaID`,"                 // 0
        "`ListItemID`,"                 // 1
        "`ListItemCount`,"              // 2
        "`MinPlayersCount`,"            // 3
        "`Coef`,"                       // 4
        "`RewardItemID`,"               // 5
        "`RewardItemCount`,"            // 6
        "`ListRewardChooseItemID`,"     // 7
        "`ListRewardChooseItemCount` "  // 8
        "FROM `gls_criteria`");

    if (!result)
    {
        LOG_WARN("modules.gls", "> Загружено 0 GLS критерий. Таблица `gls_criteria` пустая.");
        LOG_INFO("modules.gls", "");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        auto CheckItemIDs = [](std::vector<uint32> itemID) -> bool
        {
            for (auto const& _itemID : itemID)
            {
                if (!_itemID)
                    continue;

                ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(_itemID);
                if (!itemTemplate)
                {
                    LOG_ERROR("modules.gls", "> GLS: ItemID (%u) for GLS criteria is not valid.", _itemID);
                    return false;
                }
            }

            return true;
        };

        uint32 criteriaID                       = fields[0].GetUInt32();
        std::string listItemID                  = fields[1].GetString();
        std::string listItemCount               = fields[2].GetString();
        uint32 minPlayersCount                  = fields[3].GetUInt32();
        float coef                              = fields[4].GetFloat();
        uint32 rewardItemID                     = fields[5].GetUInt32();
        uint32 rewardItemCount                  = fields[6].GetUInt32();
        std::string listRewardChooseItemID      = fields[7].GetString();
        std::string listRewardChooseItemCount   = fields[8].GetString();
        
        std::vector<uint32> toCheckItems = { rewardItemID };

        GuildCriteriaStruct _data;
        _data.CriteriaID                = criteriaID;
        _data.MinPlayersCount           = minPlayersCount;
        _data.Coef                      = coef;
        _data.RewardItemID              = rewardItemID;
        _data.RewardItemCount           = rewardItemCount;

        Tokenizer listItemIDTokens(listItemID, ',');
        Tokenizer listItemCountTokens(listItemCount, ',');

        if (static_cast<uint32>(listItemIDTokens.size()) > GLS_ITEMS_COUNT)
        {
            LOG_FATAL("modules.gls", "> GLS: List items for CriteriaID (%u) > %u", criteriaID, GLS_ITEMS_COUNT);
            continue;
        }

        if (static_cast<uint32>(listItemCountTokens.size()) > GLS_ITEMS_COUNT)
        {
            LOG_FATAL("modules.gls", "> GLS: List items count for CriteriaID (%u) > %u", criteriaID, GLS_ITEMS_COUNT);
            continue;
        }

        if (listItemIDTokens.size() != listItemCountTokens.size())
        {
            LOG_FATAL("modules.gls", "> GLS: Differenst size data between `ListItemID` and `ListItemCount`");
            continue;
        }

        for (uint32 i = 0; i < static_cast<uint32>(listItemIDTokens.size()); ++i)
        {
            _data.ItemID[i] = atoi(listItemIDTokens[i]);
            toCheckItems.push_back(_data.ItemID[i]);
        }

        for (uint32 i = 0; i < static_cast<uint32>(listItemCountTokens.size()); ++i)
            _data.ItemCount[i] = atoi(listItemCountTokens[i]);

        // Reward items
        Tokenizer listRewardChooseItemIDTokens(listRewardChooseItemID, ',');
        Tokenizer listRewardChooseItemCountTokens(listRewardChooseItemCount, ',');

        if (static_cast<uint32>(listRewardChooseItemIDTokens.size()) > GLS_ITEMS_REWARD_CHOOSE_COUNT)
        {
            LOG_FATAL("modules.gls", "> GLS: List shoose reward items for CriteriaID (%u) > %u", criteriaID, GLS_ITEMS_REWARD_CHOOSE_COUNT);
            continue;
        }

        if (static_cast<uint32>(listRewardChooseItemCountTokens.size()) > GLS_ITEMS_REWARD_CHOOSE_COUNT)
        {
            LOG_FATAL("modules.gls", "> GLS: List shoose reward items count for CriteriaID (%u) > %u", criteriaID, GLS_ITEMS_REWARD_CHOOSE_COUNT);
            continue;
        }

        if (listRewardChooseItemIDTokens.size() != listRewardChooseItemCountTokens.size())
        {
            LOG_FATAL("modules.gls", "> GLS: Differenst size data between `ListRewardChooseItemID` and `ListRewardChooseItemCount`");
            continue;
        }

        for (uint32 i = 0; i < static_cast<uint32>(listRewardChooseItemIDTokens.size()); ++i)
        {
            _data.RewardChooseItemID[i] = atoi(listRewardChooseItemIDTokens[i]);
            toCheckItems.push_back(_data.RewardChooseItemID[i]);
        }

        for (uint32 i = 0; i < static_cast<uint32>(listRewardChooseItemCountTokens.size()); ++i)
            _data.RewardChooseItemCount[i] = atoi(listRewardChooseItemCountTokens[i]);

        if (!CheckItemIDs(toCheckItems))
            continue;

        _guildCriteriaBase.insert(std::make_pair(criteriaID, _data));

    } while (result->NextRow());

    LOG_INFO("modules.gls", ">> Загружено %u GLS критерий за %u мс", static_cast<uint32>(_guildCriteriaBase.size()), GetMSTimeDiffToNow(oldMSTime));
    LOG_INFO("modules.gls", "");
}

void GuildLevelSystem::LoadCriteriaProgress()
{
    uint32 oldMSTime = getMSTime();

    _guildCriteriaProgress.clear();

    QueryResult result = CharacterDatabase.Query("SELECT "
        "`GuildID`,"     // 0
        "`CriteriaID`,"  // 1
        "`ItemCount` "   // 2
        "FROM `gls_criteria_progress`");

    if (!result)
    {
        LOG_WARN("modules.gls", "> Загружено 0 GLS критерий прогресса. Таблица `gls_criteria_progress` пустая.");
        LOG_INFO("modules.gls", "");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 guildID              = fields[0].GetUInt32();
        uint32 criteriaID           = fields[1].GetUInt32();
        std::string listItemCount   = fields[2].GetString();

        // Check guild
        if (!sGuildMgr->GetGuildById(guildID))
        {
            LOG_ERROR("modules.gls", "> GLS: guild id (%u) is not exist!. Skip", guildID);
            continue;
        }

        GuildCriteriaProgressStruct _data;
        _data.GuildID       = guildID;
        _data.CriteriaID    = criteriaID;

        Tokenizer listItemCountTokens(listItemCount, ',');

        if (static_cast<uint32>(listItemCountTokens.size()) > GLS_ITEMS_COUNT)
        {
            LOG_FATAL("modules.gls", "> GLS: List items count for CriteriaID (%u) > %u", criteriaID, GLS_ITEMS_COUNT);
            continue;
        }

        for (uint32 i = 0; i < static_cast<uint32>(listItemCountTokens.size()); ++i)
            _data.ItemCount[i] = atoi(listItemCountTokens[i]);

        auto criteria = GetCriteriaProgress(guildID);
        if (!criteria)
            criteria = new GuildCriteria(guildID);

        criteria->AddProgress(criteriaID, _data);

        _guildCriteriaProgress.insert(std::make_pair(guildID, criteria));

        count++;
    } while (result->NextRow());

    LOG_INFO("modules.gls", ">> Загружено %u GLS критерий прогресса за %u мс", count, GetMSTimeDiffToNow(oldMSTime));
    LOG_INFO("modules.gls", "");
}

void GuildLevelSystem::InvestItem(Player* player, Creature* creature, uint32 action, uint8 sender, uint32 itemCount)
{
    uint32 criteriaID = action - GLS_GOSSIP_CHOOSE_INVEST, itemType = sender;

    if (itemType > GLS_ITEMS_COUNT)
    {
        LOG_ERROR("modules.gls", "> GLS: invalid item type (%u)", itemType);
        return;
    }

    ChatHandler handler(player->GetSession());

    auto guildID = player->GetGuildId();
    if (!guildID)
    {
        handler.PSendSysMessage("> Вы не состоите в гильдии!");
        return;
    }

    auto criteriaProgress = GetCriteriaProgress(guildID, true);
    if (!criteriaProgress)
        ABORT();

    uint32 itemID = criteriaProgress->GetCriteriaItemID(criteriaID, itemType);
    if (!itemID)
    {
        LOG_ERROR("modules.gls", "> GLS: invalid item id at invest (%u-%u)", criteriaID, itemType);
        handler.PSendSysMessage("> Неверный номер предмета (%u-%u)", criteriaID, itemType);
    }

    int8 Locale_index = handler.GetSessionDbLocaleIndex();
    std::string const& ItemLink = GetItemLink(itemID, Locale_index);
    uint32 hasItemCount = player->GetItemCount(itemID);    

    if (!player->HasItemCount(itemID, itemCount))
    {
        handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вам не хватает|r %s - %u", ItemLink.c_str(), itemCount - hasItemCount);
        ShowInvestedMenu(player, creature, action, sender);
        return;
    }

    uint32 maxItems = criteriaProgress->GetMaxCriteriaItemCount(criteriaID, itemType);
    uint32 currentItems = criteriaProgress->GetItemCountProgress(criteriaID, itemType);
    uint32 needItems = maxItems - currentItems;

    if (itemCount > needItems)
    {
        handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вы хотите вложить|r %s |cff6C8CD5больше, чем можно|r %u|cff6C8CD5. Введите корректное число|r", ItemLink.c_str(), needItems);
        ShowInvestedMenu(player, creature, action, sender);
        return;
    }

    player->DestroyItemCount(itemID, itemCount, true);
    criteriaProgress->AddItemProgess(criteriaID, itemType, itemCount);
    ShowInvestedMenu(player, creature, action, sender);
}

void GuildLevelSystem::InvestItemFull(Player* player, Creature* creature, uint32 action, uint8 sender)
{
    uint32 criteriaID = action - GLS_GOSSIP_FULL_INVEST, itemType = sender;

    if (itemType > GLS_ITEMS_COUNT)
    {
        LOG_ERROR("modules.gls", "> GLS: invalid item type (%u)", itemType);
        ShowInvestedMenu(player, creature, action, sender);
        return;
    }

    ChatHandler handler(player->GetSession());

    auto guildID = player->GetGuildId();
    if (!guildID)
    {
        handler.PSendSysMessage("> Вы не состоите в гильдии!");
        return;
    }

    auto criteriaProgress = GetCriteriaProgress(guildID, true);
    if (!criteriaProgress)
        ABORT();

    uint32 itemID = criteriaProgress->GetCriteriaItemID(criteriaID, itemType);
    if (!itemID)
    {
        LOG_ERROR("modules.gls", "> GLS: invalid item id at invest (%u-%u)", criteriaID, itemType);
        handler.PSendSysMessage("> Неверный номер предмета (%u-%u)", criteriaID, itemType);
    }
    auto ItemLink = GetItemLink(itemID, handler.GetSessionDbLocaleIndex());
    uint32 hasItemCount = player->GetItemCount(itemID);
    uint32 itemCount = hasItemCount;

    if (!itemCount)
    {
        handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 У вас нет|r %s", ItemLink.c_str());
        ShowInvestedMenu(player, creature, action, sender);
        return;
    }

    uint32 maxItems = criteriaProgress->GetMaxCriteriaItemCount(criteriaID, itemType);
    uint32 currentItems = criteriaProgress->GetItemCountProgress(criteriaID, itemType);
    uint32 needItems = maxItems - currentItems;

    if (itemCount > needItems)
        itemCount = needItems;

    player->DestroyItemCount(itemID, itemCount, true);
    criteriaProgress->AddItemProgess(criteriaID, itemType, itemCount);
    ShowInvestedMenu(player, creature, criteriaID + GLS_GOSSIP_CHOOSE_INVEST, sender);
}

GuildCriteria* GuildLevelSystem::GetCriteriaProgress(uint32 guildid, bool forceCreate /*= false*/)
{
    auto& itr = _guildCriteriaProgress.find(guildid);
    if (itr != _guildCriteriaProgress.end())
        return itr->second;

    if (forceCreate)
    {
        AddEmptyGuildCriteria(guildid);
        itr = _guildCriteriaProgress.find(guildid);
        if (itr != _guildCriteriaProgress.end())
            return itr->second;

        ABORT_MSG("> GLS: Invalid creeate empty guild criteria for guild (%u)", guildid);
    }

    return nullptr;
}

uint32 GuildLevelSystem::GetMaxCriteriaItemCountBase(uint32 criteriaID, uint8 itemType)
{
    auto const& itr = _guildCriteriaBase.find(criteriaID);
    if (itr != _guildCriteriaBase.end())
        return itr->second.ItemCount[itemType];

    LOG_FATAL("modules.gls", "> GLS: Not found base criteria (%u)", criteriaID);

    return 0;
}

GuildCriteriaStruct* GuildLevelSystem::GetCriteria(uint32 criteriaID)
{
    auto const& itr = _guildCriteriaBase.find(criteriaID);
    if (itr != _guildCriteriaBase.end())
        return &itr->second;

    return nullptr;
}

void GuildLevelSystem::ShowCritera(Player* player, Creature* creature)
{
    auto criteriaProgress = GetCriteriaProgress(player->GetGuildId(), true);
    if (!criteriaProgress)
        ABORT();

    AddGossipItemFor(player, 10, "Прогресс критерий:", GOSSIP_SENDER_MAIN, 2);

    for (auto const& itr : _guildCriteriaBase)
    {
        uint32 criteriaID = itr.first;
        uint32 gossipAction = criteriaID + GLS_GOSSIP_CHOOSE_INVEST;
        uint32 _count = 1;

        AddGossipItemFor(player, 10, warhead::StringFormat("--- Критерий #%u:", criteriaID), GOSSIP_SENDER_MAIN, 2);

        for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
        {
            auto itemName = GetItemLocale(itr.second.ItemID[i]);
            auto currentItems = criteriaProgress->GetItemCountProgress(criteriaID, i);
            auto maxItems = criteriaProgress->GetMaxCriteriaItemCount(criteriaID, i);

            if (!itemName.empty())
                AddGossipItemFor(player, 10, warhead::StringFormat("- %u. (%s) - %u/%u", i + 1, itemName.c_str(), currentItems, maxItems), i, gossipAction);
        }
    }
    
    AddGossipItemFor(player, 10, ">> В главное меню", GOSSIP_SENDER_MAIN, 99);
    SendGossipMenuFor(player, 1, creature->GetGUID());
}

void GuildLevelSystem::ShowInvestedMenu(Player* player, Creature* creature, uint32 action, uint32 sender)
{
    ClearGossipMenuFor(player);

    uint32 criteriaID = action - GLS_GOSSIP_CHOOSE_INVEST;

    auto criteriaProgress = GetCriteriaProgress(player->GetGuildId(), true);
    if (!criteriaProgress)
        ABORT();
  
    uint32 itemID = criteriaProgress->GetCriteriaItemID(criteriaID, sender);
    if (!itemID)
        ABORT();

    auto itemLink = GetItemLink(itemID, player->GetSession()->GetSessionDbLocaleIndex());
    uint32 currentItems = criteriaProgress->GetItemCountProgress(criteriaID, sender);
    uint32 maxItems = criteriaProgress->GetMaxCriteriaItemCount(criteriaID, sender);
    uint32 playerItemCount = player->GetItemCount(itemID);    
    uint32 itemsNeed = maxItems - currentItems;
    uint32 itemCount = playerItemCount;

    if (itemCount > itemsNeed)
        itemCount = itemsNeed;

    AddGossipItemFor(player, 10, "####################", sender, action);
    AddGossipItemFor(player, 10, warhead::StringFormat("# %s", itemLink.c_str()), sender, action);
    AddGossipItemFor(player, 10, warhead::StringFormat("# Вложений - %u/%u", currentItems, maxItems), sender, action);
    AddGossipItemFor(player, 10, warhead::StringFormat("# Осталось вложить - %u", itemsNeed), sender, action);
    AddGossipItemFor(player, 10, "####################", sender, action);
    AddGossipItemFor(player, 10, warhead::StringFormat("- В инвентаре - %u", playerItemCount), sender, action);

    if (itemsNeed)
    {
        AddGossipItemFor(player, 10, "- Вложить выборочно", sender, action, "Введите количество", 0, true);
        AddGossipItemFor(player, 10, warhead::StringFormat("- Вложить %s (%u)", itemCount == itemsNeed ? "до конца" : "всё что есть", itemCount), sender, criteriaID + GLS_GOSSIP_FULL_INVEST,
            warhead::StringFormat("Вы уверены, что хотите вложить %s %u?", itemLink.c_str(), playerItemCount), 0, false);
    }
    
    AddGossipItemFor(player, 10, ">> К списку критериев", GOSSIP_SENDER_MAIN, 2);
    AddGossipItemFor(player, 10, ">> В главное меню", GOSSIP_SENDER_MAIN, 99);
    SendGossipMenuFor(player, 1, creature->GetGUID());
}

void GuildLevelSystem::AddEmptyGuildCriteria(uint32 guildID)
{
    _guildCriteriaProgress.insert(std::make_pair(guildID, new GuildCriteria(guildID)));
}

void GuildLevelSystem::RescaleCriterias(uint32 guildID)
{
    GetCriteriaProgress(guildID, true)->RescalingCriterias();
}
