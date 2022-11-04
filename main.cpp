#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug(">> Start MICROWAVE System ");
    int font = QFontDatabase::addApplicationFont(":/Fonts/starmedfont.ttf");

    MainWindow w;
    w.setIDstarmed(font);
    w.show();

    return a.exec();
}
