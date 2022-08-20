#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QTextStream>
#include <QCloseEvent>
#include <QMessageBox>
#include <QScrollBar>
#include <QLineEdit>
#include <iostream>
#include <QFile>
#include "mainwindow.h"
#include "Loader.h"
#include "./ui_mainwindow.h"
#include "PhoneBookInputValidator.h"

#define print(string) cout << string <<endl

using namespace std;

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    disableTableButtons();
    ui->phoneNumbers->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->phoneNumbers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(ui->removeSelectionButton, &QPushButton::clicked, this, &MainWindow::removeSelection);
    connect(ui->newTableButton, &QPushButton::clicked, this, &MainWindow::newTable);
    connect(ui->loadDataButton, &QPushButton::clicked, this, &MainWindow::loadTable);
    connect(ui->addRowButton, &QPushButton::clicked, this, &MainWindow::addRow);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::closeTable);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveTable);
    ui->phoneNumbers->setStyleSheet(Loader::readFileAsQString(":styles/table.qss"));
    ui->phoneNumbers->verticalScrollBar()->setStyleSheet(Loader::readFileAsQString(":styles/scrollbar.qss"));
    auto *itDelegate = new PhoneBookInputValidator;
    ui->phoneNumbers->setItemDelegate(itDelegate);
}

void MainWindow::disableTableButtons(bool val) {
    ui->saveButton->setDisabled(val);
    ui->addRowButton->setDisabled(val);
    ui->removeSelectionButton->setDisabled(val);
    ui->closeButton->setDisabled(val);
}

QSortFilterProxyModel *MainWindow::setupProxyModel(QStandardItemModel &model) {
    auto *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(&model);
    return proxyModel;
}

void MainWindow::loadTable() {
    QString path = QFileDialog::getOpenFileName(this,
                                                QFileDialog::tr("Open CSV"), "./",
                                                QFileDialog::tr("CSV File (*.csv)"));

    // no file selected
    if (path.isEmpty())
        return;

    auto &model = *new QStandardItemModel;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        int lineIndex = 0;                     // file line counter
        QTextStream in(&file);                 // read to text stream

        while (!in.atEnd()) {
            QString fileLine = in.readLine();
            cout << fileLine.toStdString() << endl;
            QStringList lineToken = fileLine.split(",", Qt::SkipEmptyParts);

            for (int j = 0; j < lineToken.size(); j++) {
                const QString &value = lineToken.at(j);
                auto *item = new QStandardItem(value);
                if (lineIndex == 0)
                    model.setHorizontalHeaderItem(j, item);
                else
                    model.setItem(lineIndex - 1, j, item);
            }
            lineIndex++;
        }
        file.close();
    }
    ui->phoneNumbers->setModel(setupProxyModel(model));
    disableTableButtons(false);
    setTableChanged(false);
    currentTablePath = path;
}

void MainWindow::newTable() {
    closeTable();
    auto &model = *new QStandardItemModel;
    QString line("Id,Name,Tel");
    QStringList lineToken = line.split(",");
    for (int j = 0; j < lineToken.size(); j++) {
        const QString &value = lineToken.at(j);
        auto *item = new QStandardItem(value);
        model.setHorizontalHeaderItem(j, item);
    }
    ui->phoneNumbers->setModel(setupProxyModel(model));
    disableTableButtons(false);
}

bool MainWindow::tableModelExists() {
    return ui->phoneNumbers->model() != nullptr;
}

void MainWindow::removeSelection() {
    if (!tableModelExists())
        return;
    auto *model = getModel();
    QItemSelectionModel &select = *ui->phoneNumbers->selectionModel();
    if (select.hasSelection()) {
        auto selectedRows = select.selectedRows();
        QList<int> rowIndexes;
        for (auto &row : selectedRows) {
            rowIndexes << row.row();
        }
        auto cmp = [](int a, int b) {
            return a > b;
        };
        sort(rowIndexes.begin(), rowIndexes.end(), cmp);
        for (auto &i : rowIndexes)
            model->removeRow(i);
        setTableChanged(true);
    }
}

bool MainWindow::tableChanged() {
    if (ui->phoneNumbers->model() == nullptr)
        return false;
    return tableHasChanged;
}

int MainWindow::closeTable() {
    int ret = QMessageBox::Discard;
    if (!tableChanged()) {
        ret = QMessageBox::Discard;
    } else {
        QMessageBox msgBox;
        msgBox.setStyleSheet(Loader::readFileAsQString(":styles/exitDialog.qss"));
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        ret = msgBox.exec();
    }
    switch (ret) {
        case QMessageBox::Discard:
            ui->phoneNumbers->setModel(nullptr);
            disableTableButtons();
            currentTablePath = nullptr;
            setTableChanged(false);
            break;
        case QMessageBox::Save:
            saveTable();
            break;
        default:
            break;
    }
    return ret;
}

void MainWindow::saveTable() {
    if (!tableChanged())
        return;
    if (currentTablePath == nullptr) {
        QFileDialog objFlDlg(this);
        objFlDlg.setOption(QFileDialog::ShowDirsOnly, true);
        objFlDlg.setAcceptMode(QFileDialog::AcceptSave);
        objFlDlg.selectFile("myPhones.csv");
        QList<QLineEdit *> lst = objFlDlg.findChildren<QLineEdit *>();
        if (lst.count() == 1) {
            lst.at(0)->setReadOnly(true);
        } else {
        }
        if (objFlDlg.exec()) {
            currentTablePath = objFlDlg.selectedFiles().at(0);
        }
    }
    if (currentTablePath == nullptr)
        return;
    QString textData;
    auto *model = getModel();
    int rows = model->rowCount();
    int columns = model->columnCount();
    for (int i = 0; i < columns; i++) {
        textData += model->headerData(i, Qt::Horizontal).toString();
        if (i < columns - 1)
            textData += ",";
    }
    textData += "\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            textData += model->data(model->index(i, j)).toString();
            if (j < columns - 1)
                textData += ",";
        }
        textData += "\n";             // (optional: for new line segmentation)
    }
    QFile csvFile(currentTablePath);
    if (csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&csvFile);
        out << textData;
        csvFile.close();
    }
    cout << "saved successfully" << endl;
    setTableChanged(false);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    int ret = closeTable();
    switch (ret) {
        case QMessageBox::Discard:
        case QMessageBox::Save:
            event->accept();
            break;
        default:
            event->ignore();
            break;
    }
}

QStandardItemModel *MainWindow::getModel() {
    auto *model = (QStandardItemModel *) ((QSortFilterProxyModel *) ui->phoneNumbers->model())->sourceModel();
    return model;
}

void MainWindow::addRow() {
    cout << "adding row" << endl;
    auto &model = *getModel();
    QItemSelectionModel &select = *ui->phoneNumbers->selectionModel();
    auto id = new QStandardItem("0");
    auto name = new QStandardItem("Name");
    auto tel = new QStandardItem("+000000000000");
    QList<QStandardItem *> items;
    items << id << name << tel;

    if (select.hasSelection()) {
        int maxSelectedRow = -1;
        auto selectedRows = select.selectedRows();
        for (auto &row : selectedRows) {
            if (row.row() > maxSelectedRow)
                maxSelectedRow = row.row();
        }
        model.insertRow(maxSelectedRow + 1, items);
    } else {
        model.insertRow(model.rowCount(QModelIndex()), items);
    }
    setTableChanged(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setTableChanged(bool val = true) {
    tableHasChanged = val;
}


