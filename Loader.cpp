//
// Created by AS on 8/19/22.
//

#include "Loader.h"

QString Loader::readFileAsQString(const QString &path) {
    QFile styleFile(path);
    if (styleFile.exists()) {
        styleFile.open(QFile::ReadOnly);
        QString style(styleFile.readAll());
        return style;
    }
    return "";
}
