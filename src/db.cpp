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

void DB::exec_fmt(sqlite_callback c, const char *fmt, const char *args...)
{
    char* query = sqlite3_mprintf(fmt, args);
    exec(query, c);
    sqlite3_free(query);
}

void DB::exec_fmt(const char *fmt, const char *args...)
{
    char* query = sqlite3_mprintf(fmt, args);
    exec(query);
    sqlite3_free(query);
}

void DB::get_data(sqlite_callback c)
{
    exec("SELECT data FROM data;", c);
}

void DB::find_label(const char *label, sqlite_callback c)
{
    exec_fmt(c, "SELECT data FROM data WHERE label = '%q';", label);
}

void DB::insert_data(const char *data)
{
    exec_fmt("INSERT INTO data (data) VALUES ('%q');", data);
}

void DB::update_data_at_label(const char *new_data, const char *label)
{
    exec_fmt("UPDATE data SET data = '%q' WHERE label = '%q';", new_data, label);
}

void DB::clear_data()
{
    exec("DELETE FROM data;");
}