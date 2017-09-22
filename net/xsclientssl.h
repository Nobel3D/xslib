#ifndef XSCLIENTSSL_H
#define XSCLIENTSSL_H

#include <QSslSocket>
#include "xslib_global.h"

class XSLIBSHARED_EXPORT xsClientSsl
{
public:
    xsClientSsl(const QString &crt);
    int Connect(const QString &address, int port, int timeout = 30000);
    QByteArray Read(int delay = 30000);
    int Write(const QByteArray &msg, int delay = 30000);
    int Disconnect();
private:
    QSslSocket* socket;
    QString strStatus;
};

#endif // XSCLIENTSSL_H
