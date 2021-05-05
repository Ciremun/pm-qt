#include <QtWidgets>

#include "app.hpp"
#include "common.hpp"

#define CLOSE_BUTTON Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint

PM::PM(int w, int h) : QWidget(), window_width(w), window_height(h), key(NULL)
{
    setWindowTitle("password-manager");

    auto *main_layout = new QHBoxLayout();

    auto *encrypt_test_button = new QPushButton("test encrypt", this);
    encrypt_test_button->setMinimumSize(QSize(40, 40));
    connect(encrypt_test_button, &QPushButton::pressed, this, [this] {
        if(!input_key()) return;
        uint8_t* test_data = (uint8_t*)calloc(1, 256);
        memcpy(test_data, (uint8_t*)"test_data 69", 13);
        encrypt_and_write(key, test_data, 13);
        free(test_data);
    });

    auto *decrypt_test_button = new QPushButton("test decrypt", this);
    decrypt_test_button->setMinimumSize(QSize(40, 40));
    connect(decrypt_test_button, &QPushButton::pressed, this, [this] {
        if(!input_key()) return;

        QString result = decrypt_and_print(key, NULL);

        auto *info = new QPlainTextEdit();
        info->document()->setPlainText(result);
        info->setReadOnly(true);

        auto *layout = new QHBoxLayout();
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(info);

        auto *dialog = new QDialog(this, CLOSE_BUTTON);
        dialog->setWindowTitle("Decrypted data");
        dialog->setLayout(layout);
        dialog->setModal(true);
        dialog->show();

        info->verticalScrollBar()->setValue(0);
    });

    main_layout->setAlignment(Qt::AlignTop);
    main_layout->addWidget(encrypt_test_button);
    main_layout->addWidget(decrypt_test_button);
    main_layout->addStretch(8);

    setLayout(main_layout);
    setWindowFlags(Qt::Window);
    setFixedSize(QSize(window_width, window_height));
}

bool PM::input_key()
{
    if (key != NULL) return true;
    bool ok;
    QString text = QInputDialog::getText(this, "password?", nullptr, QLineEdit::Password, nullptr, &ok, CLOSE_BUTTON);
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