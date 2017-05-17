#ifndef XSCLIENT_H
#define XSCLIENT_H

#include "xslib_global.h"
#include <QTcpSocket>

class xsClient
{
private:
    QString strAddress;
    int iPort;
    QTcpSocket* socket;
public:
    xsClient();
    int Connect(const QString &_address, int _port, int _timeout = 30000);
    int Login(const QString &passwd);

    QString Read(int _timeout = 30000);
    int Write(const QString &send, int _timeout = 30000);
    void Close();

    QString getAddress();
    QString getPort();

    bool isOpen();
};

#endif // XSCLIENT_H
