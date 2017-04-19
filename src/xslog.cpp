#include "xslog.h"
#include <QDateTime>

xsLog::xsLog(QString filename)
{
    fileLog = new QFile(filename);
}

QString xsLog::format(QString message)
{
    return QDateTime::currentDateTime().toString() + " -> " + message + "\n";
}

void xsLog::addLog(QString message)
{
    fileLog->open(QFile::Append);
    fileLog->write(format(message).toUtf8());
    lastLog = message;
    fileLog->close();
}

QString xsLog::getLog()
{
    return lastLog;
}

void xsLog::addLog(QString filename, QString message)
{
    QFile* log = new QFile(filename);
    log->open(QFile::Append);
    log->write(format(message).toUtf8());
    log->close();
}
