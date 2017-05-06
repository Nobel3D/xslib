#include "xsclientssl.h"

xsClientSsl::xsClientSsl(const QString &crt)
{
    socket = new QSslSocket;
    socket->addCaCertificates(crt);
}

int xsClientSsl::Connect(const QString &address, int port, int timeout)
{
    socket->connectToHostEncrypted(address, port);

    if (socket->waitForEncrypted(timeout))    // Wait until encrypted connection is established, -1 means no timeout
    {
        qDebug() << "Connected";
        return OK;
    }
    else
    {
        strStatus = "ERROR: could not establish encrypted connection (" + socket->errorString() + ")";
        return FAIL;
    }
}

QByteArray xsClientSsl::Read(int delay)
{
    if (socket->waitForReadyRead(delay))    // Wait until some data is received
        return socket->readAll();    // Read message
    else
        strStatus = "Could not receive message (" + socket->errorString() + ")";
}
int xsClientSsl::Write(const QByteArray &msg, int delay)
{
    socket->write(msg);
    socket->waitForBytesWritten();
    return OK;
}

int xsClientSsl::Disconnect()
{
    socket->disconnectFromHost();    // Disconnect
    return OK;
}
