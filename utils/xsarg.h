#ifndef XSARG_H
#define XSARG_H

#include "xslib_global.h"

class XSLIBSHARED_EXPORT xsArg : public QObject
{
    Q_OBJECT
public:
    explicit xsArg(QObject *parent = 0);
    xsArg(int argc, char** argv);
    int check(int value);
    QString operator [] (QString flag);
    QString operator [] (int index);

private:
    QStringList args;

};

#endif // XSARG_H
