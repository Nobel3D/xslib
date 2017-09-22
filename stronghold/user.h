#ifndef USER_H
#define USER_H

#include "xslib_global.h"
#include <utils/xsblowfish.h>

class XSLIBSHARED_EXPORT User
{
public:
    User(int _id, const QString &_name, const QString &password, const QString &_database, int _level);

    QString decrypt(const QVariant& encoded);
    QString encrypt(const QVariant& encoded);

    int getID();
    QString getName();
    QString getDatabase();
    int getLevel();
private:
    int id;
    QString name;
    xsBlowfish* blowfish;
    QString database;
    int level;
};

#endif // USER_H
