#ifndef XSDATABASE_H
#define XSDATABASE_H

#include "xslib_global.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlField>

class XSLIBSHARED_EXPORT xsDatabase
{
public:
    xsDatabase(const QString& file);
    xsDatabase();

    ~xsDatabase();

    bool connect(const QString& file);

    bool createTable(const QString &table, const QString &fields);
    bool useTable(const QString &table);

    bool addValue(const QList<QVariant> &values);
    bool updateValue(const QString &field, const QVariant &value, int id); //TODO: manage recursive values
    bool removeValue(const QString& field, const QVariant& value);
    bool updateValue(const QString &field, const QVariant &oldvalue, const QVariant &newvalue);

    bool existValue(const QString& field, const QVariant &value);

    QList<QVariant> getColumn(const QString& field);
    QList<QVariant> getColumn(int index);
    QList<QVariant> getRow(int index);

    QVariant findValue(int field, int id);
    QVariant findValue(const QString& field, int id);
    int findValue(const QString& field, const QVariant& value);

    bool clearTable();

    QString format(const QVariant &value);
    QString format(const QList<QVariant> &list);
    QString format(const QStringList &list);

    int getFieldCount();
    QStringList getTables();
    QStringList getFields(bool id = false);

    bool existField(const QString &field);
    bool existField(int field);
    bool existTable(const QString &table);

    QSqlField getField(int index);
    QSqlField getField(const QString &name);

    QString getTable();
    QString getMessage();
    QString getLastQuery();
private:
    QString usingTable;
    QSqlDatabase* db;
    QSqlQuery* query;
    QSqlDriver* driver;
};


#endif // XSDATABASE_H
