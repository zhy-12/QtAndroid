#include "mainwindow.h"

#include <QApplication>
#include <scrollareawidget.h>
int main(int argc, char *argv[])
{
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);

    QApplication a(argc, argv);


    MainWindow w;
    w.show();
    return a.exec();
}
