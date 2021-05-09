#include <QtWidgets>

#include "encrypt.hpp"
#include "db.hpp"
#include "app.hpp"

struct AES_ctx ctx;
uint8_t aes_iv[] = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};

unsigned char* decode_line(const char *line)
{
    size_t decsize = 0;
    size_t line_length = strlen(line);
    unsigned char *decoded_data = b64_decode_ex(line, line_length, &decsize);
    xcrypt_buffer(pm->key, decoded_data, decsize);
    return decoded_data;
}

void decrypt_and_print(char *label)
{
    static QString result;

    if (label) {
        pm->db->find_label(label, callback_lambda {
            for (int i = 0; i < argc; ++i)
            {
                unsigned char* decoded_data = decode_line(argv[i]);
                result.append((const char*)decoded_data);
                result.append('\n');
                free(decoded_data);
            }
            return 0;
        });
    } else {
        pm->db->get_data(callback_lambda {
            for (int i = 0; i < argc; ++i)
            {
                unsigned char* decoded_data = decode_line(argv[i]);
                result.append((const char*)decoded_data);
                result.append('\n');
                free(decoded_data);
            }
            return 0;
        });
    }
    auto *info = new QPlainTextEdit();
    info->document()->setPlainText(result);
    info->setReadOnly(true);

    auto *layout = new QHBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(info);

    auto *dialog = new QDialog(pm, CLOSE_BUTTON);
    dialog->setWindowTitle("Данные");
    dialog->setLayout(layout);
    dialog->setModal(true);
    dialog->show();

    info->verticalScrollBar()->setValue(0);

    result = "";
}

void encrypt_and_write(uint8_t *data, size_t data_length)
{
    xcrypt_buffer(pm->key, data, data_length);
    char *encoded_data = b64_encode(data, data_length);
    pm->db->insert_data(encoded_data);
    free(encoded_data);
}

void xcrypt_buffer(uint8_t *key, uint8_t *buffer, size_t buffer_size)
{
    AES_init_ctx_iv(&ctx, key, aes_iv);
    AES_CTR_xcrypt_buffer(&ctx, buffer, buffer_size);
}