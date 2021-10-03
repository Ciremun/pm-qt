#ifndef DB_HPP
#define DB_HPP

#include <sqlite3.h>

#define callback_lambda [](void *e, int argc, char **argv, char **col) -> int
using sqlite_callback = int (*)(void *e, int argc, char **argv, char **col);

struct DB
{
    sqlite3 *db;
    char *err;

    DB(const char *path);
    ~DB();

    template <typename F = std::nullptr_t>
    void exec(const char *sql, F callback = 0)
    {
        printf("exec: %s\n", sql);
        int rc = sqlite3_exec(db, sql, callback, 0, &err);

        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "SQL error: %s\n", err);
            sqlite3_free(err);
            return;
        }
    }

    template <typename... Args>
    void exec_fmt(sqlite_callback c, const char *fmt, Args... args)
    {
        char *query = sqlite3_mprintf(fmt, args...);
        exec(query, c);
        sqlite3_free(query);
    }

    template <typename... Args> void exec_fmt(const char *fmt, Args... args)
    {
        char *query = sqlite3_mprintf(fmt, args...);
        exec(query);
        sqlite3_free(query);
    }

    void get_data(sqlite_callback c);
    void insert_data(const char *data, const char *label);
    void update_data_at_label(const char *new_data, const char *label);
    void find_label(const char *label, sqlite_callback c);
    void clear_data();
};

#endif // DB_HPP