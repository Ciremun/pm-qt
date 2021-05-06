#include <cstdlib>
#include <stdio.h>

#include "db.hpp"

DB::DB(const char *path)
{
    int rc = sqlite3_open(path, &db);

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    exec("CREATE TABLE IF NOT EXISTS data (id integer PRIMARY KEY, label text, data text NOT NULL);");
}

void DB::insert_data(const char *data)
{
    char* query = sqlite3_mprintf("INSERT INTO data (data) VALUES ('%q');", data);
    exec(query);
    sqlite3_free(query);
}

void DB::update_data_at_label(const char *new_data, const char *label)
{
    char* query = sqlite3_mprintf("UPDATE data SET data = '%q' WHERE label = '%q';", new_data, label);
    exec(query);
    sqlite3_free(query);
}