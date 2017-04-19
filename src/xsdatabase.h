#ifndef XSDATABASE_H
#define XSDATABASE_H

#include "xslib_global.h"
#include <QSqlDatabase>
#include <QSqlQuery>

class xsDatabase
{
public:
    xsDatabase(const QString& file);
    xsDatabase();

    ~xsDatabase();

    bool connect(const QString& file);
    int getStatus() const;

    bool createTable(const QString &table, const QString &fields);
    bool useTable(const QString &table);

    bool addValue(const QString& fields, const QString& values);
    bool removeValue(const QString& field, const QString& value);

    bool existValue(const QString& field, const QString &value);
    QStringList printColumn(const QString& field);
    QStringList printColumn(int field);
    QString findValue(const QString& field, int id);
    int findValue(const QString& field, const QString& value);

    bool clearTable();

    QString getTable();
    QString getMessage();
    QString getLastQuery();
private:
    QString usingTable;
    QSqlDatabase db;
    QSqlQuery* query;
    int status;
};


#endif // XSDATABASE_H
