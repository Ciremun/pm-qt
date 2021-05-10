#include <QtWidgets>

#include "encrypt.hpp"
#include "db.hpp"
#include "app.hpp"

struct AES_ctx ctx;
uint8_t aes_iv[] = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};
QString decoded_result;

unsigned char* decode_line(const char *line)
{
    size_t decsize = 0;
    size_t line_length = strlen(line);
    unsigned char *decoded_data = b64_decode_ex(line, line_length, &decsize);
    xcrypt_buffer(pm->key, decoded_data, decsize);
    return decoded_data;
}

void append_data_to_result(const char *col, const char* line)
{
    if (strcmp(col, "data") == 0)
    {
        unsigned char* decoded_data = decode_line(line);
        decoded_result.append((const char*)decoded_data);
        decoded_result.append('\n');
        free(decoded_data);
    }
    else if (line[0] != '\0')
    {
        decoded_result.append('[');
        decoded_result.append(line);
        decoded_result.append("] ");
    }
}

void decrypt_and_print(const char *label)
{
    if (label[0] != '\0') {
        pm->db->find_label(label, callback_lambda {
            for (int i = 0; i < argc; ++i)
                append_data_to_result(col[i], argv[i]);
            return 0;
        });
    } else {
        pm->db->get_data(callback_lambda {
            for (int i = 0; i < argc; ++i)
                append_data_to_result(col[i], argv[i]);
            return 0;
        });
    }
    auto *dialog = new QDialog(pm, CLOSE_BUTTON);
    auto *layout = new QHBoxLayout(dialog);
    auto *info = new QPlainTextEdit(dialog);

    info->document()->setPlainText(decoded_result);
    info->setReadOnly(true);

    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(info);

    dialog->setWindowTitle("Данные");
    dialog->setLayout(layout);
    dialog->setModal(true);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();

    info->verticalScrollBar()->setValue(0);

    decoded_result = "";
}
void encrypt_and_write(const char* label, uint8_t *data, size_t data_length)
{
    xcrypt_buffer(pm->key, data, data_length);
    char *encoded_data = b64_encode(data, data_length);
    pm->db->insert_data(encoded_data, label);
    free(encoded_data);
}

void xcrypt_buffer(uint8_t *key, uint8_t *buffer, size_t buffer_size)
{
    AES_init_ctx_iv(&ctx, key, aes_iv);
    AES_CTR_xcrypt_buffer(&ctx, buffer, buffer_size);
}