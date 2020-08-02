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

#ifndef _LOCALE_H_
#define _LOCALE_H_

#include "Common.h"
#include "Player.h"
#include <vector>

struct AcoreString
{
    std::vector<std::string> Content;
};

// Default locales
struct AchievementRewardLocale
{
    std::vector<std::string> Subject;
    std::vector<std::string> Text;
};

struct CreatureLocale
{
    std::vector<std::string> Name;
    std::vector<std::string> Title;
};

struct GameObjectLocale
{
    std::vector<std::string> Name;
    std::vector<std::string> CastBarCaption;
};

struct GossipMenuItemsLocale
{
    std::vector<std::string> OptionText;
    std::vector<std::string> BoxText;
};

struct ItemLocale
{
    std::vector<std::string> Name;
    std::vector<std::string> Description;
};

struct ItemSetNameLocale
{
    std::vector<std::string> Name;
};

struct NpcTextLocale
{
    NpcTextLocale() { Text_0.resize(8); Text_1.resize(8); }

    std::vector<std::vector<std::string>> Text_0;
    std::vector<std::vector<std::string>> Text_1;
};

struct PageTextLocale
{
    std::vector<std::string> Text;
};

struct PointOfInterestLocale
{
    std::vector<std::string> Name;
};

struct QuestLocale
{
    QuestLocale() { ObjectiveText.resize(4); }

    std::vector<std::string> Title;
    std::vector<std::string> Details;
    std::vector<std::string> Objectives;
    std::vector<std::string> OfferRewardText;
    std::vector<std::string> RequestItemsText;
    std::vector<std::string> AreaDescription;
    std::vector<std::string> CompletedText;
    std::vector<std::vector<std::string>> ObjectiveText;
};

struct QuestRequestItemsLocale
{
    std::vector<std::string> CompletionText;
};

struct QuestOfferRewardLocale
{
    std::vector<std::string> RewardText;
};

struct BroadcastText
{
    BroadcastText() : Id(0), Language(0), EmoteId0(0), EmoteId1(0), EmoteId2(0),
        EmoteDelay0(0), EmoteDelay1(0), EmoteDelay2(0), SoundId(0), Unk1(0), Unk2(0)
    {
        MaleText.resize(DEFAULT_LOCALE + 1);
        FemaleText.resize(DEFAULT_LOCALE + 1);
    }

    uint32 Id;
    uint32 Language;
    std::vector<std::string> MaleText;
    std::vector<std::string> FemaleText;
    uint32 EmoteId0;
    uint32 EmoteId1;
    uint32 EmoteId2;
    uint32 EmoteDelay0;
    uint32 EmoteDelay1;
    uint32 EmoteDelay2;
    uint32 SoundId;
    uint32 Unk1;
    uint32 Unk2;

    std::string const& GetText(LocaleConstant locale = DEFAULT_LOCALE, uint8 gender = GENDER_MALE, bool forceGender = false) const
    {
        if (gender == GENDER_FEMALE && (forceGender || !FemaleText[DEFAULT_LOCALE].empty()))
        {
            if (FemaleText.size() > size_t(locale) && !FemaleText[locale].empty())
                return FemaleText[locale];
            return FemaleText[DEFAULT_LOCALE];
        }
        // else if (gender == GENDER_MALE)
        {
            if (MaleText.size() > size_t(locale) && !MaleText[locale].empty())
                return MaleText[locale];
            return MaleText[DEFAULT_LOCALE];
        }
    }
};

// New strings and locales
struct RaceString
{
    RaceString()
    {
        NameMale.resize(DEFAULT_LOCALE + 1);
        NameFemale.resize(DEFAULT_LOCALE + 1);
    }

    std::vector<std::string> NameMale;
    std::vector<std::string> NameFemale;

    std::string const& GetText(LocaleConstant locale = DEFAULT_LOCALE, uint8 gender = GENDER_MALE) const
    {
        if (gender == GENDER_FEMALE)
        {
            if (NameFemale.size() > size_t(locale) && !NameFemale[locale].empty())
                return NameFemale[locale];

            if (NameMale.size() > size_t(locale) && !NameMale[locale].empty())
                return NameMale[locale];

            return NameFemale[DEFAULT_LOCALE];
        }

        if (NameMale.size() > size_t(locale) && !NameMale[locale].empty())
            return NameMale[locale];

        return NameMale[DEFAULT_LOCALE];
    }
};

struct ClassString
{
    ClassString()
    {
        NameMale.resize(DEFAULT_LOCALE + 1);
        NameFemale.resize(DEFAULT_LOCALE + 1);
    }

    std::vector<std::string> NameMale;
    std::vector<std::string> NameFemale;

    std::string const& GetText(LocaleConstant locale = DEFAULT_LOCALE, uint8 gender = GENDER_MALE) const
    {
        if (gender == GENDER_FEMALE)
        {
            if (NameFemale.size() > size_t(locale) && !NameFemale[locale].empty())
                return NameFemale[locale];

            if (NameMale.size() > size_t(locale) && !NameMale[locale].empty())
                return NameMale[locale];

            return NameFemale[DEFAULT_LOCALE];
        }

        if (NameMale.size() > size_t(locale) && !NameMale[locale].empty())
            return NameMale[locale];

        return NameMale[DEFAULT_LOCALE];
    }
};

struct ModuleString
{
    ModuleString()
    {
        Content.resize(DEFAULT_LOCALE + 1);
    }

    std::vector<std::string> Content;

    std::string GetText(uint8 locale = 0) const
    {
        if (Content.size() > size_t(locale) && !Content[locale].empty())
            return Content[locale];

        if (!Content[0].empty())
            return Content[0];

        return "<error>";
    }
};

class GameLocale
{
private:
    GameLocale();
    ~GameLocale();

public:
    static GameLocale* instance();

    void LoadAllLocales();
    bool LoadAcoreStrings();

    void AddLocaleString(std::string const& str, LocaleConstant locale, std::vector<std::string>& data);

    inline void GetLocaleString(const std::vector<std::string>& data, int loc_idx, std::string& value)
    {
        if (data.size() > size_t(loc_idx) && !data[loc_idx].empty())
            value = data[loc_idx];
    }

    AcoreString const* GetAcoreString(uint32 entry) const;
    char const* GetAcoreString(uint32 entry, LocaleConstant locale) const;
    char const* GetAcoreStringForDBCLocale(uint32 entry) const { return GetAcoreString(entry, DBCLocaleIndex); }

    LocaleConstant GetDBCLocaleIndex() const { return DBCLocaleIndex; }
    void SetDBCLocaleIndex(LocaleConstant locale) { DBCLocaleIndex = locale; }

    void LoadRewardLocales();
    void LoadBroadcastTexts();
    void LoadBroadcastTextLocales();
    void LoadCreatureLocales();
    void LoadGameObjectLocales();
    void LoadItemLocales();
    void LoadItemSetNameLocales();
    void LoadQuestLocales();
    void LoadNpcTextLocales();
    void LoadQuestOfferRewardLocale();
    void LoadQuestRequestItemsLocale();
    void LoadPageTextLocales();
    void LoadGossipMenuItemsLocales();
    void LoadPointOfInterestLocales();

    AchievementRewardLocale const* GetAchievementRewardLocale(uint32 entry) const;
    BroadcastText const* GetBroadcastText(uint32 id) const;
    CreatureLocale const* GetCreatureLocale(uint32 entry) const;
    GameObjectLocale const* GetGameObjectLocale(uint32 entry) const;
    GossipMenuItemsLocale const* GetGossipMenuItemsLocale(uint32 entry) const;
    ItemLocale const* GetItemLocale(uint32 entry) const;
    ItemSetNameLocale const* GetItemSetNameLocale(uint32 entry) const;
    NpcTextLocale const* GetNpcTextLocale(uint32 entry) const;
    PageTextLocale const* GetPageTextLocale(uint32 entry) const;
    PointOfInterestLocale const* GetPointOfInterestLocale(uint32 entry) const;
    QuestLocale const* GetQuestLocale(uint32 entry) const;
    QuestOfferRewardLocale const* GetQuestOfferRewardLocale(uint32 entry) const;
    QuestRequestItemsLocale const* GetQuestRequestItemsLocale(uint32 entry) const;

    // New strings and locales
    void LoadRaceStrings();
    void LoadClassStrings();

    RaceString const* GetRaseString(uint32 id) const;
    ClassString const* GetClassString(uint32 id) const;    

    // Module strings
    void LoadModuleString();
    std::string GetModuleString(std::string _moduleName, uint32 id, uint8 _locale) const;

    // Chat func
    template<typename... Args>
    void SendPlayerMessage(Player* player, std::string moduleName, uint32 id, Args&& ... args)
    {
        SendPlayerMessage(player, warhead::StringFormat(GetModuleString(moduleName, id, uint8(player->GetSession()->GetSessionDbLocaleIndex())), std::forward<Args>(args)...));
    }

    template<typename... Args>
    void SendGlobalMessage(std::string moduleName, uint32 id, bool gmOnly, Args&& ... args)
    {
        SendGlobalMessage(gmOnly, warhead::StringFormat(GetModuleString(moduleName, id, DBCLocaleIndex), std::forward<Args>(args)...));
    }

private:
    typedef std::unordered_map<uint32, AcoreString> AcoreStringContainer;

    AcoreStringContainer _acoreStringStore;
    LocaleConstant DBCLocaleIndex;

    typedef std::unordered_map<uint32, AchievementRewardLocale> AchievementRewardLocales;
    typedef std::unordered_map<uint32, BroadcastText> BroadcastTextContainer;
    typedef std::unordered_map<uint32, CreatureLocale> CreatureLocaleContainer;
    typedef std::unordered_map<uint32, GameObjectLocale> GameObjectLocaleContainer;
    typedef std::unordered_map<uint32, GossipMenuItemsLocale> GossipMenuItemsLocaleContainer;
    typedef std::unordered_map<uint32, ItemLocale> ItemLocaleContainer;
    typedef std::unordered_map<uint32, ItemSetNameLocale> ItemSetNameLocaleContainer;
    typedef std::unordered_map<uint32, NpcTextLocale> NpcTextLocaleContainer;
    typedef std::unordered_map<uint32, PageTextLocale> PageTextLocaleContainer;
    typedef std::unordered_map<uint32, PointOfInterestLocale> PointOfInterestLocaleContainer;
    typedef std::unordered_map<uint32, QuestLocale> QuestLocaleContainer;
    typedef std::unordered_map<uint32, QuestOfferRewardLocale> QuestOfferRewardLocaleContainer;
    typedef std::unordered_map<uint32, QuestRequestItemsLocale> QuestRequestItemsLocaleContainer;

    AchievementRewardLocales _achievementRewardLocales;
    BroadcastTextContainer _broadcastTextStore;
    CreatureLocaleContainer _creatureLocaleStore;
    GameObjectLocaleContainer _gameObjectLocaleStore;
    GossipMenuItemsLocaleContainer _gossipMenuItemsLocaleStore;
    ItemLocaleContainer _itemLocaleStore;
    ItemSetNameLocaleContainer _itemSetNameLocaleStore;    
    NpcTextLocaleContainer _npcTextLocaleStore;
    PageTextLocaleContainer _pageTextLocaleStore;    
    PointOfInterestLocaleContainer _pointOfInterestLocaleStore;
    QuestLocaleContainer _questLocaleStore;
    QuestOfferRewardLocaleContainer _questOfferRewardLocaleStore;
    QuestRequestItemsLocaleContainer _questRequestItemsLocaleStore;

    // New strings and locales
    typedef std::unordered_map<uint32, RaceString> RaceStringContainer;
    typedef std::unordered_map<uint32, ClassString> ClassStringContainer;

    RaceStringContainer _raceStringStore;
    ClassStringContainer _classStringStore;

    // Module string
    typedef std::unordered_map<uint32, ModuleString> ModuleStringContainer;
    typedef std::unordered_map<std::string, ModuleStringContainer> AllModulesStringContainer;

    AllModulesStringContainer _modulesStringStore;

    void AddModuleString(std::string const& moduleName, ModuleStringContainer& data);
    void SendPlayerMessage(Player* player, std::string&& message);
    void SendGlobalMessage(bool gmOnly, std::string&& message);
};

#define sGameLocale GameLocale::instance()

#endif //_LOCALE_H_
