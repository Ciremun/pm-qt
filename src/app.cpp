#include <QtWidgets>

#include "app.hpp"
#include "encrypt.hpp"
#include "rand.hpp"

PM::PM(int w, int h) : QWidget(), window_width(w), window_height(h), key(NULL)
{
    db = new DB("data.db");
    rand = new QRandomGenerator();

    setWindowTitle("Менеджер паролей");

    auto *main_layout = new QVBoxLayout();
    auto *top_layout = new QGridLayout();
    auto *frame_layout = new QHBoxLayout();

    auto *close_button = new QPushButton();
    close_button->setFixedSize(32, 32);
    close_button->setStyleSheet("QPushButton {background-color: #111111;}"
                                "QPushButton:hover {background-color: #cc0c1c;}");
    close_button->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    connect(close_button, SIGNAL(clicked()), this, SLOT(close()));

    frame_layout->setMargin(0);
    frame_layout->setSpacing(0);
    frame_layout->addWidget(close_button, 0, Qt::AlignRight);

    auto *encrypt_button = new QPushButton("Запись данных", this);
    encrypt_button->setMinimumSize(QSize(40, 40));
    connect(encrypt_button, &QPushButton::pressed, this, [this] {
        if (!input_key())
            return;

        auto *input_dialog = new QDialog(this, CLOSE_BUTTON);
        auto *input_dialog_layout = new QVBoxLayout(input_dialog);
        auto *input_fields_layout = new QVBoxLayout(input_dialog);
        auto *buttons_at_the_bottom_layout = new QHBoxLayout(input_dialog);
        auto *find_label_label = new QLabel("Имя записи", input_dialog);
        auto *find_label_field = new QLineEdit(input_dialog);
        auto *data_label = new QLabel("Текст записи", input_dialog);
        auto *data_field = new QLineEdit(input_dialog);
        auto *ok_button = new QPushButton("OK", input_dialog);
        auto *cancel_button = new QPushButton("Cancel", input_dialog);

        input_fields_layout->addWidget(find_label_label);
        input_fields_layout->addWidget(find_label_field);
        input_fields_layout->addWidget(data_label);
        input_fields_layout->addWidget(data_field);

        connect(cancel_button, SIGNAL(clicked()), input_dialog, SLOT(close()));
        connect(ok_button, &QPushButton::pressed, input_dialog, [=] {
            if (data_field->text().isEmpty())
            {
                input_dialog->close();
                return;
            }
            std::string label = find_label_field->text().toStdString();
            std::string data = data_field->text().toStdString();
            encrypt_and_write(data, label);
            input_dialog->close();
        });

        buttons_at_the_bottom_layout->addWidget(ok_button, 0, Qt::AlignRight);
        buttons_at_the_bottom_layout->addWidget(cancel_button, 0, Qt::AlignRight);

        input_dialog_layout->addLayout(input_fields_layout);
        input_dialog_layout->addLayout(buttons_at_the_bottom_layout);

        input_dialog->setWindowTitle("Новая запись");
        input_dialog->setModal(true);
        input_dialog->setAttribute(Qt::WA_DeleteOnClose);
        input_dialog->show();
    });

    auto *decrypt_button = new QPushButton("Чтение данных", this);
    decrypt_button->setMinimumSize(QSize(40, 40));
    connect(decrypt_button, &QPushButton::pressed, this, [this] {
        if (!input_key()) return;
        decrypt_and_print();
    });

    auto *change_key_button = new QPushButton("Сменить ключ шифрования", this);
    change_key_button->setMinimumSize(QSize(40, 40));
    connect(change_key_button, &QPushButton::pressed, this, [this] {
        bool ok;
        QString text = QInputDialog::getText(this, "Новый ключ шифрования?", nullptr, QLineEdit::Password, nullptr, &ok, CLOSE_BUTTON);
        if (ok && !text.isEmpty())
        {
            free(key);
            key = NULL;
            size_t key_len = text.length() + 1;
            if (key_len > 128)
                key = (uint8_t *)malloc(key_len);
            else
                key = (uint8_t *)calloc(1, 128);
            QByteArray ba = text.toUtf8();
            memcpy(key, ba.data(), key_len);
        }
    });

    auto *about_button = new QPushButton("О программе", this);
    about_button->setMinimumSize(QSize(50, 15));
    connect(about_button, &QPushButton::pressed, this, [this] {
        auto *dialog = new QDialog(this, CLOSE_BUTTON);
        auto *about_dialog_layout = new QHBoxLayout(dialog);
        auto *about_dialog_text = new QLabel("Менеджер паролей\n"
                                             "https://github.com/Ciremun/pm-qt\n\n"
                                             "Victor Mono Font\n"
                                             "https://github.com/rubjo/victor-mono\n\n"
                                             "Tiny AES\n"
                                             "https://github.com/kokke/tiny-AES-c\n\n"
                                             "b64.c\n"
                                             "https://github.com/littlstar/b64.c\n\n",
                                             dialog);
        about_dialog_text->setTextInteractionFlags(Qt::TextSelectableByMouse);

        about_dialog_layout->setAlignment(Qt::AlignTop);
        about_dialog_layout->addWidget(about_dialog_text);

        dialog->setWindowTitle("О программе");
        dialog->setLayout(about_dialog_layout);
        dialog->setModal(true);
        dialog->setFixedSize(300, 250);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    });

    auto *clear_button = new QPushButton("Удалить данные", this);
    clear_button->setMinimumSize(QSize(40, 40));
    connect(clear_button, &QPushButton::pressed, this, [this] {
        auto reply = QMessageBox::question(this, "Удалить данные", "Вы уверены?",
                                           QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            db->clear_data();
            QMessageBox::information(this, "Информация", "Операция была выполнена успешно.");
        }
    });

    auto *generate_data_button = new QPushButton("Сгенерировать пароль", this);
    generate_data_button->setMinimumSize(QSize(40, 40));
    connect(generate_data_button, &QPushButton::pressed, this, [this] {
        if (!input_key())
            return;
        bool ok;
        QString label = QInputDialog::getText(this, "Поиск по имени", nullptr, QLineEdit::Normal, nullptr, &ok, CLOSE_BUTTON);
        int size = rand->bounded(16, 32);
        if (ok) encrypt_and_write(random_string(size), label.toStdString());
    });

    top_layout->setAlignment(Qt::AlignTop);
    top_layout->addWidget(encrypt_button);
    top_layout->addWidget(decrypt_button);
    top_layout->addWidget(generate_data_button);
    top_layout->addWidget(change_key_button);
    top_layout->addWidget(clear_button);
    top_layout->setContentsMargins(25, 10, 25, 0);

    auto *bottom_layout = new QHBoxLayout();
    bottom_layout->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    bottom_layout->addWidget(about_button);
    bottom_layout->setContentsMargins(0, 0, 10, 10);

    main_layout->addLayout(frame_layout);
    main_layout->addLayout(top_layout);
    main_layout->addLayout(bottom_layout);
    main_layout->setMargin(0);

    setLayout(main_layout);
    setFixedSize(QSize(window_width, window_height));
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

bool PM::input_key()
{
    if (key != NULL)
        return true;
    bool ok;
    QString text = QInputDialog::getText(this, "Ключ шифрования?", nullptr, QLineEdit::Password, nullptr, &ok, CLOSE_BUTTON);
    if (ok && !text.isEmpty()) {
        size_t key_len = text.length() + 1;
        if (key_len > 128)
            key = (uint8_t *)malloc(key_len);
        else
            key = (uint8_t *)calloc(1, 128);
        QByteArray ba = text.toUtf8();
        memcpy(key, ba.data(), key_len);
        return true;
    }
    QMessageBox::critical(this, "Ошибка", "Для этой операции необходимо ввести ключ шифрования.");
    return false;
}

void PM::paintEvent(QPaintEvent *event)
{
    (void)event;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(60, 60, 60, 228));
    painter.drawRect(0, 0, width(), height());
}

void PM::mousePressEvent(QMouseEvent *event)
{
    mouseClick_X_Coordinate = event->x();
    mouseClick_Y_Coordinate = event->y();
}

void PM::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalX() - mouseClick_X_Coordinate, event->globalY() - mouseClick_Y_Coordinate);
}