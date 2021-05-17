#include <QtWidgets>

#include <filesystem>
#include <cassert>
#include <cstring>
#include <string>

#include "../src/db.hpp"
#include "../src/app.hpp"

PM* pm;

std::string test_data = "test data";
std::string test_label = "label test";
int row = 0;

int main()
{
    auto setup = [] {
    };

    auto teardown = [] {
        std::filesystem::remove("test.db");
        test_data.clear();
        test_label.clear();
        row = 0;
    };

    setup();

    // database
    {
        DB db("test.db");

        db.insert_data(test_data.c_str(), NULL);
        db.insert_data(test_data.c_str(), test_label.c_str());

        db.get_data(callback_lambda {
            assert(row < 2);
            const char *label = argv[0];
            const char *data = argv[1];
            if (row == 0)
                assert(label == NULL && strcmp(data, test_data.c_str()) == 0);
            else if (row == 1)
                assert(strcmp(label, test_label.c_str()) == 0 && strcmp(data, test_data.c_str()) == 0);
            row++;
            return 0;
        });

        printf("database pass\n");
    }

    teardown();

    return 0;
}