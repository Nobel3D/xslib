#include "xsclient.h"
#include <QtNetwork>

xsClient::xsClient()
{
    socket = new QTcpSocket;
}

int xsClient::Connect(const QString &_address, int _port, int _timeout)
{
    strAddress = _address;
    iPort = _port;

    if (!socket->isOpen())  //Disconnected
    {
        socket->connectToHost(strAddress, iPort);

        if (!socket->waitForConnected(_timeout))
            return FAIL; //strStatus = "Impossible reach host!";
        return OK; //Connected
    }

    //strStatus = "Already connected!";
    return FAIL;
}
QString xsClient::Read(int _timeout)
{
    char buffer;
    QString strout;
    socket->waitForReadyRead(_timeout);
    while(socket->bytesAvailable() != 0 && buffer != '\r')
    {
        socket->read(&buffer, 1);
        strout.append(buffer);
    }
    return strout;
}

int xsClient::Write(const QString &send, int _timeout)
{
    socket->write(send.toUtf8());
    socket->waitForBytesWritten(_timeout);
    return OK;
}

void xsClient::Close()
{
    socket->close();
}

bool xsClient::isOpen()
{
    return socket->isOpen();
}

QString xsClient::getAddress() { return strAddress; }
QString xsClient::getPort() { return QString::number(iPort); }
