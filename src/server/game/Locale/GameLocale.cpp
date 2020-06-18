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

#include "GameLocale.h"
#include "AccountMgr.h"
#include "Chat.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "Player.h"
#include "World.h"

GameLocale::GameLocale()
{
    DBCLocaleIndex = LOCALE_enUS;
}

GameLocale::~GameLocale()
{

}

GameLocale* GameLocale::instance()
{
    static GameLocale instance;
    return &instance;
}

void GameLocale::LoadAllLocales()
{
    uint32 oldMSTime = getMSTime();

    LoadBroadcastTexts();
    LoadBroadcastTextLocales();
    LoadCreatureLocales();
    LoadGameObjectLocales();
    LoadItemLocales();
    LoadItemSetNameLocales();
    LoadQuestLocales();
    LoadQuestOfferRewardLocale();
    LoadQuestRequestItemsLocale();
    LoadNpcTextLocales();
    LoadPageTextLocales();
    LoadGossipMenuItemsLocales();
    LoadPointOfInterestLocales();
    LoadRaceStrings();
    LoadClassStrings();
    LoadModuleString();

    // Get once for all the locale index of DBC language (console/broadcasts)
    sGameLocale->SetDBCLocaleIndex(sWorld->GetDefaultDbcLocale());
    sLog->outString(">> Localization strings loaded in %u ms", GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

bool GameLocale::LoadAcoreStrings()
{
    uint32 oldMSTime = getMSTime();

    _acoreStringStore.clear(); // for reload case

    QueryResult result = WorldDatabase.PQuery("SELECT entry, content_default, locale_koKR, locale_frFR, locale_deDE, locale_zhCN, locale_zhTW, locale_esES, locale_esMX, locale_ruRU FROM acore_string");
    if (!result)
    {
        sLog->outString(">> Loaded 0 acore strings. DB table `acore_strings` is empty.");
        sLog->outString();
        return false;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        auto& data = _acoreStringStore[entry];

        data.Content.resize(DEFAULT_LOCALE + 1);

        for (uint8 i = 0; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i + 1].GetString(), LocaleConstant(i), data.Content);

    } while (result->NextRow());

    sLog->outString(">> Loaded %u acore strings in %u ms", static_cast<uint32>(_acoreStringStore.size()), GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();

    return true;
}

void GameLocale::AddLocaleString(std::string const& str, LocaleConstant locale, std::vector<std::string>& data)
{
    if (!str.empty())
    {
        if (data.size() <= size_t(locale))
            data.resize(locale + 1);

        data[locale] = str;
    }
}

char const* GameLocale::GetAcoreString(uint32 entry, LocaleConstant locale) const
{
    if (auto as = GetAcoreString(entry))
    {
        if (as->Content.size() > size_t(locale) && !as->Content[locale].empty())
            return as->Content[locale].c_str();

        return as->Content[DEFAULT_LOCALE].c_str();
    }

    sLog->outErrorDb("Warhead string entry %u not found in DB.", entry);

    return "<error>";
}

AcoreString const* GameLocale::GetAcoreString(uint32 entry) const
{
    auto const& itr = _acoreStringStore.find(entry);
    if (itr == _acoreStringStore.end())
        return nullptr;

    return &itr->second;
}

void GameLocale::LoadRewardLocales()
{
    uint32 oldMSTime = getMSTime();

    _achievementRewardLocales.clear();                       // need for reload case

    //                                               0   1       2        3
    QueryResult result = WorldDatabase.Query("SELECT ID, Locale, Subject, Text FROM achievement_reward_locale");

    if (!result)
    {
        sLog->outString(">> Loaded 0 achievement reward locale strings.  DB table `achievement_reward_locale` is empty");
        sLog->outString();
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 ID = fields[0].GetUInt32();
        std::string LocaleName = fields[1].GetString();
        std::string Subject = fields[2].GetString();
        std::string Text = fields[3].GetString();

        if (_achievementRewardLocales.find(ID) == _achievementRewardLocales.end())
        {
            sLog->outErrorDb("Table `achievement_reward_locale` (Entry: %u) has locale strings for non-existing achievement reward.", ID);
            continue;
        }

        AchievementRewardLocale& data = _achievementRewardLocales[ID];
        LocaleConstant locale = GetLocaleByName(LocaleName);
        if (locale == LOCALE_enUS)
            continue;

        sGameLocale->AddLocaleString(Subject, locale, data.Subject);
        sGameLocale->AddLocaleString(Text, locale, data.Text);

    } while (result->NextRow());

    sLog->outString(">> Loaded %u Achievement Reward Locale strings in %u ms", static_cast<uint32>(_achievementRewardLocales.size()), GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void GameLocale::LoadBroadcastTexts()
{
    uint32 oldMSTime = getMSTime();

    _broadcastTextStore.clear(); // for reload case

    //                                               0   1         2         3           4         5         6         7            8            9            10       11    12
    QueryResult result = WorldDatabase.Query("SELECT ID, Language, MaleText, FemaleText, EmoteID0, EmoteID1, EmoteID2, EmoteDelay0, EmoteDelay1, EmoteDelay2, SoundId, Unk1, Unk2 FROM broadcast_text");
    if (!result)
    {
        sLog->outString(">> Loaded 0 broadcast texts. DB table `broadcast_text` is empty.");
        sLog->outString();
        return;
    }

    _broadcastTextStore.rehash(result->GetRowCount());

    do
    {
        Field* fields = result->Fetch();

        BroadcastText bct;

        bct.Id = fields[0].GetUInt32();
        bct.Language = fields[1].GetUInt32();
        bct.MaleText[DEFAULT_LOCALE] = fields[2].GetString();
        bct.FemaleText[DEFAULT_LOCALE] = fields[3].GetString();
        bct.EmoteId0 = fields[4].GetUInt32();
        bct.EmoteId1 = fields[5].GetUInt32();
        bct.EmoteId2 = fields[6].GetUInt32();
        bct.EmoteDelay0 = fields[7].GetUInt32();
        bct.EmoteDelay1 = fields[8].GetUInt32();
        bct.EmoteDelay2 = fields[9].GetUInt32();
        bct.SoundId = fields[10].GetUInt32();
        bct.Unk1 = fields[11].GetUInt32();
        bct.Unk2 = fields[12].GetUInt32();

        if (bct.SoundId && !sSoundEntriesStore.LookupEntry(bct.SoundId))
        {
            LOG_DEBUG("misc", "BroadcastText (Id: %u) in table `broadcast_text` has SoundId %u but sound does not exist.", bct.Id, bct.SoundId);
            bct.SoundId = 0;
        }

        if (!GetLanguageDescByID(bct.Language))
        {
            LOG_DEBUG("misc", "BroadcastText (Id: %u) in table `broadcast_text` using Language %u but Language does not exist.", bct.Id, bct.Language);
            bct.Language = LANG_UNIVERSAL;
        }

        if (bct.EmoteId0 && !sEmotesStore.LookupEntry(bct.EmoteId0))
        {
            LOG_DEBUG("misc", "BroadcastText (Id: %u) in table `broadcast_text` has EmoteId0 %u but emote does not exist.", bct.Id, bct.EmoteId0);
            bct.EmoteId0 = 0;
        }

        if (bct.EmoteId1 && !sEmotesStore.LookupEntry(bct.EmoteId1))
        {
            LOG_DEBUG("misc", "BroadcastText (Id: %u) in table `broadcast_text` has EmoteId1 %u but emote does not exist.", bct.Id, bct.EmoteId1);
            bct.EmoteId1 = 0;
        }

        if (bct.EmoteId2 && !sEmotesStore.LookupEntry(bct.EmoteId2))
        {
            LOG_DEBUG("misc", "BroadcastText (Id: %u) in table `broadcast_text` has EmoteId2 %u but emote does not exist.", bct.Id, bct.EmoteId2);
            bct.EmoteId2 = 0;
        }

        _broadcastTextStore.insert(std::make_pair(bct.Id, bct));
    } while (result->NextRow());

    sLog->outString(">> Loaded %u broadcast texts in %u ms", static_cast<uint32>(_broadcastTextStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GameLocale::LoadBroadcastTextLocales()
{
    uint32 oldMSTime = getMSTime();

    //                                               0   1       2         3
    QueryResult result = WorldDatabase.Query("SELECT ID, locale, MaleText, FemaleText FROM broadcast_text_locale");

    if (!result)
    {
        sLog->outString(">> Loaded 0 broadcast text locales. DB table `broadcast_text_locale` is empty.");
        sLog->outString();
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 id = fields[0].GetUInt32();
        std::string LocaleName = fields[1].GetString();
        std::string MaleText = fields[2].GetString();
        std::string FemaleText = fields[3].GetString();

        auto const& bct = _broadcastTextStore.find(id);
        if (bct == _broadcastTextStore.end())
        {
            sLog->outErrorDb("BroadcastText (Id: %u) in table `broadcast_text_locale` does not exist. Skipped!", id);
            continue;
        }

        LocaleConstant locale = GetLocaleByName(LocaleName);
        if (locale == LOCALE_enUS)
            continue;

        AddLocaleString(MaleText, locale, bct->second.MaleText);
        AddLocaleString(FemaleText, locale, bct->second.FemaleText);
    } while (result->NextRow());

    sLog->outString(">> Loaded %u Broadcast Text Locales in %u ms", static_cast<uint32>(_broadcastTextStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GameLocale::LoadCreatureLocales()
{
    uint32 oldMSTime = getMSTime();

    _creatureLocaleStore.clear();                              // need for reload case

    //                                               0      1       2     3
    QueryResult result = WorldDatabase.Query("SELECT entry, locale, Name, Title FROM creature_template_locale");
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 ID = fields[0].GetUInt32();
        std::string LocaleName = fields[1].GetString();
        std::string Name = fields[2].GetString();
        std::string Title = fields[3].GetString();

        CreatureLocale& data = _creatureLocaleStore[ID];
        LocaleConstant locale = GetLocaleByName(LocaleName);
        if (locale == LOCALE_enUS)
            continue;

        AddLocaleString(Name, locale, data.Name);
        AddLocaleString(Title, locale, data.Title);

    } while (result->NextRow());

    sLog->outString(">> Loaded %u Сreature Locale strings in %u ms", static_cast<uint32>(_creatureLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GameLocale::LoadGossipMenuItemsLocales()
{
    uint32 oldMSTime = getMSTime();

    _gossipMenuItemsLocaleStore.clear();                              // need for reload case

    //                                               0       1            2       3           4
    QueryResult result = WorldDatabase.Query("SELECT MenuID, OptionID, Locale, OptionText, BoxText FROM gossip_menu_option_locale");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint16 MenuID = fields[0].GetUInt16();
        uint16 OptionID = fields[1].GetUInt16();
        std::string LocaleName = fields[2].GetString();
        std::string OptionText = fields[3].GetString();
        std::string BoxText = fields[4].GetString();

        GossipMenuItemsLocale& data = _gossipMenuItemsLocaleStore[MAKE_PAIR32(MenuID, OptionID)];
        LocaleConstant locale = GetLocaleByName(LocaleName);
        if (locale == LOCALE_enUS)
            continue;

        AddLocaleString(OptionText, locale, data.OptionText);
        AddLocaleString(BoxText, locale, data.BoxText);

    } while (result->NextRow());

    sLog->outString(">> Loaded %u Gossip Menu Option Locale strings in %u ms", static_cast<uint32>(_gossipMenuItemsLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GameLocale::LoadGameObjectLocales()
{
    uint32 oldMSTime = getMSTime();

    _gameObjectLocaleStore.clear(); // need for reload case

    //                                               0      1       2     3
    QueryResult result = WorldDatabase.Query("SELECT entry, locale, name, castBarCaption FROM gameobject_template_locale");
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 ID = fields[0].GetUInt32();
        std::string LocaleName = fields[1].GetString();
        std::string Name = fields[2].GetString();
        std::string CastBarCaption = fields[3].GetString();

        GameObjectLocale& data = _gameObjectLocaleStore[ID];
        LocaleConstant locale = GetLocaleByName(LocaleName);
        if (locale == LOCALE_enUS)
            continue;

        AddLocaleString(Name, locale, data.Name);
        AddLocaleString(CastBarCaption, locale, data.CastBarCaption);

    } while (result->NextRow());

    sLog->outString(">> Loaded %u Gameobject Locale strings in %u ms", static_cast<uint32>(_gameObjectLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GameLocale::LoadItemLocales()
{
    uint32 oldMSTime = getMSTime();

    _itemLocaleStore.clear();                                 // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT ID, locale, Name, Description FROM item_template_locale");
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 ID = fields[0].GetUInt32();
        std::string LocaleName = fields[1].GetString();
        std::string Name = fields[2].GetString();
        std::string Description = fields[3].GetString();

        ItemLocale& data = _itemLocaleStore[ID];
        LocaleConstant locale = GetLocaleByName(LocaleName);
        if (locale == LOCALE_enUS)
            continue;

        AddLocaleString(Name, locale, data.Name);
        AddLocaleString(Description, locale, data.Description);

    } while (result->NextRow());

    sLog->outString(">> Loaded %u Item Locale strings in %u ms", static_cast<uint32>(_itemLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GameLocale::LoadItemSetNameLocales()
{
    uint32 oldMSTime = getMSTime();

    _itemSetNameLocaleStore.clear();                                 // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT ID, locale, Name FROM item_set_names_locale");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 ID = fields[0].GetUInt32();
        std::string LocaleName = fields[1].GetString();
        std::string Name = fields[2].GetString();

        ItemSetNameLocale& data = _itemSetNameLocaleStore[ID];
        LocaleConstant locale = GetLocaleByName(LocaleName);
        if (locale == LOCALE_enUS)
            continue;

        AddLocaleString(Name, locale, data.Name);

    } while (result->NextRow());

    sLog->outString(">> Loaded %u Item Set Name Locale strings in %u ms", static_cast<uint32>(_itemSetNameLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GameLocale::LoadNpcTextLocales()
{
    uint32 oldMSTime = getMSTime();

    _npcTextLocaleStore.clear();                              // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT ID, Locale, "
        //   2        3        4        5        6        7        8        9        10       11       12       13       14       15       16       17
        "Text0_0, Text0_1, Text1_0, Text1_1, Text2_0, Text2_1, Text3_0, Text3_1, Text4_0, Text4_1, Text5_0, Text5_1, Text6_0, Text6_1, Text7_0, Text7_1 "
        "FROM npc_text_locale");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 ID = fields[0].GetUInt32();
        std::string LocaleName = fields[1].GetString();

        NpcTextLocale& data = _npcTextLocaleStore[ID];
        LocaleConstant locale = GetLocaleByName(LocaleName);
        if (locale == LOCALE_enUS)
            continue;

        for (uint8 i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            AddLocaleString(fields[2 + i * 2].GetString(), locale, data.Text_0[i]);
            AddLocaleString(fields[3 + i * 2].GetString(), locale, data.Text_1[i]);
        }

    } while (result->NextRow());

    sLog->outString(">> Loaded %u Npc Text Locale strings in %u ms", static_cast<uint32>(_npcTextLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GameLocale::LoadPageTextLocales()
{
    uint32 oldMSTime = getMSTime();

    _pageTextLocaleStore.clear();                             // need for reload case

    //                                               0   1       2
    QueryResult result = WorldDatabase.Query("SELECT ID, locale, Text FROM page_text_locale");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 ID = fields[0].GetUInt32();
        std::string LocaleName = fields[1].GetString();
        std::string Text = fields[2].GetString();

        PageTextLocale& data = _pageTextLocaleStore[ID];
        LocaleConstant locale = GetLocaleByName(LocaleName);
        if (locale == LOCALE_enUS)
            continue;

        AddLocaleString(Text, locale, data.Text);

    } while (result->NextRow());

    sLog->outString(">> Loaded %u Page Text Locale strings in %u ms", static_cast<uint32>(_pageTextLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GameLocale::LoadPointOfInterestLocales()
{
    uint32 oldMSTime = getMSTime();

    _pointOfInterestLocaleStore.clear();                              // need for reload case

    //                                               0   1       2
    QueryResult result = WorldDatabase.Query("SELECT ID, locale, Name FROM points_of_interest_locale");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 ID = fields[0].GetUInt32();
        std::string LocaleName = fields[1].GetString();
        std::string Name = fields[2].GetString();

        PointOfInterestLocale& data = _pointOfInterestLocaleStore[ID];
        LocaleConstant locale = GetLocaleByName(LocaleName);
        if (locale == LOCALE_enUS)
            continue;

        AddLocaleString(Name, locale, data.Name);

    } while (result->NextRow());

    sLog->outString(">> Loaded %u Points Of Interest Locale strings in %u ms", static_cast<uint32>(_pointOfInterestLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GameLocale::LoadQuestLocales()
{
    uint32 oldMSTime = getMSTime();

    _questLocaleStore.clear();                                // need for reload case

    //                                               0   1       2      3        4           5        6              7               8               9               10
    QueryResult result = WorldDatabase.Query("SELECT ID, locale, Title, Details, Objectives, EndText, CompletedText, ObjectiveText1, ObjectiveText2, ObjectiveText3, ObjectiveText4 FROM quest_template_locale");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 ID = fields[0].GetUInt32();
        std::string LocaleName = fields[1].GetString();

        QuestLocale& data = _questLocaleStore[ID];
        LocaleConstant locale = GetLocaleByName(LocaleName);
        if (locale == LOCALE_enUS)
            continue;

        AddLocaleString(fields[2].GetString(), locale, data.Title);
        AddLocaleString(fields[3].GetString(), locale, data.Details);
        AddLocaleString(fields[4].GetString(), locale, data.Objectives);
        AddLocaleString(fields[5].GetString(), locale, data.AreaDescription);
        AddLocaleString(fields[6].GetString(), locale, data.CompletedText);

        for (uint8 i = 0; i < 4; ++i)
            sGameLocale->AddLocaleString(fields[i + 7].GetString(), locale, data.ObjectiveText[i]);

    } while (result->NextRow());

    sLog->outString(">> Loaded %u Quest Locale strings in %u ms", static_cast<uint32>(_questLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GameLocale::LoadQuestOfferRewardLocale()
{
    uint32 oldMSTime = getMSTime();

    _questOfferRewardLocaleStore.clear(); // need for reload case

    //                                               0     1          2
    QueryResult result = WorldDatabase.Query("SELECT Id, locale, RewardText FROM quest_offer_reward_locale");
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 id = fields[0].GetUInt32();
        std::string localeName = fields[1].GetString();

        QuestOfferRewardLocale& data = _questOfferRewardLocaleStore[id];
        LocaleConstant locale = GetLocaleByName(localeName);
        if (locale == LOCALE_enUS)
            continue;

        AddLocaleString(fields[2].GetString(), locale, data.RewardText);

    } while (result->NextRow());

    sLog->outString(">> Loaded %u Quest Offer Reward locale strings in %u ms", static_cast<uint32>(_questOfferRewardLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GameLocale::LoadQuestRequestItemsLocale()
{
    uint32 oldMSTime = getMSTime();

    _questRequestItemsLocaleStore.clear(); // need for reload case

    //                                               0     1          2
    QueryResult result = WorldDatabase.Query("SELECT Id, locale, CompletionText FROM quest_request_items_locale");
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 id = fields[0].GetUInt32();
        std::string localeName = fields[1].GetString();

        QuestRequestItemsLocale& data = _questRequestItemsLocaleStore[id];
        LocaleConstant locale = GetLocaleByName(localeName);
        if (locale == LOCALE_enUS)
            continue;
        
        AddLocaleString(fields[2].GetString(), locale, data.CompletionText);

    } while (result->NextRow());

    sLog->outString(">> Loaded %u Quest Request Items locale strings in %u ms", static_cast<uint32>(_questRequestItemsLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

AchievementRewardLocale const* GameLocale::GetAchievementRewardLocale(uint32 entry) const
{
    auto const& itr = _achievementRewardLocales.find(entry);
    return itr != _achievementRewardLocales.end() ? &itr->second : nullptr;
}

BroadcastText const* GameLocale::GetBroadcastText(uint32 id) const
{
    auto const& itr = _broadcastTextStore.find(id);
    if (itr != _broadcastTextStore.end())
        return &itr->second;

    return nullptr;
}

CreatureLocale const* GameLocale::GetCreatureLocale(uint32 entry) const
{
    auto const& itr = _creatureLocaleStore.find(entry);
    if (itr == _creatureLocaleStore.end()) return nullptr;
    return &itr->second;
}

GameObjectLocale const* GameLocale::GetGameObjectLocale(uint32 entry) const
{
    auto const& itr = _gameObjectLocaleStore.find(entry);
    if (itr == _gameObjectLocaleStore.end()) return nullptr;
    return &itr->second;
}

GossipMenuItemsLocale const* GameLocale::GetGossipMenuItemsLocale(uint32 entry) const
{
    auto const& itr = _gossipMenuItemsLocaleStore.find(entry);
    if (itr == _gossipMenuItemsLocaleStore.end()) return nullptr;
    return &itr->second;
}

ItemLocale const* GameLocale::GetItemLocale(uint32 entry) const
{
    auto const& itr = _itemLocaleStore.find(entry);
    if (itr == _itemLocaleStore.end()) return nullptr;
    return &itr->second;
}

ItemSetNameLocale const* GameLocale::GetItemSetNameLocale(uint32 entry) const
{
    auto const& itr = _itemSetNameLocaleStore.find(entry);
    if (itr == _itemSetNameLocaleStore.end()) return nullptr;
    return &itr->second;
}

NpcTextLocale const* GameLocale::GetNpcTextLocale(uint32 entry) const
{
    auto const& itr = _npcTextLocaleStore.find(entry);
    if (itr == _npcTextLocaleStore.end()) return NULL;
    return &itr->second;
}

PageTextLocale const* GameLocale::GetPageTextLocale(uint32 entry) const
{
    auto const& itr = _pageTextLocaleStore.find(entry);
    if (itr == _pageTextLocaleStore.end()) return nullptr;
    return &itr->second;
}

PointOfInterestLocale const* GameLocale::GetPointOfInterestLocale(uint32 entry) const
{
    auto const& itr = _pointOfInterestLocaleStore.find(entry);
    if (itr == _pointOfInterestLocaleStore.end()) return nullptr;
    return &itr->second;
}

QuestLocale const* GameLocale::GetQuestLocale(uint32 entry) const
{
    auto const& itr = _questLocaleStore.find(entry);
    if (itr == _questLocaleStore.end()) return nullptr;
    return &itr->second;
}

QuestOfferRewardLocale const* GameLocale::GetQuestOfferRewardLocale(uint32 entry) const
{
    auto const& itr = _questOfferRewardLocaleStore.find(entry);
    if (itr == _questOfferRewardLocaleStore.end()) return nullptr;
    return &itr->second;
}

QuestRequestItemsLocale const* GameLocale::GetQuestRequestItemsLocale(uint32 entry) const
{
    auto const& itr = _questRequestItemsLocaleStore.find(entry);
    if (itr == _questRequestItemsLocaleStore.end()) return nullptr;
    return &itr->second;
}

// New locale
void GameLocale::LoadRaceStrings()
{
    uint32 oldMSTime = getMSTime();

    _raceStringStore.clear();                              // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT ID, Locale, NameMale, NameFemale FROM `string_race`");
    if (!result)
    {
        LOG_WARN("sql.sql", "> DB table `string_race` is empty");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 ID = fields[0].GetUInt32();
        std::string LocaleName = fields[1].GetString();
        std::string NameMale = fields[2].GetString();
        std::string NameFemale = fields[3].GetString();
        
        LocaleConstant locale = GetLocaleByName(LocaleName);

        auto& data = _raceStringStore[ID];

        AddLocaleString(NameMale, locale, data.NameMale);
        AddLocaleString(NameFemale, locale, data.NameFemale);

    } while (result->NextRow());

    sLog->outString(">> Loaded %u Race strings in %u ms", static_cast<uint32>(_raceStringStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GameLocale::LoadClassStrings()
{
    uint32 oldMSTime = getMSTime();

    _classStringStore.clear();                              // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT ID, Locale, NameMale, NameFemale FROM `string_class`");
    if (!result)
    {
        LOG_WARN("sql.sql", "> DB table `string_class` is empty");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 ID = fields[0].GetUInt32();
        std::string LocaleName = fields[1].GetString();
        std::string NameMale = fields[2].GetString();
        std::string NameFemale = fields[3].GetString();

        LocaleConstant locale = GetLocaleByName(LocaleName);

        auto& data = _classStringStore[ID];

        AddLocaleString(NameMale, locale, data.NameMale);
        AddLocaleString(NameFemale, locale, data.NameFemale);

    } while (result->NextRow());

    sLog->outString(">> Loaded %u Class strings in %u ms", static_cast<uint32>(_classStringStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

RaceString const* GameLocale::GetRaseString(uint32 id) const
{
    auto const& itr = _raceStringStore.find(id);
    if (itr != _raceStringStore.end())
        return &itr->second;

    return nullptr;
}

ClassString const* GameLocale::GetClassString(uint32 id) const
{
    auto const& itr = _classStringStore.find(id);
    if (itr != _classStringStore.end())
        return &itr->second;

    return nullptr;
}

// Module string
void GameLocale::AddModuleString(std::string const& moduleName, ModuleStringContainer& data)
{
    if (data.empty())
    {
        LOG_ERROR("server", "ModuleStringContainer& data for module (%s) is empty!", moduleName.c_str());
        return;
    }

    _modulesStringStore.insert(std::make_pair(moduleName, data));
}

std::string GameLocale::GetModuleString(std::string _moduleName, uint32 id, uint8 _locale) const
{
    auto const& itr = _modulesStringStore.find(_moduleName);
    if (itr == _modulesStringStore.end())
        return "<error>";

    auto const& itr2 = itr->second.find(id);
    if (itr2 == itr->second.end())
        return "<error>";

    return itr2->second.GetText(_locale);
}

void GameLocale::LoadModuleString()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT DISTINCT `ModuleName` FROM `string_module`");
    if (!result)
    {
        LOG_WARN("sql.sql", "> DB table `string_module` is empty");
        return;
    }

    ModuleStringContainer _tempStore;
    std::vector<std::string> _localesModuleList;
    uint32 countAll = 0;

    // Add module list
    do
    {
        _localesModuleList.push_back(result->Fetch()->GetString());
        
    } while (result->NextRow());

    for (auto const& itr : _localesModuleList)
    {
        std::string moduleName = itr;

        result = WorldDatabase.PQuery("SELECT `ID`, `Locale`, `Text` FROM `string_module` WHERE `ModuleName` = '%s'", moduleName.c_str());
        if (!result)
        {
            LOG_WARN("sql.sql", "> Strings for module %s is bad!", moduleName.c_str());
            return;
        }

        do
        {
            Field* fields = result->Fetch();

            AddLocaleString(fields[2].GetString(), GetLocaleByName(fields[1].GetString()), _tempStore[fields[0].GetUInt32()].Content);
            countAll++;

        } while (result->NextRow());

        AddModuleString(moduleName, _tempStore);
    }

    sLog->outString(">> Loaded %u module strings for %u modules in %u ms", countAll, static_cast<uint32>(_modulesStringStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GameLocale::SendPlayerMessage(Player* player, std::string&& message)
{
    ChatHandler(player->GetSession()).SendSysMessage(message.c_str());
}

void GameLocale::SendGlobalMessage(bool gmOnly, std::string&& message)
{
    WorldPacket data;

    for (auto const& itr : sWorld->GetAllSessions())
    {
        auto session = itr.second;
        auto player = itr.second->GetPlayer();

        if (!session || !player || !player->IsInWorld())
            continue;

        if (gmOnly && AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity()))
            continue;

        ChatHandler::BuildChatPacket(data, CHAT_MSG_SYSTEM, LANG_UNIVERSAL, nullptr, nullptr, message);
        sWorld->SendGlobalMessage(&data);
    }
}
