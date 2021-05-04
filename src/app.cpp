#include <QtWidgets>

#include "app.hpp"
#include "common.hpp"

PM::PM(QWidget *parent, int w, int h) : QWidget(parent), window_width(w), window_height(h), key(NULL)
{
    setWindowTitle("password-manager");

    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->setMargin(0);
    main_layout->setSpacing(0);

    QPushButton *encrypt_test_button = new QPushButton("test encrypt", this);
    connect(encrypt_test_button, &QPushButton::pressed, this, [this] {
        if(!input_key()) return;
        uint8_t* test_data = (uint8_t*)calloc(1, 256);
        memcpy(test_data, (uint8_t*)"test_data 69", 13);
        encrypt_and_write(key, test_data, 13);
        free(test_data);
    });
    QPushButton *decrypt_test_button = new QPushButton("test decrypt", this);
    connect(decrypt_test_button, &QPushButton::pressed, this, [this] {
        if(!input_key()) return;
        std::vector<const char*> *v = decrypt_and_print(key, NULL);
        QString result;
        for (auto &i : *v)
        {
            result.append(i);
            result.append('\n');
        }
        auto *info = new QPlainTextEdit();
        info->document()->setPlainText(result);
        info->setReadOnly(true);
        info->setWindowTitle("Decrypted data");
        info->verticalScrollBar()->setValue(0);
        auto *dialog = new QDialog(this);
        auto *layout = new QHBoxLayout();
        layout->addWidget(info);
        dialog->setLayout(layout);
        dialog->show();
    });
    main_layout->addWidget(encrypt_test_button);
    main_layout->addWidget(decrypt_test_button);

    setLayout(main_layout);
    setWindowFlags(Qt::Window);
    setFixedSize(QSize(window_width, window_height));
}

bool PM::input_key()
{
    if (key != NULL) return true;
    bool ok;
    QString text = QInputDialog::getText(this, "password?", nullptr, QLineEdit::Normal, nullptr, &ok);
    if (ok && !text.isEmpty())
    {
        size_t key_len = text.length() + 1;
        if (key_len > 128) key = (uint8_t*)malloc(key_len);
        else               key = (uint8_t*)calloc(1, 128);
        memcpy(key, text.toLocal8Bit().data(), key_len);
        return true;
    }
    QMessageBox::critical(this, "Error", "this operation requires password");
    return false;
}