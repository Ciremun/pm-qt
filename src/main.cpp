#include <QApplication>
#include <QtWidgets>

#include "app.hpp"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QWidget window;
    PM *pm = new PM(&window, 600, 400);

    pm->show();
    return app.exec();
}