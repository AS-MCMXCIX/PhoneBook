#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <QFileDialog>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define print(string) cout << string <<endl

using namespace std;

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->phoneNumbers->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->phoneNumbers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveTable);
    connect(ui->addRowButton, &QPushButton::clicked, this, &MainWindow::addRow);
    connect(ui->loadDataButton, &QPushButton::clicked, this, &MainWindow::loadTable);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::closeTable);
    connect(ui->removeSelectionButton, &QPushButton::clicked, this, &MainWindow::removeSelection);
    connect(ui->newTableButton, &QPushButton::clicked, this, &MainWindow::newTable);
    setNoTableButtonsDisabled();
}

void MainWindow::setNoTableButtonsDisabled(bool val) {
    ui->saveButton->setDisabled(val);
    ui->addRowButton->setDisabled(val);
    ui->removeSelectionButton->setDisabled(val);
    ui->closeButton->setDisabled(val);
}

void MainWindow::loadTable() {
    QString path = QFileDialog::getOpenFileName(this,
                                                QFileDialog::tr("Open CSV"), "./",
                                                QFileDialog::tr("CSV File (*.csv)"));
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
    ui->phoneNumbers->setModel(&model);
    setNoTableButtonsDisabled(false);
}

void MainWindow::removeSelection() {
    QItemSelectionModel &select = *ui->phoneNumbers->selectionModel();
    if (select.hasSelection()) {
        auto selectedRows = select.selectedRows();
        for (auto &row : selectedRows) {
            ui->phoneNumbers->model()->removeRow(row.row());
        }
    }
}

void MainWindow::closeTable() {
    ui->phoneNumbers->setModel(nullptr);
    setNoTableButtonsDisabled();
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
    ui->phoneNumbers->setModel(&model);
    setNoTableButtonsDisabled(false);
}

void MainWindow::saveTable() {
    QString textData;
    auto *model = ui->phoneNumbers->model();
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
    QFile csvFile("./phones.csv");
    if (csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&csvFile);
        out << textData;
        csvFile.close();
    }
    cout << "saved successfully" << endl;
}

void MainWindow::addRow() {
    cout << "adding row" << endl;
    auto &model = *ui->phoneNumbers->model();
    QItemSelectionModel &select = *ui->phoneNumbers->selectionModel();
    if (select.hasSelection()) {
        int maxSelectedRow = -1;
        auto selectedRows = select.selectedRows();
        for (auto &row : selectedRows) {
            if (row.row() > maxSelectedRow)
                maxSelectedRow = row.row();
        }
        model.insertRow(maxSelectedRow + 1);
    } else
        model.insertRow(model.rowCount(QModelIndex()));
}

MainWindow::~MainWindow() {
    delete ui;
}


