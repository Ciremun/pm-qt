#ifndef DB_HPP
#define DB_HPP

#include <sqlite3.h>

struct DB
{
    sqlite3 *db;
    char *err;

    DB(const char* path);
    void exec(const char* sql);
};

#endif // DB_HPP