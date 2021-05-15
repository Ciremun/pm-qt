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
    if (strcmp(col, "data") == 0) {
        unsigned char* decoded_data = decode_line(line);
        decoded_result.append((const char*)decoded_data);
        decoded_result.append('\n');
        free(decoded_data);
    } else if (line && line[0] != '\0') {
        decoded_result.append('[');
        decoded_result.append(line);
        decoded_result.append("] ");
    }
}

void decrypt_and_print()
{
    pm->db->get_data(callback_lambda {
        (void)e;
        for (int i = 0; i < argc; ++i)
            append_data_to_result(col[i], argv[i]);
        return 0;
    });

    auto *dialog = new QDialog(pm, CLOSE_BUTTON);
    auto *layout = new QVBoxLayout(dialog);

    auto *info = new QPlainTextEdit(dialog);
    info->setReadOnly(true);
    info->document()->setPlainText(decoded_result);
    decoded_result = "";

    auto *search_bar = new QLineEdit(dialog);
    search_bar->setPlaceholderText("Поиск по имени");
    QLineEdit::connect(search_bar, &QLineEdit::textChanged, dialog, [info, search_bar] {
        if (!search_bar->text().isEmpty()) {
            QString search_bar_text = search_bar->text();
            QByteArray ba = search_bar_text.toUtf8();
            const char *label = ba.data();
            pm->db->find_label(label, callback_lambda {
                (void)e;
                for (int i = 0; i < argc; ++i)
                    append_data_to_result(col[i], argv[i]);
                return 0;
            });
        }
        else
        {
            pm->db->get_data(callback_lambda {
                (void)e;
                for (int i = 0; i < argc; ++i)
                    append_data_to_result(col[i], argv[i]);
                return 0;
            });
        }
        info->document()->setPlainText(decoded_result);
        decoded_result = "";
    });

    layout->setMargin(1);
    layout->setSpacing(1);
    layout->addWidget(search_bar);
    layout->addWidget(info);

    dialog->setWindowTitle("Данные");
    dialog->setLayout(layout);
    dialog->setModal(true);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setMinimumSize(400, 300);
    dialog->show();

    search_bar->setFocus();
    info->verticalScrollBar()->setValue(0);
}

void encrypt_and_write(std::string data_str, std::string label_str)
{
    const char *label = NULL;
    if (!label_str.empty()) label = label_str.c_str();
    size_t data_size = data_str.length() + 1;
    uint8_t *data = (uint8_t*)data_str.c_str();

    encrypt_and_write(label, data, data_size);
}

void encrypt_and_write(std::string data_str)
{
    uint8_t *data = (uint8_t*)data_str.c_str();
    size_t data_size = data_str.length() + 1;

    encrypt_and_write(NULL, data, data_size);
}

void encrypt_and_write(const char *label, uint8_t *data, size_t data_size)
{
    xcrypt_buffer(pm->key, data, data_size);

    char *encoded_data = b64_encode(data, data_size);
    pm->db->insert_data(encoded_data, label);
    free(encoded_data);
}

void xcrypt_buffer(uint8_t *key, uint8_t *buffer, size_t buffer_size)
{
    AES_init_ctx_iv(&ctx, key, aes_iv);
    AES_CTR_xcrypt_buffer(&ctx, buffer, buffer_size);
}