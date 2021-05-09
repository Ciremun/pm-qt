#ifndef APP_HPP
#define APP_HPP

#include "db.hpp"

#define CLOSE_BUTTON Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint

class PM : public QWidget
{
public:
    PM(int w, int h);

    bool input_key();

    int window_width;
    int window_height;
    uint8_t *key;
    DB *db;
private:
    QRandomGenerator *rand;
};

extern PM* pm;

#endif // APP_HPP