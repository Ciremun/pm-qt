#include <QtWidgets>

#include "../src/db.hpp"
#include "../src/app.hpp"

PM* pm;

int main()
{
    auto *db = new DB("test.db");
    db->insert_data("test_data", "test_label");

    return 0;
}