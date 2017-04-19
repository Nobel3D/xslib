#ifndef XSLOG_H
#define XSLOG_H

#include <QFile>

#include "xslib_global.h"

class xsLog : public QObject
{
    Q_OBJECT
public:
    explicit xsLog(QObject *parent = 0);
    xsLog(QString filename);
    void addLog(QString message);
    QString getLog();
    static void addLog(QString filename, QString message);

private:
    QFile* fileLog;
    QString lastLog;
    static QString format(QString message);
};

#endif // XSLOG_H
