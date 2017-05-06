#ifndef XSSERVERSSL_H
#define XSSERVERSSL_H

#include "xslib_global.h"
#include <QTcpServer>
#include <QSslSocket>
#include <QString>
#include <QSsl>
#include <QSslCertificate>
#include <QSslKey>

class xsServerSsl : public QTcpServer
{
    Q_OBJECT

public:
    xsServerSsl(const QString &filecrt, const QString &filekey, QObject *parent = 0);
    int Start(QHostAddress address, quint16 port);
    int waitConnection();

    QByteArray Read(int delay = 30000);
    int Write(QByteArray msg, int delay = 30000);
    int Disconnect();

    const QSslCertificate &getLocalCertificate() const;
    const QSslKey &getPrivateKey() const;
    QSsl::SslProtocol getProtocol() const;
    QString getStatus() const;


    void setLocalCertificate(const QSslCertificate &certificate);
    int setLocalCertificate(const QString &path, QSsl::EncodingFormat format = QSsl::Pem);

    void setPrivateKey(const QSslKey &key);
    int setPrivateKey(const QString &fileName, QSsl::KeyAlgorithm algorithm = QSsl::Rsa, QSsl::EncodingFormat format = QSsl::Pem, const QByteArray &passPhrase = QByteArray());

    void setProtocol(QSsl::SslProtocol protocol);


protected:
    void incomingConnection(qintptr socketDescriptor) override final;


private:
    QSslCertificate m_sslLocalCertificate;
    QSslKey m_sslPrivateKey;
    QSsl::SslProtocol m_sslProtocol;
    QSslSocket* socket;
    QString strStatus;
    int iTimeout = -1;
};

#endif // XSSERVERSSL_H
