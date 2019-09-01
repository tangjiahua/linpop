#ifndef MULISTITEMDATA_H
#define MULISTITEMDATA_H

#include <QMetaType>

typedef struct {
    QString iconPath;
    QString name;
    QString status;
} MyMuItemData;

Q_DECLARE_METATYPE(MyMuItemData)

#endif // MULISTITEMDATA_H
