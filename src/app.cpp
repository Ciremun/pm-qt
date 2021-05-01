#include <QtWidgets>

#include "app.hpp"
#include "common.hpp"

PM::PM(QWidget *parent, int w, int h) : QWidget(parent), window_width(w), window_height(h)
{
    setWindowTitle("password-manager");

    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->setMargin(0);
    main_layout->setSpacing(0);

    uint8_t* key = (uint8_t*)calloc(1, 256);
    memcpy(key, "secret_key", 11);

    QPushButton *encrypt_test_button = new QPushButton("test encrypt", this);
    connect(encrypt_test_button, &QPushButton::pressed, this, [this, key] {
        uint8_t* test_data = (uint8_t*)calloc(1, 256);
        memcpy(test_data, (uint8_t*)"test_data 69", 13);
        encrypt_and_write(key, test_data, 13);
        QMessageBox::information(this, "Info", "done");
        free(test_data);
    });
    QPushButton *decrypt_test_button = new QPushButton("test decrypt", this);
    connect(decrypt_test_button, &QPushButton::pressed, this, [this, key] {
        size_t total_lines = 0;
        char **result = decrypt_and_print(key, &total_lines, NULL);
        QString message;
        for (size_t i = 0; i < total_lines; i++)
        {
            message.append(QString(result[i]) + QChar('\n'));
            free(result[i]);
        }
        QMessageBox::information(this, "Info", message);
        free(result);
    });
    main_layout->addWidget(encrypt_test_button);
    main_layout->addWidget(decrypt_test_button);

    setLayout(main_layout);
    setWindowFlags(Qt::Window);
    setFixedSize(QSize(window_width, window_height));
}