#include <cstdlib>
#include <stdio.h>

#include "db.hpp"

DB::DB(const char *path)
{
    int rc = sqlite3_open(path, &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n",
                sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    exec("create table if not exists data (id integer PRIMARY KEY, data text NOT NULL);");
}

void DB::exec(const char *sql)
{
    int rc = sqlite3_exec(db, sql, 0, 0, &err);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err);
        sqlite3_free(err);
        return;
    }
}
