/*
 * Copyright (C) 2016+     AzerothCore <www.azerothcore.org>
 */

#include "GameLocale.h"
#include "DatabaseEnv.h"

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

bool GameLocale::LoadAcoreStrings()
{
    uint32 oldMSTime = getMSTime();

    _acoreStringStore.clear(); // for reload case

    QueryResult result = WorldDatabase.PQuery("SELECT entry, content_default, content_loc1, content_loc2, content_loc3, content_loc4, content_loc5, content_loc6, content_loc7, content_loc8 FROM acore_string");
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

    sLog->outString(">> Loaded %u acore strings in %u ms", (uint32)_acoreStringStore.size(), GetMSTimeDiffToNow(oldMSTime));
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
    if (auto ts = GetAcoreString(entry))
    {
        if (ts->Content.size() > size_t(locale) && !ts->Content[locale].empty())
            return ts->Content[locale].c_str();

        return ts->Content[DEFAULT_LOCALE].c_str();
    }

    sLog->outErrorDb("Azeroth string entry %u not found in DB.", entry);

    return "<error>";
}

AcoreString const* GameLocale::GetAcoreString(uint32 entry) const
{
    auto const& itr = _acoreStringStore.find(entry);
    if (itr == _acoreStringStore.end())
        return nullptr;

    return &itr->second;
}

