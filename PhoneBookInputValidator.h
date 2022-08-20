//
// Created by AS on 8/20/22.
//

#ifndef PHONEBOOK_PHONEBOOKINPUTVALIDATOR_H
#define PHONEBOOK_PHONEBOOKINPUTVALIDATOR_H

#include <QItemDelegate>
#include <QRegExpValidator>

class PhoneBookInputValidator : public QItemDelegate {
Q_OBJECT
public:
    explicit PhoneBookInputValidator(QObject *parent = nullptr);
    static const QRegExpValidator *idValidator;
    static const QRegExpValidator *phoneValidator;

protected:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void
    updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:

public slots:

};


#endif //PHONEBOOK_PHONEBOOKINPUTVALIDATOR_H
