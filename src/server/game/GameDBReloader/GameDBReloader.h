/*
 * Copyright (C)
 */

#ifndef _DBLOADER_
#define _DBLOADER_

#include "Common.h"
#include <unordered_map>
#include <functional>

struct DBLoaderSctuct
{
    std::string Message;
    std::function<void()> Function;
};

class GameDBReloader
{
public:
    static GameDBReloader* instance();

    void LoadDefaultTables();
    void LoadTable(std::string const& dbName);
    bool AddTable(std::string dbName, std::string const& message, std::function<void()> function);

private:
    std::unordered_map<std::string /*table name*/, DBLoaderSctuct /*msg and func*/> _store;
};

#define sDBLoader GameDBReloader::instance()

#endif // _DBLOADER_
