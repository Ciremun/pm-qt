#ifndef APP_HPP
#define APP_HPP

#include <QtWidgets>

#include "db.hpp"

#define CLOSE_BUTTON                                                           \
    Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint

class PM : public QWidget
{
    Q_OBJECT
public:
    PM(int w, int h);
    virtual ~PM();

    bool input_key();

    int window_width;
    int window_height;
    uint8_t *key;
    DB *db;

private:
    QRandomGenerator *rand;
    int mouseClick_X_Coordinate;
    int mouseClick_Y_Coordinate;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

extern PM *pm;

#endif // APP_HPP