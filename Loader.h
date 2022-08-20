//
// Created by AS on 8/19/22.
//

#ifndef PHONEBOOK_LOADER_H
#define PHONEBOOK_LOADER_H


#include <QString>
#include <QFile>

class Loader {
public:
    static QString readFileAsQString(const QString& path);
};


#endif //PHONEBOOK_LOADER_H
