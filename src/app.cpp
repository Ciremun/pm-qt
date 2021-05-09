#include <QtWidgets>

#include "app.hpp"
#include "common.hpp"

PM::PM(int w, int h) : QWidget(), window_width(w), window_height(h), key(NULL)
{
    db = new DB("uwu.db");

    setWindowTitle("Менеджер паролей");

    auto *main_layout = new QHBoxLayout();
    auto *top_layout = new QHBoxLayout();

    auto *encrypt_test_button = new QPushButton("Запись данных", this);
    encrypt_test_button->setMinimumSize(QSize(40, 40));
    connect(encrypt_test_button, &QPushButton::pressed, this, [this] {
        if(!input_key()) return;
        bool ok;
        QString text = QInputDialog::getText(this, "Что записать?", nullptr, QLineEdit::Normal, nullptr, &ok, CLOSE_BUTTON);
        if (ok && !text.isEmpty())
            encrypt_and_write((uint8_t*)text.toLocal8Bit().data(), text.length() + 1);
    });

    auto *decrypt_test_button = new QPushButton("Чтение данных", this);
    decrypt_test_button->setMinimumSize(QSize(40, 40));
    connect(decrypt_test_button, &QPushButton::pressed, this, [this] {
        if(input_key()) decrypt_and_print(NULL);
    });

    auto *change_key_button = new QPushButton("Сменить ключ", this);
    change_key_button->setMinimumSize(QSize(40, 40));
    connect(change_key_button, &QPushButton::pressed, this, [this] {
        bool ok;
        QString text = QInputDialog::getText(this, "Новый ключ?", nullptr, QLineEdit::Password, nullptr, &ok, CLOSE_BUTTON);
        if (ok && !text.isEmpty())
        {
            free(key);
            key = NULL;
            size_t key_len = text.length() + 1;
            if (key_len > 128) key = (uint8_t*)malloc(key_len);
            else               key = (uint8_t*)calloc(1, 128);
            memcpy(key, text.toLocal8Bit().data(), key_len);
        }
    });

    auto *about_button = new QPushButton("О программе", this);
    about_button->setMinimumSize(QSize(50, 15));
    connect(about_button, &QPushButton::pressed, this, [this] {
        auto *about_dialog_text = new QLabel(""
                                             "Менеджер паролей\n"
                                             "https://github.com/Ciremun/pm-qt"
                                             "");
        about_dialog_text->setTextInteractionFlags(Qt::TextSelectableByMouse);

        auto *about_dialog_layout = new QHBoxLayout();
        about_dialog_layout->setAlignment(Qt::AlignTop);
        about_dialog_layout->addWidget(about_dialog_text);

        auto *dialog = new QDialog(this, CLOSE_BUTTON);
        dialog->setWindowTitle("О программе");
        dialog->setLayout(about_dialog_layout);
        dialog->setModal(true);
        dialog->setFixedSize(250, 250);
        dialog->show();
    });

    auto *clear_button = new QPushButton("Удалить данные", this);
    clear_button->setMinimumSize(QSize(40, 40));
    connect(clear_button, &QPushButton::pressed, this, [this] {
        db->clear_data();
        QMessageBox::information(this, "Информация", "Операция была выполнена успешно.");
    });

    top_layout->setAlignment(Qt::AlignTop);
    top_layout->addWidget(encrypt_test_button);
    top_layout->addWidget(decrypt_test_button);
    top_layout->addWidget(change_key_button);
    top_layout->addWidget(clear_button);
    top_layout->addStretch(8);

    auto *bottom_layout = new QHBoxLayout();
    bottom_layout->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    bottom_layout->addWidget(about_button);

    main_layout->addLayout(top_layout);
    main_layout->addLayout(bottom_layout);

    setLayout(main_layout);
    setWindowFlags(Qt::Window);
    setFixedSize(QSize(window_width, window_height));
}

bool PM::input_key()
{
    if (key != NULL) return true;
    bool ok;
    QString text = QInputDialog::getText(this, "Ключ?", nullptr, QLineEdit::Password, nullptr, &ok, CLOSE_BUTTON);
    if (ok && !text.isEmpty()) {
        size_t key_len = text.length() + 1;
        if (key_len > 128) key = (uint8_t*)malloc(key_len);
        else               key = (uint8_t*)calloc(1, 128);
        memcpy(key, text.toLocal8Bit().data(), key_len);
        return true;
    }
    QMessageBox::critical(this, "Ошибка", "Для этой операции необходимо ввести ключ.");
    return false;
}