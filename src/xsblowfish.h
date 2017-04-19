#ifndef XSBLOWFISH_H
#define XSBLOWFISH_H


#include <qmath.h>
#include "xslib_global.h"
#include "hexpi.h"

class xsBlowfish : public QObject
{
    Q_OBJECT
public:

    xsBlowfish(QString key);

    void initBoxes();
    void calcSubKey(QString keyStr);
    QByteArray encrypt(QByteArray data);
    QByteArray decrypt(QByteArray cryptedData);


private:
    void _decrypt(quint32 &L, quint32 &R);
    void _encyrpt(quint32 &L, quint32 &R);
    quint32 f(quint32 x);
    quint32 get32Batch(QByteArray data, uint startVal);
    quint32 get32FromChars(QByteArray data, uint startVal);
    QByteArray convertToChar(quint32 L, quint32 R);

    HexPi hexPi;

    quint32 P[18];
    quint32 S[4][256];
};

#endif // XSBLOWFISH_H
