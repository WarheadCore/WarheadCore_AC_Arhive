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
#include "SpellMgr.h"
#include "SpellInfo.h"
#include "ExternalMail.h"
#include "StringConvert.h"
#include "Tokenize.h"

namespace
{
    void EraseWhitespace(std::string& str)
    {
        str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    }
}

#define GLS_IRERATOR(a) for (uint32 i = 0; i < a; ++i)

GuildCriteria::GuildCriteria(uint32 guildID, uint32 stageID)
    : _guildID(guildID), _stageID(stageID)
{
    AddBaseCriterias();
    RescalingCriterias();
}

GuildCriteriaProgressStruct* GuildCriteria::GetProgress(uint32 criteriaID, bool forceCreate /*= false*/)
{
    auto const& progress = _guildCriteriaProgress.find(criteriaID);
    if (progress != _guildCriteriaProgress.end())
        return &progress->second;

    if (forceCreate)
    {
        AddEmptyProgress(criteriaID);

        auto const& _progress = _guildCriteriaProgress.find(criteriaID);
        if (_progress != _guildCriteriaProgress.end())
            return &_progress->second;

        LOG_FATAL("module.gls", "> GLS: Not found criteria progress (%u) after insert empty", criteriaID);
    }

    return nullptr;
}

GuildCriteriaStruct* GuildCriteria::GetCriteria(uint32 criteriaID)
{
    auto const& criteria = _guildCriteria.find(criteriaID);
    if (criteria == _guildCriteria.end())
        LOG_FATAL("module.gls", "> GLS: Not found criteria (%u) for guild id (%u)", criteriaID, _guildID);

    return &criteria->second;
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
        LOG_FATAL("module.gls", "> GLS: No members count in guild (%u)", _guildID);

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
    _data.SelectedSpell = 0;
    _data.IsDone = false;

    for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
        _data.ItemCount[i] = 0;

    AddProgress(criteriaID, _data);
}

uint32 GuildCriteria::GetMaxCriteriaItemCount(uint32 criteriaID, uint8 itemType)
{
    return GetCriteria(criteriaID)->ItemCount[itemType];
}

uint32 GuildCriteria::GetItemCountProgress(uint32 criteriaID, uint8 itemType)
{
    return GetProgress(criteriaID, true)->ItemCount[itemType];
}

uint32 GuildCriteria::GetCountProgressDone(uint32 criteriaID)
{
    uint32 count = 0;

    auto const& criteriaProgress = GetProgress(criteriaID);
    if (!criteriaProgress)
        return count;

    for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
        if (criteriaProgress->ItemCount[i] == GetMaxCriteriaItemCount(criteriaID, i))
            count++;

    return count;
}

uint32 GuildCriteria::GetMaxCountProgressDone(uint32 criteriaID)
{
    uint32 count = 0;

    auto const& criteriaProgress = GetCriteria(criteriaID);
    if (!criteriaProgress)
        return count;

    for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
        if (criteriaProgress->ItemCount[i] && criteriaProgress->ItemID[i])
            count++;

    return count;
}

void GuildCriteria::SetSelectedSpell(uint32 criteriaID, uint32 spellID)
{
    if (auto progress = GetProgress(criteriaID))
        progress->SelectedSpell = spellID;
}

void GuildCriteria::AddItemProgess(uint32 criteriaID, uint8 itemType, uint32 itemCount)
{
    auto progress = GetProgress(criteriaID, true);
    if (!progress)
        return;

    // Add progress count
    progress->ItemCount[itemType] += itemCount;

    // Save in DB
    SaveToDB(criteriaID);
}

uint32 GuildCriteria::GetCriteriaItemID(uint32 criteriaID, uint8 itemType)
{
    auto criteria = GetCriteria(criteriaID);
    if (!criteria)
        return 0;

    return criteria->ItemID[itemType];
}

uint32 GuildCriteria::GetRewardSpellID(uint32 criteriaID, uint8 spellType)
{
    auto criteria = GetCriteria(criteriaID);
    if (!criteria)
        return 0;

    return criteria->RewardSpells[spellType];
}

void GuildCriteria::SaveToDB(uint32 criteriaID)
{
    auto const& progressStruct = GetProgress(criteriaID);
    std::string itemsCount;

    for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
        itemsCount += Warhead::ToString(progressStruct->ItemCount[i]) + ",";

    // Delete last (,)
    if (!itemsCount.empty())
        itemsCount.erase(itemsCount.end() - 1, itemsCount.end());

    auto trans = CharacterDatabase.BeginTransaction();

    // gls_criteria_progress
    trans->PAppend("DELETE FROM `gls_criteria_progress` WHERE `GuildID` = %u AND `CriteriaID` = %u", progressStruct->GuildID, progressStruct->CriteriaID);
    trans->PAppend("INSERT INTO `gls_criteria_progress`(`GuildID`, `CriteriaID`, `ItemCount`, `SelectedSpell`, `IsDone`) VALUES (%u, %u, '%s', %u, %d)",
                   _guildID, progressStruct->CriteriaID, itemsCount.c_str(), progressStruct->SelectedSpell, progressStruct->IsDone);

    CharacterDatabase.CommitTransaction(trans);
}

void GuildCriteria::SetProgressDone(uint32 criteriaID, bool isDone /*= true*/)
{
    GetProgress(criteriaID, true)->IsDone = isDone;
}

bool GuildCriteria::IsProgressDone(uint32 criteriaID)
{
    auto criteriaProgress = GetProgress(criteriaID);
    if (!criteriaProgress)
        return false;

    return criteriaProgress->IsDone;
}

uint32 GuildCriteria::GetCountCriteriaProgressDone()
{
    uint32 count = 0;

    auto const& stagesAll = sGuildLevelSystem->GetStages();
    auto const& stages = stagesAll.find(_stageID);

    if (stages == stagesAll.end())
        return count;

    for (auto const& itr : stages->second)
    {
        auto criteriaProgress = GetProgress(itr);
        if (criteriaProgress && criteriaProgress->IsDone)
            count ++;
    }

    return count;
}

void GuildCriteria::UnLearnSpells(uint32 lowGuid)
{
    auto trans = CharacterDatabase.BeginTransaction();
    Player* player = ObjectAccessor::FindPlayerInOrOutOfWorld(lowGuid);

    for (auto const& itr : _guildCriteriaProgress)
    {
        uint32 spellID = itr.second.SelectedSpell;

        if (player && player->HasSpell(spellID))
            player->removeSpell(spellID, SPEC_MASK_ALL, false);

        if (!player)
            trans->PAppend("DELETE FROM `character_spell` WHERE `guid` = %u AND `spell` = %u", lowGuid, spellID);
    }

    CharacterDatabase.CommitTransaction(trans);
}

void GuildCriteria::LearnSpells(Player* player)
{
    for (auto const& itr : _guildCriteriaProgress)
    {
        uint32 spellID = itr.second.SelectedSpell;

        if (!player->HasSpell(spellID))
            player->learnSpell(spellID);
    }
}

void GuildCriteria::SetStageID(uint32 stage, bool saveDB /*= true*/)
{
    _stageID = stage;

    if (!saveDB)
        return;

    auto trans = CharacterDatabase.BeginTransaction();

    // gls_stages_progress
    trans->PAppend("DELETE FROM `gls_stages_progress` WHERE `GuildID` = %u", _guildID);
    trans->PAppend("INSERT INTO `gls_stages_progress`(`GuildID`, `StageID`) VALUES (%u, %u)", _guildID, _stageID);

    CharacterDatabase.CommitTransaction(trans);
}

GuildLevelSystem* GuildLevelSystem::instance()
{
    static GuildLevelSystem instance;
    return &instance;
}

void GuildLevelSystem::Init()
{
    LOG_INFO("module.gls", "Loading Guild Level System");

    LoadBaseCriteria();
    LoadCriteriaProgress();
    LoadStageProgress();

    LOG_INFO("module.gls", "");
}

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

// Load data from DB
void GuildLevelSystem::LoadBaseCriteria()
{
    uint32 oldMSTime = getMSTime();

    _guildCriteriaBase.clear();
    _guildStages.clear();

    QueryResult result = CharacterDatabase.Query("SELECT "
                         "`CriteriaID`,"                 // 0
                         "`StageID`,"                    // 1
                         "`ListItemID`,"                 // 2
                         "`ListItemCount`,"              // 3
                         "`MinPlayersCount`,"            // 4
                         "`Coef`,"                       // 5
                         "`ListRewardSpells` "           // 6
                         "FROM `gls_criteria`");

    if (!result)
    {
        LOG_WARN("module.gls", "> Загружено 0 GLS критерий. Таблица `gls_criteria` пустая.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        auto CheckItemIDs = [](uint32 itemID) -> bool
        {
            if (!itemID)
                return true;

            ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemID);
            if (!itemTemplate)
            {
                LOG_ERROR("module.gls", "> GLS: ItemID (%u) for GLS criteria is not valid.", itemID);
                return false;
            }

            return true;
        };

        uint32 criteriaID                       = fields[0].GetUInt32();
        uint32 stageID                          = fields[1].GetUInt32();
        std::string listItemID                  = fields[2].GetString();
        std::string listItemCount               = fields[3].GetString();
        uint32 minPlayersCount                  = fields[4].GetUInt32();
        float coef                              = fields[5].GetFloat();
        std::string listRewardSpells            = fields[6].GetString();

        GuildCriteriaStruct _data;
        _data.CriteriaID        = criteriaID;
        _data.StageID           = stageID;
        _data.MinPlayersCount   = minPlayersCount;
        _data.Coef              = coef;

        // Set empty items
        GLS_IRERATOR(GLS_ITEMS_COUNT)
        {
            _data.ItemID[i] = 0;
            _data.ItemCount[i] = 0;
        }

        // Set empty spells
        GLS_IRERATOR(GLS_SPELLS_REWARD_COUNT)
        _data.RewardSpells[i] = 0;

        // Delele whitespace
        ::EraseWhitespace(listItemID);
        ::EraseWhitespace(listItemCount);
        ::EraseWhitespace(listRewardSpells);

        auto const& listItemIDTokens = Warhead::Tokenize(listItemID, ',', true);
        auto const& listItemCountTokens = Warhead::Tokenize(listItemCount, ',', true);

        if (static_cast<uint32>(listItemIDTokens.size()) > GLS_ITEMS_COUNT)
        {
            LOG_FATAL("module.gls", "> GLS: List items for CriteriaID (%u) > %u", criteriaID, GLS_ITEMS_COUNT);
            continue;
        }

        if (static_cast<uint32>(listItemCountTokens.size()) > GLS_ITEMS_COUNT)
        {
            LOG_FATAL("module.gls", "> GLS: List items count for CriteriaID (%u) > %u", criteriaID, GLS_ITEMS_COUNT);
            continue;
        }

        if (listItemIDTokens.size() != listItemCountTokens.size())
        {
            LOG_FATAL("module.gls", "> GLS: Differenst size data between `ListItemID` and `ListItemCount`");
            continue;
        }

        GLS_IRERATOR(static_cast<uint32>(listItemIDTokens.size()))
        {
            uint32 itemID = Warhead::StringTo<uint32>(listItemIDTokens[i]).value_or(0);

            if (!CheckItemIDs(itemID))
                itemID = 0;

            _data.ItemID[i] = itemID;
        }

        for (uint32 i = 0; i < static_cast<uint32>(listItemCountTokens.size()); ++i)
            _data.ItemCount[i] = Warhead::StringTo<uint32>(listItemCountTokens[i]).value_or(0);

        // Reward spells
        if (listRewardSpells.empty())
        {
            LOG_ERROR("module.gls", "> GLS: Not exist reward spells for criteria id (%u). Skip", criteriaID);
            continue;
        }

        auto const& listRewardSpellsTokens = Warhead::Tokenize(listRewardSpells, ',', true);

        if (static_cast<uint32>(listRewardSpellsTokens.size()) > GLS_SPELLS_REWARD_COUNT)
        {
            LOG_FATAL("module.gls", "> GLS: List reward spells for CriteriaID (%u) > %u", criteriaID, GLS_SPELLS_REWARD_COUNT);
            continue;
        }

        GLS_IRERATOR(static_cast<uint32>(listRewardSpellsTokens.size()))
        {
            uint32 spellID = Warhead::StringTo<uint32>(listRewardSpellsTokens[i]).value_or(0);

            if (!sSpellMgr->GetSpellInfo(spellID))
            {
                LOG_ERROR("module.gls", "> GLS: Spell id (%u) is not exist! Skip.", spellID);
                spellID = 0;
            }

            _data.RewardSpells[i] = spellID;
        }

        _guildCriteriaBase.insert(std::make_pair(criteriaID, _data));

        auto const& _stages = _guildStages.find(stageID);
        if (_stages == _guildStages.end())
        {
            std::vector<uint32> __vector = { criteriaID };
            _guildStages.insert(std::make_pair(stageID, __vector));
        }
        else
            _stages->second.push_back(criteriaID);

    } while (result->NextRow());

    LOG_INFO("module.gls", ">> Loading %u GLS criteria in %u мс", static_cast<uint32>(_guildCriteriaBase.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GuildLevelSystem::LoadCriteriaProgress()
{
    uint32 oldMSTime = getMSTime();

    _guildCriteriaProgress.clear();

    QueryResult result = CharacterDatabase.Query("SELECT "
                         "`GuildID`,"        // 0
                         "`CriteriaID`,"     // 1
                         "`ItemCount`,"      // 2
                         "`SelectedSpell`,"  // 3
                         "`IsDone` "         // 4
                         "FROM `gls_criteria_progress`");

    if (!result)
    {
        LOG_WARN("module.gls", "> Загружено 0 GLS критерий прогресса. Таблица `gls_criteria_progress` пустая.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 guildID              = fields[0].GetUInt32();
        uint32 criteriaID           = fields[1].GetUInt32();
        std::string listItemCount   = fields[2].GetString();
        uint32 spellID              = fields[3].GetUInt32();
        bool isDone                 = fields[4].GetBool();

        // Check guild
        if (!sGuildMgr->GetGuildById(guildID))
        {
            LOG_ERROR("module.gls", "> GLS: guild id (%u) is not exist!. Skip", guildID);
            continue;
        }

        GuildCriteriaProgressStruct _data;
        _data.GuildID       = guildID;
        _data.CriteriaID    = criteriaID;
        _data.SelectedSpell = spellID;
        _data.IsDone        = isDone;

        // Set empty items
        GLS_IRERATOR(GLS_ITEMS_COUNT)
        _data.ItemCount[i] = 0;

        // Delete whilespace
        ::EraseWhitespace(listItemCount);

        auto const& listItemCountTokens = Warhead::Tokenize(listItemCount, ',', true);

        if (static_cast<uint32>(listItemCountTokens.size()) > GLS_ITEMS_COUNT)
        {
            LOG_FATAL("module.gls", "> GLS: List items count for CriteriaID (%u) > %u", criteriaID, GLS_ITEMS_COUNT);
            continue;
        }

        // Check spell
        if (spellID && !sSpellMgr->GetSpellInfo(spellID))
        {
            LOG_FATAL("module.gls", "> GLS: spell (%u) for CriteriaID (%u) is invalid", spellID, criteriaID);
            continue;
        }

        // Check wtf logic
        if (!spellID && isDone)
        {
            LOG_FATAL("module.gls", "> GLS: CriteriaID (%u) is done and spell not selectable!", criteriaID);
            continue;
        }

        for (uint32 i = 0; i < static_cast<uint32>(listItemCountTokens.size()); ++i)
            _data.ItemCount[i] = Warhead::StringTo<uint32>(listItemCountTokens[i]).value_or(0);

        auto criteria = GetCriteriaProgress(guildID);
        if (!criteria)
            criteria = new GuildCriteria(guildID, 1);

        criteria->AddProgress(criteriaID, _data);

        _guildCriteriaProgress.insert(std::make_pair(guildID, criteria));

        count++;
    } while (result->NextRow());

    LOG_INFO("module.gls", ">> Загружено %u GLS критерий прогресса за %u мс", count, GetMSTimeDiffToNow(oldMSTime));
}

void GuildLevelSystem::LoadStageProgress()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = CharacterDatabase.Query("SELECT "
                         "`GuildID`,"     // 0
                         "`StageID` "     // 1
                         "FROM `gls_stages_progress`");

    if (!result)
    {
        LOG_WARN("module.gls", "> Загружено 0 GLS стадий прогресса. Таблица `gls_stages_progress` пустая.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 guildID = fields[0].GetUInt32();
        uint32 stageID = fields[1].GetUInt32();

        // Check guild
        if (!sGuildMgr->GetGuildById(guildID))
        {
            LOG_ERROR("module.gls", "> GLS: guild id (%u) is not exist!. Skip", guildID);
            continue;
        }

        auto criteriaProgress = GetCriteriaProgress(guildID);
        if (!criteriaProgress)
        {
            LOG_ERROR("module.gls", "> GLS: guild id (%u) is not exist in criteria progress!. Skip", guildID);
            continue;
        }

        criteriaProgress->SetStageID(stageID, false);

        count++;
    } while (result->NextRow());

    LOG_INFO("module.gls", ">> Загружено %u GLS стадий прогресса за %u мс", count, GetMSTimeDiffToNow(oldMSTime));
}

// Gossip
void GuildLevelSystem::InvestItem(Player* player, Creature* creature, uint32 sender, uint32 action, uint32 itemCount)
{
    uint32 criteriaID = action - GLS_GOSSIP_CRITERIA_ID, itemType = sender;

    if (itemType > GLS_ITEMS_COUNT)
    {
        LOG_ERROR("module.gls", "> GLS: invalid item type (%u)", itemType);
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
        return;

    uint32 itemID = criteriaProgress->GetCriteriaItemID(criteriaID, itemType);
    if (!itemID)
    {
        LOG_ERROR("module.gls", "> GLS: invalid item id at invest (%u-%u)", criteriaID, itemType);
        handler.PSendSysMessage("> Неверный номер предмета (%u-%u)", criteriaID, itemType);
    }

    int8 Locale_index = handler.GetSessionDbLocaleIndex();
    std::string const& ItemLink = sGameLocale->GetItemLink(itemID, Locale_index);
    uint32 hasItemCount = player->GetItemCount(itemID);

    if (!player->HasItemCount(itemID, itemCount))
    {
        handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вам не хватает|r %s - %u", ItemLink.c_str(), itemCount - hasItemCount);
        ShowInvestedMenu(player, creature, sender, action);
        return;
    }

    uint32 maxItems = criteriaProgress->GetMaxCriteriaItemCount(criteriaID, itemType);
    uint32 currentItems = criteriaProgress->GetItemCountProgress(criteriaID, itemType);
    uint32 needItems = maxItems - currentItems;

    if (itemCount > needItems)
    {
        handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вы хотите вложить|r %s |cff6C8CD5больше, чем можно|r %u|cff6C8CD5. Введите корректное число|r", ItemLink.c_str(), needItems);
        ShowInvestedMenu(player, creature, sender, action);
        return;
    }

    player->DestroyItemCount(itemID, itemCount, true);
    criteriaProgress->AddItemProgess(criteriaID, itemType, itemCount);
    SendGuildFormat(guildID, "|cffff0000#|r %s |cff6C8CD5вложил|r %u %s. |cff6C8CD5Осталось -|r %u", handler.playerLink(player->GetName()).c_str(), itemCount, ItemLink.c_str(), needItems - itemCount);

    if (criteriaProgress->GetCountProgressDone(criteriaID) == criteriaProgress->GetMaxCountProgressDone(criteriaID))
        SendGuildFormat(player->GetGuildId(), "|cffff0000#|r |cff6C8CD5Гильдия выполнила критерий|r %u", criteriaID);

    ShowInvestedMenu(player, creature, sender, action);
}

void GuildLevelSystem::InvestItemFull(Player* player, Creature* creature, uint32 sender, uint32 action)
{
    uint32 criteriaID = action - GLS_GOSSIP_CRITERIA_ID_FULL, itemType = sender;

    if (itemType > GLS_ITEMS_COUNT)
    {
        LOG_ERROR("module.gls", "> GLS: invalid item type (%u)", itemType);
        ShowInvestedMenu(player, creature, sender, action);
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
        return;

    uint32 itemID = criteriaProgress->GetCriteriaItemID(criteriaID, itemType);
    if (!itemID)
    {
        LOG_ERROR("module.gls", "> GLS: invalid item id at invest (%u-%u)", criteriaID, itemType);
        handler.PSendSysMessage("> Неверный номер предмета (%u-%u)", criteriaID, itemType);
    }

    auto ItemLink = sGameLocale->GetItemLink(itemID, handler.GetSessionDbLocaleIndex());
    uint32 hasItemCount = player->GetItemCount(itemID);
    uint32 itemCount = hasItemCount;

    if (!itemCount)
    {
        handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 У вас нет|r %s", ItemLink.c_str());
        ShowInvestedMenu(player, creature, sender, action);
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
    ShowInvestedMenu(player, creature, sender, criteriaID + GLS_GOSSIP_CRITERIA_ID);
}

GuildCriteria* GuildLevelSystem::GetCriteriaProgress(uint32 guildid, bool forceCreate /*= false*/)
{
    auto const& itr = _guildCriteriaProgress.find(guildid);
    if (itr != _guildCriteriaProgress.end())
        return itr->second;

    if (forceCreate)
    {
        AddEmptyGuildCriteria(guildid);

        auto const& _itr = _guildCriteriaProgress.find(guildid);
        if (_itr != _guildCriteriaProgress.end())
            return _itr->second;

        LOG_FATAL("module.gls", "> GLS: Invalid create empty guild criteria for guild (%u)", guildid);
    }

    return nullptr;
}

GuildCriteriaStruct* GuildLevelSystem::GetCriteriaBase(uint32 criteriaID)
{
    auto const& itr = _guildCriteriaBase.find(criteriaID);
    if (itr != _guildCriteriaBase.end())
        return &itr->second;

    LOG_FATAL("module.gls", "> GLS: Not found base criteria (%u)", criteriaID);

    return nullptr;
}

uint32 GuildLevelSystem::GetMaxCriteriaItemCountBase(uint32 criteriaID, uint8 itemType)
{
    auto const& itr = _guildCriteriaBase.find(criteriaID);
    if (itr != _guildCriteriaBase.end())
        return itr->second.ItemCount[itemType];

    LOG_FATAL("module.gls", "> GLS: Not found base criteria (%u)", criteriaID);

    return 0;
}

void GuildLevelSystem::ShowAllCriteriaInfo(Player* player, Creature* creature)
{
    auto criteriaProgress = GetCriteriaProgress(player->GetGuildId(), true);
    if (!criteriaProgress)
        return;

    uint32 stageID = criteriaProgress->GetStageID();

    AddGossipItemFor(player, 10, Warhead::StringFormat("# Текущая стадия: %u", stageID), GOSSIP_SENDER_MAIN, 2);

    uint32 countDone = criteriaProgress->GetCountCriteriaProgressDone();

    if (GetCountCriteriaInStage(stageID) != countDone)
    {
        AddGossipItemFor(player, 10, "# Прогресс критерий:", GOSSIP_SENDER_MAIN, 2);
        AddGossipItemFor(player, 10, "#", GOSSIP_SENDER_MAIN, 2);

        for (auto const& itr : _guildCriteriaBase)
        {
            uint32 criteriaStage = itr.second.StageID;

            if (criteriaStage != stageID)
                continue;

            uint32 criteriaID = itr.first;
            uint32 gossipAction = criteriaID + GLS_GOSSIP_CRITERIA_ID;
            uint32 countNow = criteriaProgress->GetCountProgressDone(criteriaID);
            uint32 countMax = criteriaProgress->GetMaxCountProgressDone(criteriaID);

            if (criteriaProgress->IsProgressDone(criteriaID))
                continue;

            AddGossipItemFor(player, 10, Warhead::StringFormat("--- Критерий #%u. Прогресс (%u/%u)", criteriaID, countNow, countMax), GLS_GOSSIP_SHOW_CRITERIA_SENDER, gossipAction);

            if (CONF_GET_BOOL("GLS.Criteria.ShowItems.Enable"))
            {
                for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
                {
                    uint32 itemID = itr.second.ItemID[i];
                    if (!itemID)
                        continue;

                    auto itemLink = sGameLocale->GetItemLink(itemID);
                    auto currentItems = criteriaProgress->GetItemCountProgress(criteriaID, i);
                    auto maxItems = criteriaProgress->GetMaxCriteriaItemCount(criteriaID, i);
                    float persents = float(currentItems) / float(maxItems) * 100.0f;

                    if (!itemLink.empty())
                        AddGossipItemFor(player, 10, Warhead::StringFormat("- %u. %s - %u/%u", i + 1, itemLink.c_str(), currentItems, maxItems), i, gossipAction);
                }
            }
        }
    }
    else
    {
        AddGossipItemFor(player, 10, "# Вы выполнили все возможные критерии", GOSSIP_SENDER_MAIN, 2);

        if (player->GetRank() == 0 && GetNextStage(stageID) != stageID)
            AddGossipItemFor(player, 10, Warhead::StringFormat("# >> Завершить стадию - %u", countDone), GOSSIP_SENDER_MAIN, 4);
    }

    AddGossipItemFor(player, 10, ">> В главное меню", GOSSIP_SENDER_MAIN, 99);
    SendGossipMenuFor(player, 1, creature->GetGUID());
}

void GuildLevelSystem::ShowCriteriaInfo(Player* player, Creature* creature, uint32 sender, uint32 action)
{
    auto criteriaProgress = GetCriteriaProgress(player->GetGuildId(), true);
    if (!criteriaProgress)
        return;

    uint32 criteriaID = action - GLS_GOSSIP_CRITERIA_ID;

    AddGossipItemFor(player, 10, "#", sender, action);
    AddGossipItemFor(player, 10, Warhead::StringFormat("# Детальная информация о критерии %u", criteriaID), sender, action);
    AddGossipItemFor(player, 10, "#", sender, action);
    AddGossipItemFor(player, 10, "# Требования:", sender, action);

    for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
    {
        uint32 itemID = criteriaProgress->GetCriteriaItemID(criteriaID, i);
        if (!itemID)
            continue;

        auto itemLink = sGameLocale->GetItemLink(itemID);
        auto currentItems = criteriaProgress->GetItemCountProgress(criteriaID, i);
        auto maxItems = criteriaProgress->GetMaxCriteriaItemCount(criteriaID, i);
        float persents = float(currentItems) / float(maxItems) * 100.0f;

        if (!itemLink.empty())
            AddGossipItemFor(player, 10, Warhead::StringFormat("- %u. %s - %u/%u", i + 1, itemLink.c_str(), currentItems, maxItems), i, action);
    }

    AddGossipItemFor(player, 10, "#", sender, action);
    AddGossipItemFor(player, 10, "# Награды на выбор:", sender, action);

    for (uint32 i = 0; i < GLS_SPELLS_REWARD_COUNT; ++i)
    {
        uint32 spellID = criteriaProgress->GetRewardSpellID(criteriaID, i);
        if (!spellID)
            continue;

        AddGossipItemFor(player, 10, Warhead::StringFormat("# %u. %s", i + 1, sGameLocale->GetSpellNamelocale(spellID).c_str()), sender, action);
    }

    AddGossipItemFor(player, 10, "#", sender, action);

    // Get reward if progress is done
    if (criteriaProgress->GetCountProgressDone(criteriaID) == criteriaProgress->GetMaxCountProgressDone(criteriaID))
    {
        if (player->GetRank() == 0 && !criteriaProgress->IsProgressDone(criteriaID))
            AddGossipItemFor(player, 10, ">> Получить награды", GLS_GOSSIP_SHOW_REWARDS_SENDER, action);
    }

    AddGossipItemFor(player, 10, ">> Просмотр наград и требований", GLS_GOSSIP_GET_ALL_LINK, action);
    AddGossipItemFor(player, 10, ">> К списку критериев", GOSSIP_SENDER_MAIN, 2);
    AddGossipItemFor(player, 10, ">> В главное меню", GOSSIP_SENDER_MAIN, 99);
    SendGossipMenuFor(player, 1, creature->GetGUID());
}

void GuildLevelSystem::ShowRewardInfo(Player* player, Creature* creature, uint32 sender, uint32 action)
{
    auto criteriaProgress = GetCriteriaProgress(player->GetGuildId(), true);
    if (!criteriaProgress)
        return;

    if (player->GetRank() != 0)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("Вы не глава гильдии!");
        return;
    }

    uint32 criteriaID = action - GLS_GOSSIP_CRITERIA_ID;

    AddGossipItemFor(player, 10, "#", sender, action);
    AddGossipItemFor(player, 10, Warhead::StringFormat("# Получение награды за критерий %u", criteriaID), sender, action);
    AddGossipItemFor(player, 10, "#", sender, action);

    auto criteria = GetCriteriaBase(criteriaID);
    if (!criteria)
        return;

    uint32 selectShoose = 0;

    AddGossipItemFor(player, 10, "# Выберите награду из списка:", sender, action);

    if (sender >= GLS_GOSSIP_GET_REWARDS_SENDER)
        selectShoose = sender - GLS_GOSSIP_GET_REWARDS_SENDER + 1;

    for (uint32 i = 0; i < GLS_SPELLS_REWARD_COUNT; ++i)
    {
        uint32 spellID = criteria->RewardSpells[i];
        if (!spellID)
            continue;

        auto stringSelect = [](uint32 select, uint32 spellRewardNumber) -> std::string const
        {
            if (!select)
                return "";

            if (select - 1 == spellRewardNumber)
                return "[Выбрано] - ";

            return "";
        };

        auto spellLink = sGameLocale->GetSpellNamelocale(spellID);

        AddGossipItemFor(player, 10, Warhead::StringFormat("# %u. %s%s", i + 1, stringSelect(selectShoose, i).c_str(), spellLink.c_str()), GLS_GOSSIP_GET_REWARDS_SENDER + i, action,
                         Warhead::StringFormat("Вы уверены, что хотите выбрать %s?", spellLink.c_str()), 0, false);
    }

    AddGossipItemFor(player, 10, "#", sender, action);

    if (selectShoose && !criteriaProgress->IsProgressDone(criteriaID))
        AddGossipItemFor(player, 10, "--- Получить награды", selectShoose + GLS_GOSSIP_GET_ALL_REWARDS_SENDER - 1, action);

    AddGossipItemFor(player, 10, "#", sender, action);
    AddGossipItemFor(player, 10, ">> К списку критериев", GOSSIP_SENDER_MAIN, 2);
    AddGossipItemFor(player, 10, ">> В главное меню", GOSSIP_SENDER_MAIN, 99);
    SendGossipMenuFor(player, 1, creature->GetGUID());
}

void GuildLevelSystem::GetRewardsCriteria(Player* player, Creature* creature, uint32 sender, uint32 action)
{
    uint32 criteriaID = action - GLS_GOSSIP_CRITERIA_ID;

    auto criteriaProgress = GetCriteriaProgress(player->GetGuildId());
    if (!criteriaProgress)
        return;

    if (player->GetRank() != 0)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("Вы не глава гильдии!");
        return;
    }

    uint32 spellType = sender - GLS_GOSSIP_GET_ALL_REWARDS_SENDER;

    if (spellType >= GLS_SPELLS_REWARD_COUNT)
    {
        LOG_FATAL("module.gls", "> GLS: spellType(%u) >= GLS_SPELLS_REWARD_COUNT(%u)", spellType, GLS_SPELLS_REWARD_COUNT);
        return;
    }

    uint32 spellID = criteriaProgress->GetRewardSpellID(criteriaID, spellType);

    auto trans = CharacterDatabase.BeginTransaction();

    // Reward spells
    for (auto const& itr : player->GetGuild()->GetAllMembers())
    {
        auto member = itr.second;

        if (!member->IsOnline())
            trans->PAppend("INSERT INTO `character_spell`(`guid`, `spell`, `specMask`) VALUES (%u, %u, 255)", static_cast<uint32>(member->GetGUID()), spellID);
        else
        {
            auto player = member->FindPlayer();
            if (!player)
                return;

            if (!player->HasSpell(spellID))
                player->learnSpell(spellID);
        }
    }

    CharacterDatabase.CommitTransaction(trans);

    criteriaProgress->SetSelectedSpell(criteriaID, spellID);
    criteriaProgress->SetProgressDone(criteriaID);
    criteriaProgress->SaveToDB(criteriaID);

    CloseGossipMenuFor(player);
}

void GuildLevelSystem::GetAllLink(Player* player, Creature* creature, uint32 sender, uint32 action)
{
    uint32 criteriaID = action - GLS_GOSSIP_CRITERIA_ID;

    auto criteriaProgress = GetCriteriaProgress(player->GetGuildId(), true);
    if (!criteriaProgress)
        return;

    ChatHandler handler(player->GetSession());

    handler.PSendSysMessage("# Требуемые предметы для критерия %u", criteriaID);

    for (uint32 i = 0; i < GLS_ITEMS_COUNT; ++i)
    {
        uint32 itemID = criteriaProgress->GetCriteriaItemID(criteriaID, i);
        if (!itemID)
            continue;

        auto itemLink = sGameLocale->GetItemLink(itemID);
        auto currentItems = criteriaProgress->GetItemCountProgress(criteriaID, i);
        auto maxItems = criteriaProgress->GetMaxCriteriaItemCount(criteriaID, i);
        float persents = float(currentItems) / float(maxItems) * 100.0f;

        if (!itemLink.empty())
            handler.PSendSysMessage("# %u. %s - %u/%u", i + 1, itemLink.c_str(), currentItems, maxItems);
    }

    handler.PSendSysMessage("#");
    handler.PSendSysMessage("# Награды на выбор при выполнения критерия %u", criteriaID);

    for (uint32 i = 0; i < GLS_SPELLS_REWARD_COUNT; ++i)
    {
        uint32 spellID = criteriaProgress->GetRewardSpellID(criteriaID, i);
        if (!spellID)
            continue;

        handler.PSendSysMessage("# %u. %s", i + 1, sGameLocale->GetSpellLink(spellID).c_str());
    }

    ShowCriteriaInfo(player, creature, GLS_GOSSIP_SHOW_CRITERIA_SENDER, action);
}

void GuildLevelSystem::SetNextStage(Player* player)
{
    uint32 guildID = player->GetGuildId();

    if (player->GetRank() != 0)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("Вы не глава гильдии!");
        return;
    }

    auto criteriaProgress = GetCriteriaProgress(guildID);
    if (!criteriaProgress)
        return;

    uint32 nextStage = GetNextStage(criteriaProgress->GetStageID());

    criteriaProgress->SetStageID(nextStage);
    SendGuildFormat(guildID, "|cFFFF0000#|cff6C8CD5 Гильдия перешла на новую стадию -|r %u", nextStage);
}

// Spells
void GuildLevelSystem::UnLearnSpellsForPlayer(uint32 lowGuid, uint32 guildID)
{
    auto criteriaProgress = GetCriteriaProgress(guildID);
    if (!criteriaProgress)
        return; // if guild newest

    criteriaProgress->UnLearnSpells(lowGuid);
}

void GuildLevelSystem::LearnSpellsForPlayer(Player* player, uint32 guildID)
{
    auto criteriaProgress = GetCriteriaProgress(guildID);
    if (!criteriaProgress)
        return; // if guild newest

    criteriaProgress->LearnSpells(player);
}

void GuildLevelSystem::ShowInvestedMenu(Player* player, Creature* creature, uint32 sender, uint32 action)
{
    ClearGossipMenuFor(player);

    uint32 criteriaID = action - GLS_GOSSIP_CRITERIA_ID;

    auto criteriaProgress = GetCriteriaProgress(player->GetGuildId(), true);
    if (!criteriaProgress)
        return;

    uint32 itemID = criteriaProgress->GetCriteriaItemID(criteriaID, sender);
    if (!itemID)
        return;

    auto itemLink = sGameLocale->GetItemLink(itemID, player->GetSession()->GetSessionDbLocaleIndex());
    uint32 currentItems = criteriaProgress->GetItemCountProgress(criteriaID, sender);
    uint32 maxItems = criteriaProgress->GetMaxCriteriaItemCount(criteriaID, sender);
    uint32 playerItemCount = player->GetItemCount(itemID);
    uint32 itemsNeed = maxItems - currentItems;
    uint32 itemCount = playerItemCount;
    float persents = float(currentItems) / float(maxItems) * 100.0f;

    if (itemCount > itemsNeed)
        itemCount = itemsNeed;

    AddGossipItemFor(player, 10, "#", sender, action);
    AddGossipItemFor(player, 10, Warhead::StringFormat("# %s", itemLink.c_str()), sender, action);
    AddGossipItemFor(player, 10, Warhead::StringFormat("# Вложений - %u/%u", currentItems, maxItems), sender, action);
    AddGossipItemFor(player, 10, Warhead::StringFormat("# Осталось вложить - %u", itemsNeed), sender, action);
    AddGossipItemFor(player, 10, Warhead::StringFormat("# Прогресс выполнения - %0.2f%%", persents), sender, action);
    AddGossipItemFor(player, 10, "#", sender, action);
    AddGossipItemFor(player, 10, Warhead::StringFormat("- В инвентаре - %u", playerItemCount), sender, action);

    if (itemsNeed)
    {
        AddGossipItemFor(player, 10, "- Вложить выборочно", sender, action, "Введите количество", 0, true);
        AddGossipItemFor(player, 10, Warhead::StringFormat("- Вложить %s (%u)", itemCount == itemsNeed ? "до конца" : "всё что есть", itemCount), sender, criteriaID + GLS_GOSSIP_CRITERIA_ID_FULL,
                         Warhead::StringFormat("Вы уверены, что хотите вложить %s %u?", itemLink.c_str(), playerItemCount), 0, false);
    }

    AddGossipItemFor(player, 10, ">> К списку критериев", GOSSIP_SENDER_MAIN, 2);
    AddGossipItemFor(player, 10, ">> В главное меню", GOSSIP_SENDER_MAIN, 99);
    SendGossipMenuFor(player, 1, creature->GetGUID());
}

void GuildLevelSystem::AddEmptyGuildCriteria(uint32 guildID)
{
    _guildCriteriaProgress.insert(std::make_pair(guildID, new GuildCriteria(guildID, 1)));
}

void GuildLevelSystem::RescaleCriterias(uint32 guildID)
{
    auto criteriaProgress = GetCriteriaProgress(guildID, true);
    if (!criteriaProgress)
        return;

    criteriaProgress->RescalingCriterias();
}

uint32 GuildLevelSystem::GetCountCriteriaInStage(uint32 stageID)
{
    auto const& _stages = _guildStages.find(stageID);
    if (_stages != _guildStages.end())
        return static_cast<uint32>(_stages->second.size());

    LOG_FATAL("module.gls", "> GLS: Not find stage %u", stageID);

    return 0;
}

uint32 GuildLevelSystem::GetNextStage(uint32 currStage)
{
    uint32 nextStage = currStage + 1;

    for (auto const& itr : _guildStages)
        if (nextStage == itr.first)
            return nextStage;

    LOG_FATAL("module.gls", "> GLS: Not found next stage after %u", currStage);

    return currStage;
}
