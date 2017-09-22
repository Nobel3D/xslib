 #include "xshash.h"

xsHash::xsHash()
{

}

xsHash::xsHash(QCryptographicHash::Algorithm type)
{
    t = type;
}

QString xsHash::hash(const QString &key)
{
    QCryptographicHash hasher(t);
    hasher.addData(key.toUtf8());
    return hasher.result().toHex();
}

QString xsHash::hash(const QString &key, QCryptographicHash::Algorithm type)
{
    QCryptographicHash hasher(type);
    hasher.addData(key.toUtf8());
    return hasher.result().toHex();
}
