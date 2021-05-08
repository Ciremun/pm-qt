#include <QApplication>
#include <QtWidgets>

#include "app.hpp"

PM* pm;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    pm = new PM(600, 400);
    pm->show();
    return app.exec();
}