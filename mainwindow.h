#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    bool tableChanged();

    void setTableChanged(bool val);
public slots:

    void saveTable();

    void addRow();

    void loadTable();

    int closeTable();

    void removeSelection();

    void newTable();


private:
    Ui::MainWindow *ui;
    QString currentTablePath = nullptr;
    bool tableHasChanged = false;

    void disableTableButtons(bool val = true);

    bool tableModelExists();

    void closeEvent(QCloseEvent *event) override;

    QStandardItemModel *getModel();

    QSortFilterProxyModel *setupProxyModel(QStandardItemModel &model);
};

#endif // MAINWINDOW_H
