//
// Created by AS on 8/20/22.
//

#include "PhoneBookInputValidator.h"
#include "mainwindow.h"

#include <QLineEdit>
#include <QRegExpValidator>
#include <QIntValidator>
#include <iostream>

using namespace std;


PhoneBookInputValidator::PhoneBookInputValidator(QObject *parent) :
        QItemDelegate(parent) {
}

const QRegExpValidator *PhoneBookInputValidator::idValidator = new QRegExpValidator(QRegExp(R"(\d+)"));
const QRegExpValidator *PhoneBookInputValidator::phoneValidator = new QRegExpValidator(
        QRegExp(R"(\s*(?:\+?(\d{1,3}))?[-. (]*(\d{3})[-. )]*(\d{3})[-. ]*(\d{4})(?: *x(\d+))?\s*)"));

QWidget *PhoneBookInputValidator::createEditor(QWidget *parent,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const {

    auto *editor = new QLineEdit(parent);
    switch (index.column()) {
        case 0:
            editor->setValidator(idValidator);
            break;
        case 1:
            break;
        case 2:
            editor->setValidator(phoneValidator);
            break;
    }
    cout << "(" << index.row() << ", " << index.column() << ")" << endl;
    return editor;
}


void PhoneBookInputValidator::setEditorData(QWidget *editor,
                                            const QModelIndex &index) const {
    QString value = index.model()->data(index, Qt::EditRole).toString();
    auto *line = dynamic_cast<QLineEdit *>(editor);
    line->setText(value);
}


void PhoneBookInputValidator::setModelData(QWidget *editor,
                                           QAbstractItemModel *model,
                                           const QModelIndex &index) const {
    auto *line = dynamic_cast<QLineEdit *>(editor);
    QString value = line->text();
    if (model->data(index) != value)
        ((MainWindow *) model->parent())->setTableChanged(true);
    model->setData(index, value);
}


void PhoneBookInputValidator::updateEditorGeometry(QWidget *editor,
                                                   const QStyleOptionViewItem &option,
                                                   const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}
