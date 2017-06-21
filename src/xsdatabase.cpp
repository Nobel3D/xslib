#include "xsdatabase.h"

#include <QSqlDriver>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>

xsDatabase::xsDatabase(const QString &file)
{
    connect(file);
}
xsDatabase::xsDatabase()
{
}


xsDatabase::~xsDatabase()
{
    if (db->isOpen())
        db->close();
}

bool xsDatabase::connect(const QString& file)
{
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName(file);
    query = new QSqlQuery(*db);
    driver = db->driver();
    return db->open();
}

bool xsDatabase::createTable(const QString& table, const QString& fields)
{
    if (table.isEmpty() || fields.isEmpty())
        return false;

    return query->exec("CREATE TABLE "+ table + "( id INTEGER PRIMARY KEY," + fields + ");");
}
bool xsDatabase::useTable(const QString &table)
{
    if(table.isEmpty() || !existTable(table))
        return false;
    usingTable = table; //TODO: Add if table exists
    return true;
}

bool xsDatabase::existTable(const QString &table)
{
    return db->tables().contains(table);
}

bool xsDatabase::existField(const QString &field)
{
    return driver->record(usingTable).field(field).isValid();
}

bool xsDatabase::existField(int field)
{
    return driver->record(usingTable).field(field).isValid();
}


bool xsDatabase::addValue(const QList<QVariant> &values)
{
    X_PARAMS(values.isEmpty())

    QStringList fields = getFields();

    return query->exec("INSERT INTO " + usingTable + " (" + format(fields) + ") VALUES (" + format(values) + ")");
}

bool xsDatabase::updateValue(const QString &field, const QVariant &value, int id)
{
    X_PARAMS(field.isEmpty() || value.isNull() || id < 0);

    return query->exec("UPDATE " + usingTable + " SET " + field + " = '" + format(value) + "' WHERE ID = " + QString::number(id));
}

bool xsDatabase::removeValue(const QString& field, const QVariant& value)
{
    X_PARAMS (field.isEmpty() || value.isNull())

    if (existValue(field, value))
        return false;

    return query->exec("DELETE FROM " + usingTable + " WHERE " + field + " = (" +format(value) + ")");
}

bool xsDatabase::updateValue(const QString &field, const QVariant &oldvalue, const QVariant &newvalue)
{
    X_PARAMS(field.isEmpty() || oldvalue.isNull() || newvalue.isNull())

    if (!existValue(field, oldvalue))
        return false;

    return query->exec("UPDATE " + usingTable + " SET " + field + " = '" + format(newvalue) + "' WHERE " + field + " = '" + format(oldvalue) + "'");
}

QList<QVariant> xsDatabase::getColumn(int field)
{
    X_PARAMS(field < 0);
    QList<QVariant> offset;

    X_NOT_FOUND_FIELD(field, offset);

    query->exec("SELECT * FROM " + usingTable);
    while (query->next())
        offset.append(query->value(field));

    return offset;
}

QList<QVariant> xsDatabase::getColumn(const QString& field)
{
    X_PARAMS(field.isEmpty());
    QList<QVariant> offset;

    X_NOT_FOUND_FIELD(field, offset);

    query->exec("SELECT * FROM " + usingTable);
    while (query->next())
        offset.append(query->value(field));

    return offset;
}


QList<QVariant> xsDatabase::getRow(int index)
{
    X_PARAMS(index < 0);
    QList<QVariant> out;

    X_NOT_FOUND_FIELD(index, out);

    query->exec("SELECT * FROM " + usingTable);
    int fields = getFieldCount();
    int j = 0;

    while(j++ < index)
        query->next();

    for (int i = 0; i <= fields; i++)
            out.append(query->value(i));

    return out;
}

QVariant xsDatabase::findValue(int field, int id)
{
    X_PARAMS(field < 0 || id < 0);

    X_NOT_FOUND_FIELD(field, QVariant(QVariant::Invalid));

    query->exec("SELECT * FROM " + usingTable);
    for (int i = 0; query->next(); i++)
        if(i == id)
            return query->value(field);
}

QVariant xsDatabase::findValue(const QString& field, int id)
{
    X_PARAMS(field.isEmpty() || id < 0);

    X_NOT_FOUND_FIELD(field, QVariant(QVariant::Invalid));

    query->exec("SELECT * FROM " + usingTable);
    for (int i = 0; query->next(); i++)
        if(i == id)
            return query->value(field);

    return QVariant();
}

int xsDatabase::findValue(const QString& field, const QVariant &value)
{
    X_PARAMS(field.isEmpty() || value.isNull());

    X_NOT_FOUND_FIELD(field, -1);

    query->exec("SELECT * FROM " + usingTable);
    for (int i = 0; query->next(); i++)
        if(query->value(field) == value)
            return i;

    return -1;
}

bool xsDatabase::existValue(const QString& field, const QVariant &value)
{
    X_PARAMS(field.isEmpty() || value.isNull());
    return findValue(field, value) != -1;
}

bool xsDatabase::clearTable()
{
    if (!query->exec("DELETE FROM " + usingTable ))
        return false;

    return true;
}

QString xsDatabase::format(const QVariant &value)
{
    QString offset;

    if(value.type() != QVariant::String)
        offset.append(value.toString());
    else
        offset.append("\"" + value.toString() + "\"");
    return offset; //Erase last character ','
}

QString xsDatabase::format(const QList<QVariant> &list)
{
    int x = list.size();
    QString offset;

    for(int i = 0; i < x; i++)
        if(list.at(i).type() != QVariant::String)
            offset.append(list.at(i).toString() + ",");
        else
            offset.append("\"" + list.at(i).toString() + "\",");
    return offset.left(offset.length() - 1); //Erase last character ','
}

QString xsDatabase::format(const QStringList &list)
{
    int x = list.size();
    QString offset;

    for(int i = 0; i < x; i++)
            offset.append("\"" + list.at(i) + "\",");
    return offset.left(offset.length() - 1); //Erase last character ','
}


QStringList xsDatabase::getTables()
{
    return db->tables();
}

QStringList xsDatabase::getFields(bool id)
{
    QStringList offset;
    int i = 1;
    if(id)
        i = 0;

    for(; i < getFieldCount(); i++)
        offset.append(driver->record(usingTable).fieldName(i));

    return offset;
}


QSqlField xsDatabase::getField(int index)
{
    return driver->record(usingTable).field(index);
}

QSqlField xsDatabase::getField(const QString &name)
{
    return driver->record(usingTable).field(name);
}

int xsDatabase::getFieldCount()
{
    return driver->record(usingTable).count(); //start with value '0'
}

QString xsDatabase::getTable()
{
    return usingTable;
}
QString xsDatabase::getMessage()
{
    return query->lastError().text();
}
QString xsDatabase::getLastQuery()
{
    return query->lastQuery();
}
