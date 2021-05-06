#ifndef DB_HPP
#define DB_HPP

#include <sqlite3.h>

struct DB {
    sqlite3 *db;
    char *err;

    DB(const char *path);

    template <typename F = std::nullptr_t>
    void exec(const char *sql, F callback = 0)
    {
        int rc = sqlite3_exec(db, sql, callback, 0, &err);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", err);
            sqlite3_free(err);
            return;
        }
    }

    void insert_data(const char *data);
    void update_data_at_label(const char *new_data, const char *label);
};

#endif // DB_HPP