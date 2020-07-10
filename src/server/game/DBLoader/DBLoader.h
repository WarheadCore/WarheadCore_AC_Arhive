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

class DBLoader
{
public:
    static DBLoader* instance();

    void LoadDefaultTables();
    void LoadTable(std::string const& dbName);
    bool AddTable(std::string dbName, std::string const& message, std::function<void()> function);

private:
    std::unordered_map<std::string /*table name*/, DBLoaderSctuct /*msg and func*/> _store;
};

#define sDBLoader DBLoader::instance()

#endif // _DBLOADER_
