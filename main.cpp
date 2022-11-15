#include "mainwindow.h"

#include <QApplication>
#include <scrollareawidget.h>
int main(int argc, char *argv[])
{
    //QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);

    QApplication a(argc, argv);
    a.setStyleSheet("QWidget{background-color:255,255,0}");


    MainWindow w;
    w.show();
    return a.exec();
}
