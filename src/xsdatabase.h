#ifndef XSDATABASE_H
#define XSDATABASE_H

#include "xslib_global.h"
#include <QSqlDatabase>
#include <QSqlQuery>

class XSLIBSHARED_EXPORT xsDatabase //TODO: ADD VALUES AND FIELDS CLASS!
{
public:
    xsDatabase(const QString& file);
    xsDatabase();

    ~xsDatabase();

    bool connect(const QString& file);
    int getStatus() const;

    bool createTable(const QString &table, const QString &fields);
    bool useTable(const QString &table);
    bool existTable(const QString &table);

    bool addValue(const QStringList &values);
    bool updateValue(const QString &field, const QString &value, int id);
    bool removeValue(const QString& field, const QString& value);
    bool updateValue(const QString &field, const QString &oldvalue, const QString &newvalue);

    bool existValue(const QString& field, const QString &value);
    QStringList printColumn(const QString& field);
    QStringList printColumn(int field);
    QString findValue(int field, int id);
    QString findValue(const QString& field, int id);
    int findValue(const QString& field, const QString& value);

    bool clearTable();

    QString format(const QStringList &list, bool text = false);

    int getFieldCount();
    QStringList getTables();
    QStringList getFieldsList();
    QString getFieldName(int x);
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
