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

void DB::get_data(sqlite_callback c)
{
    exec("SELECT label, data FROM data;", c);
}

void DB::find_label(const char *label, sqlite_callback c)
{
    exec_fmt(c, "SELECT label, data FROM data WHERE label LIKE '%q%%';", label);
}

void DB::insert_data(const char *data, const char *label)
{
    if (label && label[0] != '\0')
    {
        bool found = false;
        struct sqlite3_stmt *select_statement;
        char* query = sqlite3_mprintf("SELECT data FROM data WHERE label = '%q';", label);

        int result = sqlite3_prepare_v2(db, query, -1, &select_statement, NULL);
        if(result == SQLITE_OK && sqlite3_step(select_statement) == SQLITE_ROW)
            found = true;

        sqlite3_free(query);
        sqlite3_finalize(select_statement);

        if (found)
        {
            update_data_at_label(data, label);
            return;
        }
    }
    exec_fmt("INSERT INTO data (label, data) VALUES (%Q, %Q);", label, data);
}

void DB::update_data_at_label(const char *new_data, const char *label)
{
    exec_fmt("UPDATE data SET data = '%q' WHERE label = '%q';", new_data, label);
}

void DB::clear_data()
{
    exec("DELETE FROM data;");
}