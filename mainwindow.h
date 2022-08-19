#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

public slots:

    void saveTable();

    void addRow();

    void loadTable();

    void closeTable();

    void removeSelection();

    void newTable();

private:
    Ui::MainWindow *ui;
    QString currentTablePath = nullptr;

    void disableTableButtons(bool val = true);

    bool tableModelExists();

    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
