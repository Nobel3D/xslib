#include "user.h"

User::User(int _id, const QString &_name, const QString &password, const QString &_database, int _level)
{
    id = _id;
    name = _name;
    blowfish = new xsBlowfish(password);
    database = _database;
    level = _level;
}

QString User::decrypt(const QVariant& encoded)
{
    if (encoded.type() == QVariant::String)
        return QString(blowfish->decrypt(QByteArray::fromHex(encoded.toString().toUtf8())));
    else
        return encoded.toString();
}

QString User::encrypt(const QVariant& decoded)
{
    return QString(blowfish->encrypt(decoded.toString().toUtf8()).toHex());
}

int User::getID() { return id; }
QString User::getName() { return name; }
QString User::getDatabase() { return database; }
int User::getLevel() { return level; }
