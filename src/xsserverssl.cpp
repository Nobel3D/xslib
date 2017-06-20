#include "xsserverssl.h"

#include <QFile>

xsServerSsl::xsServerSsl(const QString &filecrt, const QString &filekey, QObject *parent) : QTcpServer(parent)
{
    setLocalCertificate(filecrt);
    setPrivateKey(filekey);
    setProtocol(QSsl::TlsV1_2);
}

int xsServerSsl::Start(QHostAddress address, quint16 port)
{

    if (!listen(address, port))
    {
        strStatus = "ERROR: could not bind to " + address.toString() + ":" + port;
        return FAIL;
    }
    return OK;
}

int xsServerSsl::waitConnection()
{
    if (!waitForNewConnection(iTimeout))    // Wait until a new connection is received, -1 means no timeout
    {
        strStatus = "ERROR: could not establish encrypted connection (" + errorString() + ")";
        return FAIL;
    }
    qDebug() << "New connection";
    socket = dynamic_cast<QSslSocket*>(nextPendingConnection());
    return OK;
}

QByteArray xsServerSsl::Read(int delay)
{
    if (socket->waitForReadyRead(delay))    // Wait until some data is received
        return socket->readAll();    // Read message

    strStatus = "Could not receive message (" + socket->errorString() + ")";
    return QByteArray();
}
int xsServerSsl::Write(QByteArray msg, int delay)
{
    socket->write(msg);
    socket->waitForBytesWritten(delay);
    return OK;
}

int xsServerSsl::Disconnect()
{
    socket->disconnectFromHost();    // Disconnect
    return OK;
}

void xsServerSsl::incomingConnection(qintptr socketDescriptor)
{
    // Create socket
    QSslSocket *sslSocket = new QSslSocket(this);
    sslSocket->setSocketDescriptor(socketDescriptor);
    sslSocket->setLocalCertificate(m_sslLocalCertificate);
    sslSocket->setPrivateKey(m_sslPrivateKey);
    sslSocket->setProtocol(m_sslProtocol);
    sslSocket->startServerEncryption();

    // Add to the internal list of pending connections (see Qt doc: http://qt-project.org/doc/qt-5/qtcpserver.html#addPendingConnection)
    addPendingConnection(sslSocket);
}


const QSslCertificate &xsServerSsl::getLocalCertificate() const
{
    return m_sslLocalCertificate;
}

const QSslKey &xsServerSsl::getPrivateKey() const
{
    return m_sslPrivateKey;
}

QSsl::SslProtocol xsServerSsl::getProtocol() const
{
    return m_sslProtocol;
}

QString xsServerSsl::getStatus() const
{
    return strStatus;
}

void xsServerSsl::setLocalCertificate(const QSslCertificate &certificate)
{
    m_sslLocalCertificate = certificate;
}

int xsServerSsl::setLocalCertificate(const QString &path, QSsl::EncodingFormat format)
{
    QFile certificateFile(path);

    if (!certificateFile.open(QIODevice::ReadOnly))
        return FAIL;

    m_sslLocalCertificate = QSslCertificate(certificateFile.readAll(), format);
    return OK;
}


void xsServerSsl::setPrivateKey(const QSslKey &key)
{
    m_sslPrivateKey = key;
}

int xsServerSsl::setPrivateKey(const QString &fileName, QSsl::KeyAlgorithm algorithm, QSsl::EncodingFormat format, const QByteArray &passPhrase)
{
    QFile keyFile(fileName);

    if (!keyFile.open(QIODevice::ReadOnly))
        return FAIL;

    m_sslPrivateKey = QSslKey(keyFile.readAll(), algorithm, format, QSsl::PrivateKey, passPhrase);
    return OK;
}


void xsServerSsl::setProtocol(QSsl::SslProtocol protocol)
{
    m_sslProtocol = protocol;
}
