#include <QApplication>
#include <QFileDialog>
#include <iostream>
#include <QFontDatabase>
#include "mainwindow.h"

using namespace std;


int main(int argc, char *argv[]) {
    QFontDatabase::addApplicationFont(":/fonts/Orbitron-Regular.ttf");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
