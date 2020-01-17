/*
 * Copyright (C) 2016+     AzerothCore <www.azerothcore.org>
 */

#ifndef _LOCALE_H_
#define _LOCALE_H_

#include "Common.h"
#include <vector>

struct AcoreString
{
    std::vector<std::string> Content;
};

class GameLocale
{
private:
    GameLocale();
    ~GameLocale();

public:
    static GameLocale* instance();

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

private:
    typedef std::unordered_map<int32, AcoreString> AcoreStringContainer;

    AcoreStringContainer _acoreStringStore;
    LocaleConstant DBCLocaleIndex;
};

#define sGameLocale GameLocale::instance()

#endif //_LOCALE_H_
