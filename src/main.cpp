#include <QApplication>
#include <QtWidgets>

#include "app.hpp"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    PM *pm = new PM(600, 400);

    pm->show();
    return app.exec();
}