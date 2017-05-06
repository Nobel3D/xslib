#include "xspassword.h"

xsPassword::xsPassword(QString &passwd, int maxhit)
{
    setPassword(passwd);
    if(setMaxHit(maxhit) == FAIL)
        setMaxHit(6);
}

xsPassword::xsPassword(QFile &passwd, int maxhit)
{
    passwd.open(QFile::ReadOnly);
    strPassword = passwd.readAll();
    passwd.close();
    if(setMaxHit(maxhit) == FAIL)
        setMaxHit(6);
}

int xsPassword::Check(const QString &hit)
{
    if(iHit > iMaxHit)
        return FAIL;

    if(HashKey(hit) == strPassword)
        return OK;
    else
    {
        iHit++;
        return FAIL;
    }
}

int xsPassword::Check(QFile &hit)
{
    if(iHit > iMaxHit)
        return FAIL;

    hit.open(QFile::ReadOnly);
    QString tmp = hit.readAll();
    hit.close();
    if(HashKey(tmp) == strPassword)
        return OK;
    else
    {
        iHit++;
        return FAIL;
    }
}

int xsPassword::Save(const QString &pathfile)
{
    QFile file(pathfile);
    file.open(QIODevice::WriteOnly);
    file.write(strPassword.toUtf8());
    file.close();
    return OK;
}

int xsPassword::Save(const QString &pathfile, QString &passwd)
{
    QFile file(pathfile);
    file.open(QIODevice::WriteOnly);
    file.write(HashKey(passwd).toUtf8());
    file.close();
    passwd.clear();
    return OK;
}

int xsPassword::setPassword(QString &passwd)
{
    strPassword = HashKey(passwd);
    passwd.clear();
    return OK;
}

QString xsPassword::getPassword()
{
    return strPassword;
}
int xsPassword::getHit()
{
    return iHit;
}
int xsPassword::getMaxHit()
{
    return iMaxHit;
}

int xsPassword::setMaxHit(int n)
{
    iHit = 0;
    if(n < 1)
        return FAIL;

    iMaxHit = n;
    return OK;
}

QString xsPassword::HashKey(const QString &key)
{
    QCryptographicHash hasher(QCryptographicHash::Sha512);
    hasher.addData(key.toLatin1());
    return hasher.result();
}
