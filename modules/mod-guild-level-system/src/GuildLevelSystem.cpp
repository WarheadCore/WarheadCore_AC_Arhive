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
#include "ExternalMail.h"

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

uint32 GuildCriteria::GetCountProgressDone(uint32 criteriaID)
{
    uint32 count = 0;

    auto const& criteriaProgress = _guildCriteriaProgress.find(criteriaID);
    if (criteriaProgress == _guildCriteriaProgress.end())
        return count;

    for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
        if (criteriaProgress->second.ItemCount[i] == GetMaxCriteriaItemCount(criteriaID, i))
            count++;

    return count;
}

uint32 GuildCriteria::GetMaxCountProgressDone(uint32 criteriaID)
{
    uint32 count = 0;

    auto const& criteriaProgress = _guildCriteria.find(criteriaID);
    if (criteriaProgress == _guildCriteria.end())
        return count;

    for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
        if (criteriaProgress->second.ItemCount[i] && criteriaProgress->second.ItemID[i])
            count++;

    return count;
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

    LOG_INFO("modules.gls", "> Loading gls criteria");
    LoadBaseCriteria();

    LOG_INFO("modules.gls", "> Loading gls criteria progress");
    LoadCriteriaProgress();

    //LOG_INFO("modules", "");
}

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

void GuildLevelSystem::SendGuildMessage(uint32 guildID, std::string&& message)
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

        if (player->GetGuildId() != guildID)
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
    if (!itemTemplate)
        return "";

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
        break;
    }

    return warhead::StringFormat("|%s|Hitem:%u:0:0:0:0:0:0:0:0|h[%s]|h|r", color.c_str(), itemid, name.c_str());
}

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

void GuildLevelSystem::InvestItem(Player* player, Creature* creature, uint32 sender, uint32 action, uint32 itemCount)
{
    uint32 criteriaID = action - GLS_GOSSIP_CRITERIA_ID, itemType = sender;

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
    SendGuildFormat(guildID, "|cffff0000#|r %s |cff6C8CD5вложил|r %u %s. |cff6C8CD5Осталось -|r %u", handler.playerLink(player->GetName()).c_str(), itemCount, ItemLink.c_str(), needItems - itemCount);

    if (criteriaProgress->GetCountProgressDone(criteriaID) == criteriaProgress->GetMaxCountProgressDone(criteriaID))
        SendGuildFormat(player->GetGuildId(), "|cffff0000#|r |cff6C8CD5Гильдия выполнила критерий|r %u", criteriaID);

    ShowInvestedMenu(player, creature, action, sender);
}

void GuildLevelSystem::InvestItemFull(Player* player, Creature* creature, uint32 sender, uint32 action)
{
    uint32 criteriaID = action - GLS_GOSSIP_CRITERIA_ID_FULL, itemType = sender;

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
    SendGuildFormat(guildID, "|cffff0000#|r %s |cff6C8CD5вложил|r %u %s. |cff6C8CD5Осталось -|r %u", handler.playerLink(player->GetName()).c_str(), itemCount, ItemLink.c_str(), needItems - itemCount);
    ShowInvestedMenu(player, creature, criteriaID + GLS_GOSSIP_CRITERIA_ID, sender);
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

    ABORT_MSG("> GLS: Not found base criteria (%u)", criteriaID);

    return nullptr;
}

bool GuildLevelSystem::IsExistRewardItemsChoose(uint32 criteriaID)
{
    auto criteria = GetCriteria(criteriaID);

    for (uint32 i = 0; i < GLS_ITEMS_REWARD_CHOOSE_COUNT; ++i)
        if (criteria->RewardChooseItemID[i] && criteria->RewardChooseItemCount[i])
            return true;

    return false;
}

bool GuildLevelSystem::IsExistRewardItems(uint32 criteriaID)
{
    auto criteria = GetCriteria(criteriaID);

    if (criteria->RewardItemID && criteria->RewardItemCount)
        return true;

    return false;
}

void GuildLevelSystem::ShowAllCriteriaInfo(Player* player, Creature* creature)
{
    auto criteriaProgress = GetCriteriaProgress(player->GetGuildId(), true);
    if (!criteriaProgress)
        ABORT();

    AddGossipItemFor(player, 10, "Прогресс критерий:", GOSSIP_SENDER_MAIN, 2);

    for (auto const& itr : _guildCriteriaBase)
    {
        uint32 criteriaID = itr.first;
        uint32 gossipAction = criteriaID + GLS_GOSSIP_CRITERIA_ID;
        uint32 countNow = criteriaProgress->GetCountProgressDone(criteriaID);
        uint32 countMax = criteriaProgress->GetMaxCountProgressDone(criteriaID);

        AddGossipItemFor(player, 10, warhead::StringFormat("--- Критерий #%u. Прогресс (%u/%u)", criteriaID, countNow, countMax), GLS_GOSSIP_SHOW_CRITERIA_SENDER, gossipAction);

        for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
        {
            auto itemLink = GetItemLink(itr.second.ItemID[i]);
            auto currentItems = criteriaProgress->GetItemCountProgress(criteriaID, i);
            auto maxItems = criteriaProgress->GetMaxCriteriaItemCount(criteriaID, i);
            float persents = float(currentItems) / float(maxItems) * 100.0f;

            if (!itemLink.empty())
                AddGossipItemFor(player, 10, warhead::StringFormat("- %u. %s - %u/%u", i + 1, itemLink.c_str(), currentItems, maxItems), i, gossipAction);
        }
    }
    
    AddGossipItemFor(player, 10, ">> В главное меню", GOSSIP_SENDER_MAIN, 99);
    SendGossipMenuFor(player, 1, creature->GetGUID());
}

void GuildLevelSystem::ShowCriteriaInfo(Player* player, Creature* creature, uint32 sender, uint32 action)
{
    auto criteriaProgress = GetCriteriaProgress(player->GetGuildId(), true);
    if (!criteriaProgress)
        ABORT();

    uint32 criteriaID = action - GLS_GOSSIP_CRITERIA_ID;

    AddGossipItemFor(player, 10, "#", sender, action);
    AddGossipItemFor(player, 10, warhead::StringFormat("# Детальная информация о критерии %u", criteriaID), sender, action);
    AddGossipItemFor(player, 10, "#", sender, action);
    AddGossipItemFor(player, 10, "# Требования:", sender, action);

    for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
    {
        auto itemLink = GetItemLink(criteriaProgress->GetCriteriaItemID(criteriaID, i));
        auto currentItems = criteriaProgress->GetItemCountProgress(criteriaID, i);
        auto maxItems = criteriaProgress->GetMaxCriteriaItemCount(criteriaID, i);
        float persents = float(currentItems) / float(maxItems) * 100.0f;

        if (!itemLink.empty())
            AddGossipItemFor(player, 10, warhead::StringFormat("- %u. %s - %u/%u", i + 1, itemLink.c_str(), currentItems, maxItems), i, action);
    }

    auto criteria = GetCriteria(criteriaID);
    if (!criteria)
        ABORT();

    if (IsExistRewardItemsChoose(criteriaID))
    {
        AddGossipItemFor(player, 10, "#", sender, action);
        AddGossipItemFor(player, 10, "# Награды за выполнение на выбор:", sender, action);

        for (uint32 i = 0; i < GLS_ITEMS_REWARD_CHOOSE_COUNT; ++i)
        {
            auto itemLink = GetItemLink(criteria->RewardChooseItemID[i]);
            auto itemCount = criteria->RewardChooseItemCount[i];

            if (!itemLink.empty())
                AddGossipItemFor(player, 10, warhead::StringFormat("# %u. %s x%u", i + 1, itemLink.c_str(), itemCount), sender, action);
        }
    }

    if (IsExistRewardItems(criteriaID))
    {
        auto itemLink = GetItemLink(criteria->RewardItemID);
        auto itemCount = criteria->RewardItemCount;

        AddGossipItemFor(player, 10, "#", sender, action);
        AddGossipItemFor(player, 10, "# Обязательные награды:", sender, action);

        if (!itemLink.empty())
            AddGossipItemFor(player, 10, warhead::StringFormat("# %s x%u", itemLink.c_str(), itemCount), sender, action);
    }

    AddGossipItemFor(player, 10, "#", sender, action);

    // Get reward if progress is done
    if (criteriaProgress->GetCountProgressDone(criteriaID) == criteriaProgress->GetMaxCountProgressDone(criteriaID))
    {
        if (player->GetRank() == 0)
            AddGossipItemFor(player, 10, ">> Получить награды", GLS_GOSSIP_SHOW_REWARDS_SENDER, action);
    }
    
    AddGossipItemFor(player, 10, ">> К списку критериев", GOSSIP_SENDER_MAIN, 2);
    AddGossipItemFor(player, 10, ">> В главное меню", GOSSIP_SENDER_MAIN, 99);
    SendGossipMenuFor(player, 1, creature->GetGUID());
}

void GuildLevelSystem::ShowRewardInfo(Player* player, Creature* creature, uint32 sender, uint32 action)
{
    auto criteriaProgress = GetCriteriaProgress(player->GetGuildId(), true);
    if (!criteriaProgress)
        ABORT();

    if (player->GetRank() != 0)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("Вы не глава гильдии!");
        return;
    }

    uint32 criteriaID = action - GLS_GOSSIP_CRITERIA_ID;

    AddGossipItemFor(player, 10, "#", sender, action);
    AddGossipItemFor(player, 10, warhead::StringFormat("# Получение награды за критерий %u", criteriaID), sender, action);
    AddGossipItemFor(player, 10, "#", sender, action);

    auto criteria = GetCriteria(criteriaID);
    if (!criteria)
        ABORT();

    uint32 selectShoose = 0;

    if (IsExistRewardItemsChoose(criteriaID))
    {
        AddGossipItemFor(player, 10, "# Выберите награду из списка:", sender, action);

        if (sender >= GLS_GOSSIP_GET_REWARDS_SENDER)
            selectShoose = sender - GLS_GOSSIP_GET_REWARDS_SENDER + 1;

        for (uint32 i = 0; i < GLS_ITEMS_REWARD_CHOOSE_COUNT; ++i)
        {
            auto itemLink = GetItemLink(criteria->RewardChooseItemID[i]);
            auto itemCount = criteria->RewardChooseItemCount[i];

            auto stringSelect = [](uint32 select, uint32 itemRewardNumber) -> std::string const
            {
                if (!select)
                    return "";

                if (select - 1 == itemRewardNumber)
                    return "[Выбрано] - ";

                return "";
            };

            if (!itemLink.empty())
                AddGossipItemFor(player, 10, warhead::StringFormat("# %u. %s %s x%u", i + 1, stringSelect(selectShoose, i).c_str(), itemLink.c_str(), itemCount), GLS_GOSSIP_GET_REWARDS_SENDER + i, action,
                    warhead::StringFormat("Вы уверены, что хотите выбрать %s x%u?", itemLink.c_str(), itemCount), 0, false);
        }

        AddGossipItemFor(player, 10, "#", sender, action);
    }

    if (IsExistRewardItems(criteriaID))
    {
        auto itemLink = GetItemLink(criteria->RewardItemID);
        auto itemCount = criteria->RewardItemCount;
        
        AddGossipItemFor(player, 10, "# Дополнительный награды:", sender, action);

        if (!itemLink.empty())
            AddGossipItemFor(player, 10, warhead::StringFormat("# %s x%u", itemLink.c_str(), itemCount), sender, action);
    }
    
    if (selectShoose)
        AddGossipItemFor(player, 10, "--- Получить награды", selectShoose + GLS_GOSSIP_GET_ALL_REWARDS_SENDER - 1, action);

    AddGossipItemFor(player, 10, "#", sender, action);
    AddGossipItemFor(player, 10, ">> К списку критериев", GOSSIP_SENDER_MAIN, 2);
    AddGossipItemFor(player, 10, ">> В главное меню", GOSSIP_SENDER_MAIN, 99);
    SendGossipMenuFor(player, 1, creature->GetGUID());
}

void GuildLevelSystem::GetRewardsCriteria(Player* player, Creature* creature, uint32 sender, uint32 action)
{
    uint32 criteriaID = action - GLS_GOSSIP_CRITERIA_ID;

    auto criteria = GetCriteria(criteriaID);
    if (!criteria)
        ABORT();

    if (player->GetRank() != 0)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("Вы не глава гильдии!");
        return;
    }

    uint32 itemType = sender - GLS_GOSSIP_GET_ALL_REWARDS_SENDER;

    if (itemType >= GLS_ITEMS_REWARD_CHOOSE_COUNT)
        ABORT();

    std::unordered_map<uint32, uint32> itemList;

    if (IsExistRewardItems(criteriaID))
        itemList.insert(std::make_pair(criteria->RewardItemID, criteria->RewardItemCount));

    if (IsExistRewardItemsChoose(criteriaID))
        itemList.insert(std::make_pair(criteria->RewardChooseItemID[itemType], criteria->RewardChooseItemCount[itemType]));

    auto tanksSub = warhead::StringFormat("Гильдия. Награда за выполенение критерия %u", criteriaID);
    auto tanksText = warhead::StringFormat("Поздравляем, ваша гильдия выполнила критерий (%u) и каждый из вас получает награду.", criteriaID);

    for (auto const& itr : player->GetGuild()->GetAllMembers())
    {
        auto const& member = itr.second;

        for (auto const& mailItems : itemList)
            sEM->AddMail(member->GetName(), tanksSub, tanksText, mailItems.first, mailItems.second, 37688);
    }

    SendGuildFormat(player->GetGuildId(), "|cffff0000#|r |cff6C8CD5Награда за критерий|r %u |cff6C8CD5отправлена на почту", criteriaID);
}

void GuildLevelSystem::ShowInvestedMenu(Player* player, Creature* creature, uint32 sender, uint32 action)
{
    ClearGossipMenuFor(player);

    uint32 criteriaID = action - GLS_GOSSIP_CRITERIA_ID;

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
    float persents = float(currentItems) / float(maxItems) * 100.0f;

    if (itemCount > itemsNeed)
        itemCount = itemsNeed;

    AddGossipItemFor(player, 10, "#", sender, action);
    AddGossipItemFor(player, 10, warhead::StringFormat("# %s", itemLink.c_str()), sender, action);
    AddGossipItemFor(player, 10, warhead::StringFormat("# Вложений - %u/%u", currentItems, maxItems), sender, action);
    AddGossipItemFor(player, 10, warhead::StringFormat("# Осталось вложить - %u", itemsNeed), sender, action);
    AddGossipItemFor(player, 10, warhead::StringFormat("# Прогресс выполнения - %0.2f%%", persents), sender, action);
    AddGossipItemFor(player, 10, "#", sender, action);
    AddGossipItemFor(player, 10, warhead::StringFormat("- В инвентаре - %u", playerItemCount), sender, action);

    if (itemsNeed)
    {
        AddGossipItemFor(player, 10, "- Вложить выборочно", sender, action, "Введите количество", 0, true);
        AddGossipItemFor(player, 10, warhead::StringFormat("- Вложить %s (%u)", itemCount == itemsNeed ? "до конца" : "всё что есть", itemCount), sender, criteriaID + GLS_GOSSIP_CRITERIA_ID_FULL,
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
