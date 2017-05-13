#ifndef XSPASSWORD_H
#define XSPASSWORD_H

#include "xslib_global.h"
#include <QCryptographicHash>
#include <QFile>

class XSLIBSHARED_EXPORT xsPassword
{
public:
    xsPassword();
    xsPassword(QString &passwd, bool copyClear = false, QCryptographicHash::Algorithm type = QCryptographicHash::Sha512, int maxhit = 6);
    int Check(const QString &hit);
    int Check(QFile &hit);
    int Check(const xsPassword &hit);
    int Save(const QString &pathfile) const;
    int Load(QFile &file, QCryptographicHash::Algorithm type = QCryptographicHash::Sha512, int maxhit = 6);
    static int Save(const QString &pathfile, QString &passwd, QCryptographicHash::Algorithm type = QCryptographicHash::Sha512);
    static QString Load(const QString &pathfile);
    int setPassword(QString &passwd, bool copyClear = false);
    int setAlgorithm(QCryptographicHash::Algorithm type);
    QString getPassword() const;
    QString getClearPassword() const;
    int getHit();
    int getMaxHit();
    int setMaxHit(int n);
    static QString HashKey(const QString &key, QCryptographicHash::Algorithm type);
    static QString pwGenerate(int length);
private:
    QCryptographicHash::Algorithm iType;
    QString strPassword;
    QString strClear;
    int iMaxHit;
    int iHit;
};

#endif // XSPASSWORD_H
