#include <limits>

#include <QtWidgets>

#include "app.hpp"
#include "encrypt.hpp"
#include "lang.hpp"
#include "rand.hpp"

extern const char **lang;
extern const char *lang_en[lang_string_kind::count];
extern const char *lang_ru[lang_string_kind::count];

PM::PM(int w, int h) : QWidget(), window_width(w), window_height(h), key(NULL)
{
    db = new DB("data.db");
    rand = new QRandomGenerator();

    setWindowTitle(lang[PASSWORD_MANAGER]);

    auto *main_layout = new QVBoxLayout();
    auto *top_layout = new QGridLayout();
    auto *frame_layout = new QHBoxLayout();

    auto *close_button = new QPushButton();
    close_button->setFixedSize(32, 32);
    close_button->setStyleSheet(
        "QPushButton {background-color: #111111;}"
        "QPushButton:hover {background-color: #cc0c1c;}");
    close_button->setIcon(
        style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    connect(close_button, SIGNAL(clicked()), this, SLOT(close()));

    frame_layout->setMargin(0);
    frame_layout->setSpacing(0);
    frame_layout->addWidget(close_button, 0, Qt::AlignRight);

    auto *encrypt_button = new QPushButton(lang[WRITE_DATA], this);
    encrypt_button->setMinimumSize(QSize(40, 40));
    connect(
        encrypt_button, &QPushButton::pressed, this,
        [this]
        {
            if (!input_key())
                return;

            auto *input_dialog = new QDialog(this, CLOSE_BUTTON);
            auto *input_dialog_layout = new QVBoxLayout(input_dialog);
            auto *input_fields_layout = new QVBoxLayout(input_dialog);
            auto *buttons_at_the_bottom_layout = new QHBoxLayout(input_dialog);
            auto *find_label_label = new QLabel(lang[ENTRY_NAME], input_dialog);

            auto *find_label_field = new QLineEdit(input_dialog);
            find_label_field->setPlaceholderText(lang[OPT]);

            auto *data_label = new QLabel(lang[ENTRY_DATA], input_dialog);

            auto *data_field = new QLineEdit(input_dialog);
            data_field->setPlaceholderText(lang[ENTER_DATA]);

            auto *ok_button = new QPushButton("OK", input_dialog);
            auto *cancel_button = new QPushButton(lang[CANCEL], input_dialog);

            input_fields_layout->addWidget(find_label_label);
            input_fields_layout->addWidget(find_label_field);
            input_fields_layout->addWidget(data_label);
            input_fields_layout->addWidget(data_field);

            connect(cancel_button, SIGNAL(clicked()), input_dialog,
                    SLOT(close()));
            connect(ok_button, &QPushButton::pressed, input_dialog,
                    [=]
                    {
                        if (data_field->text().isEmpty())
                        {
                            input_dialog->close();
                            return;
                        }
                        std::string label =
                            find_label_field->text().toStdString();
                        std::string data = data_field->text().toStdString();
                        encrypt_and_write(data, label);
                        input_dialog->close();
                    });

            buttons_at_the_bottom_layout->addWidget(ok_button, 0,
                                                    Qt::AlignRight);
            buttons_at_the_bottom_layout->addWidget(cancel_button, 0,
                                                    Qt::AlignRight);

            input_dialog_layout->addLayout(input_fields_layout);
            input_dialog_layout->addLayout(buttons_at_the_bottom_layout);

            input_dialog->setWindowTitle(lang[NEW_ENTRY]);
            input_dialog->setModal(true);
            input_dialog->setAttribute(Qt::WA_DeleteOnClose);
            input_dialog->show();
        });

    auto *decrypt_button = new QPushButton(lang[READ_DATA], this);
    decrypt_button->setMinimumSize(QSize(40, 40));
    connect(decrypt_button, &QPushButton::pressed, this,
            [this]
            {
                if (!input_key())
                    return;
                decrypt_and_print();
            });

    auto *change_key_button = new QPushButton(lang[CHANGE_CYPHER_KEY], this);
    change_key_button->setMinimumSize(QSize(40, 40));
    connect(change_key_button, &QPushButton::pressed, this,
            [this]
            {
                bool ok;
                QString text = QInputDialog::getText(
                    this, lang[NEW_CYPHER_KEY], nullptr, QLineEdit::Password,
                    nullptr, &ok, CLOSE_BUTTON);
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

    auto *about_button = new QPushButton(lang[ABOUT_PROGRAM], this);
    about_button->setMinimumSize(QSize(50, 15));
    connect(about_button, &QPushButton::pressed, this,
            [this]
            {
                auto *dialog = new QDialog(this, CLOSE_BUTTON);
                auto *about_dialog_layout = new QHBoxLayout(dialog);
                std::string about_dialog_text =
                    std::string(lang[PASSWORD_MANAGER]) + std::string("\n") +
                    std::string("https://github.com/Ciremun/pm-qt\n\n"
                                "Victor Mono Font\n"
                                "https://github.com/rubjo/victor-mono\n\n"
                                "Tiny AES\n"
                                "https://github.com/kokke/tiny-AES-c\n\n"
                                "b64.c\n"
                                "https://github.com/littlstar/b64.c\n\n");
                auto *about_dialog_label =
                    new QLabel(about_dialog_text.c_str(), dialog);
                about_dialog_label->setTextInteractionFlags(
                    Qt::TextSelectableByMouse);

                about_dialog_layout->setAlignment(Qt::AlignTop);
                about_dialog_layout->addWidget(about_dialog_label);

                dialog->setWindowTitle(lang[ABOUT_PROGRAM]);
                dialog->setLayout(about_dialog_layout);
                dialog->setModal(true);
                dialog->setFixedSize(300, 250);
                dialog->setAttribute(Qt::WA_DeleteOnClose);
                dialog->show();
            });

    auto *clear_button = new QPushButton(lang[WIPE_DATA], this);
    clear_button->setMinimumSize(QSize(40, 40));
    connect(
        clear_button, &QPushButton::pressed, this,
        [this]
        {
            auto reply =
                QMessageBox::question(this, lang[WIPE_DATA], lang[ARE_YOU_SURE],
                                      QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                db->clear_data();
                QMessageBox::information(this, lang[INFO], lang[OP_SUCCESS]);
            }
        });

    auto *generate_data_button = new QPushButton(lang[GENERATE_PASSWORD], this);
    generate_data_button->setMinimumSize(QSize(40, 40));
    connect(
        generate_data_button, &QPushButton::pressed, this,
        [this]
        {
            if (!input_key())
                return;

            auto *input_dialog = new QDialog(this, CLOSE_BUTTON);
            input_dialog->setMinimumWidth(280);

            auto *input_dialog_layout = new QVBoxLayout(input_dialog);
            auto *input_fields_layout = new QVBoxLayout(input_dialog);
            auto *buttons_at_the_bottom_layout = new QHBoxLayout(input_dialog);
            auto *password_name_label =
                new QLabel(lang[PASSWORD_NAME], input_dialog);

            auto *password_name_field = new QLineEdit(input_dialog);
            password_name_field->setPlaceholderText(lang[OPT]);

            auto *password_length_label =
                new QLabel(lang[PASSWORD_LENGTH], input_dialog);

            auto *password_length_field = new QLineEdit(input_dialog);
            password_length_field->setValidator(new QIntValidator(
                1, std::numeric_limits<int>::max(), input_dialog));
            password_length_field->setPlaceholderText(
                lang[DEFAULT_PASSWORD_LENGTH]);

            auto *ok_button = new QPushButton("OK", input_dialog);
            auto *cancel_button = new QPushButton(lang[CANCEL], input_dialog);

            input_fields_layout->addWidget(password_name_label);
            input_fields_layout->addWidget(password_name_field);
            input_fields_layout->addWidget(password_length_label);
            input_fields_layout->addWidget(password_length_field);

            connect(cancel_button, SIGNAL(clicked()), input_dialog,
                    SLOT(close()));
            connect(
                ok_button, &QPushButton::pressed, input_dialog,
                [=]
                {
                    QString password_length_str = password_length_field->text();
                    unsigned int size;
                    bool ok = false;
                    if (!password_length_str.isEmpty())
                    {
                        unsigned int password_length =
                            password_length_str.toUInt(&ok, 10);
                        if (ok)
                            size = password_length;
                        else
                            QMessageBox::critical(
                                this, lang[ERR],
                                lang[ERR_COULD_NOT_CONVERT_PASSWORD_LENGTH]);
                    }
                    if (!ok)
                    {
                        size = rand->bounded(16, 32);
                    }
                    std::string random_str = random_string(size);
                    QString password_name_qstr = password_name_field->text();
                    std::string password_name_str =
                        password_name_qstr.toStdString();
                    encrypt_and_write(random_str, password_name_str);
                    input_dialog->close();
                });

            buttons_at_the_bottom_layout->addStretch(5);
            buttons_at_the_bottom_layout->addWidget(ok_button, 0,
                                                    Qt::AlignRight);
            buttons_at_the_bottom_layout->addWidget(cancel_button, 0,
                                                    Qt::AlignRight);

            input_dialog_layout->addLayout(input_fields_layout);
            input_dialog_layout->addLayout(buttons_at_the_bottom_layout);

            input_dialog->setWindowTitle(lang[GENERATE_PASSWORD]);
            input_dialog->setModal(true);
            input_dialog->setAttribute(Qt::WA_DeleteOnClose);
            input_dialog->show();
        });

    top_layout->setAlignment(Qt::AlignTop);
    top_layout->addWidget(encrypt_button);
    top_layout->addWidget(decrypt_button);
    top_layout->addWidget(generate_data_button);
    top_layout->addWidget(change_key_button);
    top_layout->addWidget(clear_button);
    top_layout->setContentsMargins(25, 10, 25, 0);

    auto update_widget = [=]()
    {
        encrypt_button->setText(lang[WRITE_DATA]);
        decrypt_button->setText(lang[READ_DATA]);
        generate_data_button->setText(lang[GENERATE_PASSWORD]);
        change_key_button->setText(lang[CHANGE_CYPHER_KEY]);
        clear_button->setText(lang[WIPE_DATA]);
        about_button->setText(lang[ABOUT_PROGRAM]);
    };

    auto *set_lang_en_button = new QPushButton("EN", this);
    connect(set_lang_en_button, &QPushButton::pressed, this,
            [=]
            {
                lang = lang_en;
                update_widget();
            });

    auto *set_lang_ru_button = new QPushButton("RU", this);
    connect(set_lang_ru_button, &QPushButton::pressed, this,
            [=]
            {
                lang = lang_ru;
                update_widget();
            });

    auto *bottom_left_layout = new QHBoxLayout();
    bottom_left_layout->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    bottom_left_layout->addWidget(set_lang_en_button);
    bottom_left_layout->addWidget(set_lang_ru_button);

    auto *bottom_right_layout = new QHBoxLayout();
    bottom_right_layout->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    bottom_right_layout->addWidget(about_button);

    auto *bottom_layout = new QHBoxLayout();
    bottom_layout->addLayout(bottom_left_layout);
    bottom_layout->addLayout(bottom_right_layout);
    bottom_layout->setContentsMargins(10, 10, 10, 10);

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
    QString text =
        QInputDialog::getText(this, lang[CYPHER_KEY], nullptr,
                              QLineEdit::Password, nullptr, &ok, CLOSE_BUTTON);
    if (ok && !text.isEmpty())
    {
        size_t key_len = text.length() + 1;
        if (key_len > 128)
            key = (uint8_t *)malloc(key_len);
        else
            key = (uint8_t *)calloc(1, 128);
        QByteArray ba = text.toUtf8();
        memcpy(key, ba.data(), key_len);
        return true;
    }
    QMessageBox::critical(this, lang[ERR],
                          lang[ERR_THIS_OPERATION_REQUIRES_A_CYPHER_KEY]);
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
    move(event->globalX() - mouseClick_X_Coordinate,
         event->globalY() - mouseClick_Y_Coordinate);
}

PM::~PM() { delete db; }