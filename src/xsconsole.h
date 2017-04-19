#ifndef XSCONSOLE_H
#define XSCONSOLE_H

#include <QTextStream>
#include "xslib_global.h"

class xsConsole : public QObject
{
    Q_OBJECT
public:
    explicit xsConsole(QObject *parent = 0);

    void Write(QString text);
    void Read(QString& text);
    void setXY(int x, int y);
    void getXY(int& x, int& y);
    xsConsole& operator <<(QString text);
    xsConsole& operator >>(QString& text);

private:
    QTextStream* in;
    QTextStream* out;
};

#endif // XSCONSOLE_H
