#ifndef XSPASSWORD_H
#define XSPASSWORD_H

#include "xslib_global.h"
#include <QCryptographicHash>
#include <QFile>

class xsPassword
{
public:
    xsPassword(QString &passwd, int maxhit = 6);
    xsPassword(QFile &passwd, int maxhit = 6);
    int Check(const QString &hit);
    int Check(QFile &hit);
    int Save(const QString &pathfile);
    static int Save(const QString &pathfile, QString &passwd);
    int setPassword(QString &passwd);
    QString getPassword();
    int getHit();
    int getMaxHit();
    int setMaxHit(int n);
    static QString HashKey(const QString &key);
private:
    QString strPassword;
    int iMaxHit;
    int iHit;
};

#endif // XSPASSWORD_H
