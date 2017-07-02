#include "xspassword.h"
#include <time.h>

xsPassword::xsPassword()
{
    setMaxHit(6);
}

xsPassword::xsPassword(QString &passwd, bool copyClear, QCryptographicHash::Algorithm type , int maxhit)
{
    setAlgorithm(type);
    setPassword(passwd, copyClear);
    if(setMaxHit(maxhit) == FAIL)
        setMaxHit(6);
}

int xsPassword::Check(const QString &hit)
{
    if(iHit > iMaxHit)
        return FAIL;

    if(HashKey(hit, iType) == strPassword)
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

    if(HashKey(Load(hit.fileName()), iType) == strPassword)
        return OK;
    else
    {
        iHit++;
        return FAIL;
    }
}

int xsPassword::Check(const xsPassword &hit)
{
    if(hit.strPassword == strPassword && iHit <= iMaxHit && hit.iType == iType)
        return OK;
    else
    {
        iHit++;
        return FAIL;
    }
}

int xsPassword::Save(const QString &pathfile) const
{
    QFile file(pathfile);
    file.open(QIODevice::WriteOnly);
    file.write(strPassword.toUtf8());
    file.close();
    return OK;
}

int xsPassword::Load(QFile &file, QCryptographicHash::Algorithm type, int maxhit)
{
    if(!file.exists())
        return FAIL;

    file.open(QFile::ReadOnly);
    strPassword = file.readAll();
    file.close();
    iType = type;
    iMaxHit = maxhit;
    return OK;
}

int xsPassword::Save(const QString &pathfile, QString &passwd, QCryptographicHash::Algorithm type)
{
    QFile file(pathfile);
    file.open(QIODevice::WriteOnly);
    file.write(HashKey(passwd, type).toUtf8());
    file.close();
    return OK;
}

QString xsPassword::Load(const QString &pathfile)
{
    QString offset;
    QFile file(pathfile);
    if(!file.exists())
        return offset;

    file.open(QFile::ReadOnly);
    offset = file.readAll();
    file.close();
    return offset;
}

int xsPassword::setPassword(QString &passwd, bool copyClear,  QCryptographicHash::Algorithm type , int maxhit)
{
    if(copyClear)
        strClear = passwd;

    strPassword = HashKey(passwd,type);
    iType = type;
    iMaxHit = maxhit;
    return OK;
}

void xsPassword::setAlgorithm(QCryptographicHash::Algorithm type)
{
    iType = type;
}

QString xsPassword::getPassword() const
{
    return strPassword;
}

QString xsPassword::getClearPassword() const
{
    return strClear;
}

int xsPassword::getHit() const
{
    return iHit;
}
int xsPassword::getMaxHit() const
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

QString xsPassword::HashKey(const QString &key, QCryptographicHash::Algorithm type)
{
    QCryptographicHash hasher(type);
    hasher.addData(key.toUtf8());
    return hasher.result().toHex();
}

QString xsPassword::generate(int length, bool symbols, bool spaces, bool unicode, bool numbers, bool lowers, bool uppers)
{
    QChar tmp;
    QString offset;
    bool pass = false;
    srand(time(0));

    while(offset.length() < length)
    {
        if(unicode)
            tmp = (unsigned short)rand() % MAX_UNICODE;
        else
            tmp = (char)rand() % MAX_ASCII;
        if(tmp.isUpper() && uppers)
            pass = true;
        else if(tmp.isLower() && lowers)
            pass = true;
        else if(tmp.isDigit() && numbers)
            pass = true;
        else if(tmp.isSpace() && spaces)
            pass = true;
        else if(tmp.isSymbol() && symbols)
            pass = true;
        else
            pass = false;

        if(pass)
            offset.append(tmp);

    }

    return offset;
}
