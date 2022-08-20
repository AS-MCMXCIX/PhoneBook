#include <QApplication>
#include <QFileDialog>
#include <iostream>
#include <QFontDatabase>
#include "mainwindow.h"
#include "Loader.h"

using namespace std;


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    int id = QFontDatabase::addApplicationFont(":/fonts/Orbitron-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family);
    QApplication::setFont(font);
    MainWindow w;
    w.show();
    w.setStyleSheet(Loader::readFileAsQString(":styles/main.qss"));
    return QApplication::exec();
}
