#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:

    void saveTable();

    void addRow();

    void loadTable();

    void closeTable();

    void removeSelection();

    void newTable();

private:
    Ui::MainWindow *ui;


    void setNoTableButtonsDisabled(bool val=true);
};

#endif // MAINWINDOW_H
