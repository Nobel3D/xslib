#include "xsdatabase.h"

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
    if (table.isEmpty() && fields.isEmpty())
        return false;

    if (!query->exec("CREATE TABLE "+ table + "(" + fields + ");"))
    {
        status = -2;
        return false;
    }
    return true;
}
bool xsDatabase::useTable(const QString &table)
{
    if(table.isEmpty())
        return false;
    usingTable = table;
    return true;
}
bool xsDatabase::addValue(const QString& fields, const QString& values)
{
    if (fields.isEmpty() && values.isEmpty())
        return false;

    QStringList valuelist = values.split(",");
    QString formatvalue = "";

    for(int i = 0; i < valuelist.size(); i++)
        formatvalue += "?,";

    //Erase last character ','
    query->prepare("INSERT INTO " + usingTable + " (" + fields + ") VALUES (" +
                   formatvalue.left(formatvalue.length() - 1) + ")");

    for(int i = 0; i < valuelist.size(); i++)
    {
         query->bindValue(i, valuelist.value(i).trimmed());
    }

    if(!query->exec())
        return false;
    else
        return true;
}

bool xsDatabase::removeValue(const QString& field, const QString& value)
{
    if (field.isEmpty() && value.isEmpty())
        return false;

    if (existValue(field, value))
        return false;

    return query->exec("DELETE FROM " + usingTable + " WHERE " + field + " = (" + value + ")");
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

QString xsDatabase::findValue(const QString& field, int id)
{
    if (field.isEmpty() && id < 0)
        return "";

    query->exec("SELECT * FROM " + usingTable);
    for (int i = 0; query->next(); i++)
        if(i == id)
            return query->value(field).toString();

    return "";
}

int xsDatabase::findValue(const QString& field, const QString& value)
{
    if (field.isEmpty() && value.isEmpty())
        return FAIL;

    query->exec("SELECT * FROM " + usingTable);
    for (int i = 0; query->next(); i++)
        if(query->value(field).toString() == value)
            return i;

    return FAIL;
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
