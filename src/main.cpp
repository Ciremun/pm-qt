#include <QApplication>
#include <QtWidgets>

#include "app.hpp"
#include "external.hpp"

PM* pm;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QPixmap pixmap;
    pixmap.loadFromData(QByteArray::fromBase64(PM_ICON));
    app.setWindowIcon(QIcon(pixmap));

    char *font_data_ptr = (char*)malloc(sizeof(VICTOR_MONO_REGULAR));
    strcpy(font_data_ptr, VICTOR_MONO_REGULAR);

    auto font_data = QByteArray::fromBase64(font_data_ptr);
    int id = QFontDatabase::addApplicationFontFromData(font_data);
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont victor_mono(family, 10);

    victor_mono.setStyleHint(QFont::Monospace);
    app.setFont(victor_mono);

    pm = new PM(600, 400);
    pm->show();
    
    return app.exec();
}