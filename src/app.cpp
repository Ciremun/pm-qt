#include <QtWidgets>

#include "app.hpp"
#include "common.hpp"

PM::PM(QWidget *parent, int w, int h) : QWidget(parent), window_width(w), window_height(h)
{
    setWindowTitle("password-manager");

    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->setMargin(0);
    main_layout->setSpacing(0);

    uint8_t *key = (uint8_t *)calloc(1, 256);
    memcpy(key, "test key", 9);
    uint8_t *data = (uint8_t *)calloc(1, 256);
    memcpy(data, "test data", 10);

    encrypt_and_write(key, data, 10);
    char **result = decrypt_and_print(key, NULL);

    QLabel *label = new QLabel(result[0]);
    main_layout->addWidget(label);

    setLayout(main_layout);
    setWindowFlags(Qt::Window);
    setFixedSize(QSize(window_width, window_height));
}