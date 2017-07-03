#ifndef XSDATABASE_H
#define XSDATABASE_H

#include "xslib_global.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlField>

class XSLIBSHARED_EXPORT xsDatabase
{
public:
    xsDatabase(const QString& file, const QString &connection_name);
    xsDatabase();

    ~xsDatabase();

    bool connect(const QString& file, const QString &connection_name);

    bool createTable(const QString &table, const QList<QSqlField> &fields);
    bool useTable(const QString &table);

    bool addValue(const QList<QVariant> &values);
    bool addValue(const QList<QSqlField> &fields, const QList<QVariant> &values);
    bool updateValue(const QSqlField &field, const QVariant &value, int id);
    bool removeValue(const QSqlField &field, const QVariant &value);
    bool updateValue(const QSqlField &field, const QVariant &oldvalue, const QVariant &newvalue);

    bool existValue(const QSqlField& field, const QVariant &value);

    QList<QVariant> getColumn(const QSqlField &field);
    QList<QVariant> getRow(int index);

    QVariant findValue(const QSqlField& field, int id);
    int findValue(const QSqlField& field, const QVariant& value);

    bool clearTable();

    QString format(const QList<QVariant> &list);
    QString format(const QStringList &list);
    QString format(const QList<QSqlField> &list, bool create = false);

    int getFieldCount();
    int getRecordCount();
    QStringList getTables();
    QList<QSqlField> getFields(bool hideID = false);

    bool existField(const QSqlField &field);
    bool existTable(const QString &table);

    QSqlField getField(int index);
    QSqlField getField(const QString &name);

    QString getTable();
    QString getMessage();
    QString getLastQuery();

    QString type(const QVariant &var);
    QString type(const QVariant::Type &t);
    QVariant type(const QString &str);

    bool Import(const QString &table, const QString &dir);
    bool Export(const QString &dir);
private:
    QString usingTable;
    QSqlDatabase* db;
    QSqlQuery* query;
    QSqlDriver* driver;
};


#endif // XSDATABASE_H
