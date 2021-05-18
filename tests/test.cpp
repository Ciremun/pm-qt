#include <QApplication>
#include <QtWidgets>

#include <filesystem>
#include <cstring>
#include <string>

#include <unistd.h>

#include "../src/db.hpp"
#include "../src/app.hpp"
#include "../src/encrypt.hpp"

#define AES_KEY "test key"

PM* pm;
std::string test_data;
std::string test_label;
int row;

size_t test_name_length = 0;

struct test_case {
    const char *name;
    void (*run)(void);
};

template <typename ... Args>
void print(std::string fmt, Args ... args)
{
    fprintf(stderr, (fmt + '\n').c_str(), args...);
}

void assert_test(bool t, std::string name)
{
    const char *result = NULL;
    if (t)
        result = "PASS";
    else
        result = "FAIL";
    size_t name_length = name.length();
    if (name_length > test_name_length)
        test_name_length = name_length;
    if (name_length <= test_name_length)
    {
        std::string spaces;
        size_t spaces_count = test_name_length - name.length();
        for (size_t i = 0; i < spaces_count; i++)
            spaces += ' ';
        name += spaces;
    }
    print("%s\t%s", name.c_str(), result);
}

void test_database()
{
    pm->db->insert_data(test_data.c_str(), NULL);
    pm->db->insert_data(test_data.c_str(), test_label.c_str());

    pm->db->get_data(callback_lambda {
        (void)e;
        (void)argc;
        (void)col;
        const char *label = argv[0];
        const char *data = argv[1];
        if (row == 0)
            assert_test(label == NULL && strcmp(data, test_data.c_str()) == 0, "NULL label and data");
        else if (row == 1)
            assert_test(strcmp(label, test_label.c_str()) == 0 && strcmp(data, test_data.c_str()) == 0, "label and data");
        row++;
        return 0;
    });
}

void test_encrypt()
{
    // void encrypt_and_write(std::string data_str, std::string label_str)
    {
        encrypt_and_write(test_data);
        pm->db->get_data(callback_lambda {
            (void)e;
            (void)argc;
            (void)col;
            const char *label = argv[0];
            const char *data = (const char *)decode_line(argv[1]);
            assert_test(label == NULL && strcmp(data, test_data.c_str()) == 0, "NULL label and data");
            return 0;
        });
    }

    // void encrypt_and_write(std::string data_str)
    {
        encrypt_and_write(test_data, test_label);
        pm->db->get_data(callback_lambda {
            (void)e;
            (void)argc;
            (void)col;
            if (row == 1)
            {
                const char *label = argv[0];
                const char *data = (const char *)decode_line(argv[1]);
                assert_test(strcmp(label, test_label.c_str()) == 0 && strcmp(data, test_data.c_str()) == 0, "label and data");
            }
            row++;
            return 0;
        });
    }
}

test_case cases[] = {
    {
        "test_database",
        test_database
    },
    {
        "test_encrypt",
        test_encrypt
    }
};

int main(int argc, char **argv)
{
    close(1);

    QApplication app(argc, argv);

    auto setup = [] (test_case *test) {
        print("%s:", test->name);
        pm = new PM(600, 400);
        pm->key = (uint8_t *)calloc(1, 256);
        memcpy(pm->key, AES_KEY, sizeof(AES_KEY));
        test_data = "test data";
        test_label = "label test";
        row = 0;
    };

    auto teardown = [] {
        free(pm->key);
        delete pm;
        std::filesystem::remove("data.db");
        test_data.clear();
        test_label.clear();
        print("----------------------------");
    };

    for (auto test : cases)
    {
        setup(&test);
        test.run();
        teardown();
    }

    return 0;
}