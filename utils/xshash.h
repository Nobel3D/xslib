#ifndef XSHASH_H
#define XSHASH_H

#include <QCryptographicHash>
#include "xslib_global.h"

class xsHash
{
public:
    xsHash();
    xsHash(QCryptographicHash::Algorithm type);
    QString hash(const QString& key);
    static QString hash(const QString& key, QCryptographicHash::Algorithm type);

private:
    QCryptographicHash::Algorithm t;
};

#endif // XSHASH_H
