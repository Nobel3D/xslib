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

bool xsDatabase::createTable(const QString& table, const QList<QSqlField> &fields)
{
    X_PARAMS(table.isEmpty() || fields.isEmpty());

    return query->exec("CREATE TABLE "+ table + "(" + format(fields) + ")");
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

bool xsDatabase::existField(const QSqlField &field)
{
    return driver->record(usingTable).field(field.name()).isValid();
}


bool xsDatabase::addValue(const QList<QVariant> &values)
{
    X_PARAMS(values.isEmpty())

    return query->exec("INSERT INTO " + usingTable + " (" + format(getFields()) + ") VALUES (" + format(values) + ")");
}

bool xsDatabase::updateValue(const QSqlField &field, const QVariant &value, int id)
{
    X_PARAMS(value.isNull() || id < 0);
    X_FIELD(field, value, false);

    return query->exec("UPDATE " + usingTable + " SET " + field.name() + " = '" + value.toString() + "' WHERE ID = " + QString::number(id));
}

bool xsDatabase::removeValue(const QSqlField &field, const QVariant& value)
{
    X_PARAMS (value.isNull());
    X_FIELD(field, value, false);
    return query->exec("DELETE FROM " + usingTable + " WHERE " + field.name() + " = (" + value.toString() + ")");
}

bool xsDatabase::updateValue(const QSqlField &field, const QVariant &oldvalue, const QVariant &newvalue)
{
    X_PARAMS(oldvalue.isNull() || newvalue.isNull())
    X_FIELD(field, oldvalue, false);
    X_FIELD(field, newvalue, false);
    return query->exec("UPDATE " + usingTable + " SET " + field.name() + " = '" + newvalue.toString() + "' WHERE " + field.name() + " = '" + oldvalue.toString() + "'");
}


QList<QVariant> xsDatabase::getColumn(const QSqlField& field)
{
    QList<QVariant> offset;

    X_NOT_FOUND_FIELD(field, offset);

    query->exec("SELECT * FROM " + usingTable);
    while (query->next())
        offset.append(query->value(field.name()));

    return offset;
}


QList<QVariant> xsDatabase::getRow(int index)
{
    X_PARAMS(index < 0);
    QList<QVariant> out;

    query->exec("SELECT * FROM " + usingTable);
    int fields = getFieldCount();
    int j = 0;

    while(j++ < index)
        query->next();

    for (int i = 0; i < fields; i++)
            out.append(query->value(i));

    return out;
}

QVariant xsDatabase::findValue(const QSqlField& field, int id)
{
    X_PARAMS(id < 0);

    X_NOT_FOUND_FIELD(field, QVariant(QVariant::Invalid));

    query->exec("SELECT * FROM " + usingTable);
    for (int i = 0; query->next(); i++)
        if(i == id)
            return query->value(field.name());

    return QVariant();
}

int xsDatabase::findValue(const QSqlField& field, const QVariant &value)
{
    X_PARAMS(value.isNull());
    X_FIELD(field, value, -1);

    query->exec("SELECT * FROM " + usingTable);
    for (int i = 0; query->next(); i++)
        if(query->value(field.name()) == value)
            return i;

    return -1;
}

bool xsDatabase::existValue(const QSqlField &field, const QVariant &value)
{
    X_PARAMS(value.isNull());
    X_FIELD(field, value, false);
    return findValue(field, value) != -1;
}

bool xsDatabase::clearTable()
{
    if (!query->exec("DELETE FROM " + usingTable ))
        return false;

    return true;
}

QString xsDatabase::format(const QList<QVariant> &list)
{
    QString offset;

    for(int i = 0; i < list.size(); i++)
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
            offset.append(list.at(i) + ",");
    return offset.left(offset.length() - 1); //Erase last character ','
}

QString xsDatabase::format(const QList<QSqlField> &list)
{
    QString format;

    for(int i = 0; i < list.count(); i++)
        format.append(list.at(i).name() + " " + type(list.at(i).type()) + ",");
    return format.left(format.length() - 1);
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

int xsDatabase::getRecordCount()
{
    int i=0;
    query->exec("SELECT * FROM " + usingTable);
    query->last();
    return query->at();
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

QString xsDatabase::type(const QVariant &var)
{
    switch(var.type())
    {
    case QVariant::LongLong:
        return "INTEGER PRIMARY KEY";
    case QVariant::String:
        return "TEXT";
    case QVariant::Invalid:
        return "NULL";
    case QVariant::Int:
        return "INTEGER";
    case QVariant::Double:
        return "REAL";
    case QVariant::ByteArray:
        return "BLOB";
    default:
        X_INVALID_TYPE(var);
    }
}

QVariant xsDatabase::type(const QString &str)
{
    if(str == "INTEGER PRIMARY KEY")
        return QVariant(QVariant::LongLong);
    else if(str == "TEXT")
        return QVariant(QVariant::String);
    else if(str == "NULL")
        return QVariant(QVariant::Invalid);
    else if(str == "INTEGER")
        return QVariant(QVariant::Int);
    else if(str == "REAL")
        return QVariant(QVariant::Double);
    else if(str == "BLOB")
        return QVariant(QVariant::ByteArray);
    else
        qWarning() << __func__ << "() Invalid type name: " << str;
}
