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
    if (db.isOpen())
        db.close();
}

bool xsDatabase::connect(const QString& file)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(file);
    status = db.open() ? OK : FAIL;
    query = new QSqlQuery(db);
    return status == OK;
}

int xsDatabase::getStatus() const
{
    return status;
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
    return db.tables().contains(table);
}

bool xsDatabase::addValue(const QStringList& values)
{
    if (values.isEmpty())
        return false;

    QStringList fields = getFieldsList();

    return query->exec("INSERT INTO " + usingTable + " (" + format(fields) + ") VALUES (" + format(values) + ")");
}

bool xsDatabase::updateValue(const QString &field, const QString &value, int id)
{
    if(field.isEmpty() || value.isEmpty() || id < 0)
        return false;

    return query->exec("UPDATE " + usingTable + " SET " + field + " = '" + value + "' WHERE ID = " + QString::number(id));
}

bool xsDatabase::removeValue(const QString& field, const QString& value)
{
    if (field.isEmpty() || value.isEmpty())
        return false;

    if (existValue(field, value))
        return false;

    return query->exec("DELETE FROM " + usingTable + " WHERE " + field + " = (" + value + ")");
}

bool xsDatabase::updateValue(const QString &field, const QString &oldvalue, const QString &newvalue)
{
    if (field.isEmpty() || oldvalue.isEmpty() || newvalue.isEmpty())
        return false;

    if (!existValue(field, oldvalue))
        return false;

    return query->exec("UPDATE " + usingTable + " SET " + field + " = '" + newvalue + "' WHERE " + field + " = '" + oldvalue + "'");
}

QStringList xsDatabase::printColumn(const QString& field)
{
    QStringList offset;
    query->exec("SELECT * FROM " + usingTable);
    int id = query->record().indexOf(field);
    while (query->next())
        offset.append(query->value(id).toString());

    return offset;
}

QStringList xsDatabase::printColumn(int field)
{
    QStringList offset;
    query->exec("SELECT * FROM " + usingTable);
    while (query->next())
        offset.append(query->value(field).toString());

    return offset;
}

QString xsDatabase::findValue(int field, int id)
{
    if (field < 0 || id < 0)
        return "";

    query->exec("SELECT * FROM " + usingTable);
    for (int i = 0; query->next(); i++)
        if(i == id)
            return query->value(field).toString();

    return "";
}

QString xsDatabase::findValue(const QString& field, int id)
{
    if (field.isEmpty() || id < 0)
        return "";

    query->exec("SELECT * FROM " + usingTable);
    for (int i = 0; query->next(); i++)
        if(i == id)
            return query->value(field).toString();

    return "";
}

int xsDatabase::findValue(const QString& field, const QString& value)
{
    if (field.isEmpty() || value.isEmpty())
        return FAIL;

    query->exec("SELECT * FROM " + usingTable);
    for (int i = 0; query->next(); i++)
        if(query->value(field).toString() == value)
            return i;

    return -1;
}

bool xsDatabase::existValue(const QString& field, const QString& value)
{
    return findValue(field, value) != -1;
}

bool xsDatabase::clearTable()
{
    if (!query->exec("DELETE FROM " + usingTable + ")"))
        return false;

    return true;
}

QString xsDatabase::format(const QStringList &list, bool text)
{
    int x = list.size();
    QString offset = "";

    for(int i = 0; i < x; i++)
        if(text)
            offset.append(list.value(i) + ",");      //FIELDS != FIELD
        else
            offset.append("\"" + list.value(i) + "\",");
    return offset.left(offset.length() - 1); //Erase last character ','
}

int xsDatabase::getFieldCount()
{
    const QSqlDriver* driver = query->driver();
    return driver->record(usingTable).count();
}

QStringList xsDatabase::getTables()
{
    return db.tables();
}

QStringList xsDatabase::getFieldsList()
{
    QStringList offset;

    const QSqlDriver* driver = query->driver();
    for(int i = 1; i < getFieldCount(); i++)
        offset.append(driver->record(usingTable).fieldName(i));

    return offset;
}

QString xsDatabase::getFieldName(int x)
{
    const QSqlDriver* driver = query->driver();
    return driver->record(usingTable).fieldName(x);
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
