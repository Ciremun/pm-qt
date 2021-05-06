#ifndef APP_HPP
#define APP_HPP

#include "db.hpp"

class PM : public QWidget
{
public:
    PM(int w, int h);

    bool input_key();

private:
    int window_width;
    int window_height;
    uint8_t *key;
    DB *db;
};

#endif // APP_HPP