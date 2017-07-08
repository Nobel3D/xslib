#include "xsdatabase.h"

#include <QSqlDriver>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QFile>
#include <QDebug>

xsDatabase::xsDatabase(const QFileInfo &file, const QString &connection_name)
{
    if( !connect(file.absoluteFilePath(), connection_name) )
        qWarning() << "Impossible to open database in " << file.absoluteFilePath();

    XSDBG_DB
}
xsDatabase::xsDatabase()
{
}


xsDatabase::~xsDatabase()
{
    if (db->isOpen())
        db->close();
}

bool xsDatabase::connect(const QString& file, const QString &connection_name)
{
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connection_name));
    db->setDatabaseName(file);
    query = new QSqlQuery(*db);
    driver = db->driver();
    return db->open();
}

bool xsDatabase::createTable(const QString& table, const QList<QSqlField> &fields)
{
    X_PARAMS(table.isEmpty() || fields.isEmpty());

    call("CREATE TABLE "+ table + "(" + format(fields, true) + ")");
    return true;
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

bool xsDatabase::addValue(const QList<QVariant> &values) //TODO: TAKE ID?
{
    X_PARAMS(values.isEmpty())

    call("INSERT INTO " + usingTable + " (" + format(getFields(true)) + ") VALUES (" + format(values) + ")");
    return true;
}

bool xsDatabase::addValue(const QList<QSqlField> &fields, const QList<QVariant> &values)
{
    X_PARAMS(values.isEmpty())

    call("INSERT INTO " + usingTable + " (" + format(fields) + ") VALUES (" + format(values) + ")");
    return true;
}

bool xsDatabase::addValue()
{
    call("INSERT INTO " + usingTable + " (" + getField(1).name() + ") VALUES (NULL)");
    return true; //TODO: MANAGE NOT NULL FIELD!
}

bool xsDatabase::updateValue(const QSqlField &field, const QVariant &value, int id)
{
    X_PARAMS(value.isNull() || id < 0);
    X_FIELD(field, value, false);

    call("UPDATE " + usingTable + " SET " + field.name() + " = '" + value.toString() + "' WHERE ID = " + QString::number(id));
    return true;
}

bool xsDatabase::removeValue(const QSqlField &field, const QVariant& value)
{
    X_PARAMS (value.isNull());
    X_FIELD(field, value, false);
    call("DELETE FROM " + usingTable + " WHERE " + field.name() + " = (" + value.toString() + ")");
    return true;
}

bool xsDatabase::updateValue(const QSqlField &field, const QVariant &oldvalue, const QVariant &newvalue)
{
    X_PARAMS(oldvalue.isNull() || newvalue.isNull())
    X_FIELD(field, oldvalue, false);
    X_FIELD(field, newvalue, false);
    call("UPDATE " + usingTable + " SET " + field.name() + " = '" + newvalue.toString() + "' WHERE " + field.name() + " = '" + oldvalue.toString() + "'");
    return true;
}


QList<QVariant> xsDatabase::getColumn(const QSqlField& field)
{
    QList<QVariant> offset;

    X_NOT_FOUND_FIELD(field, offset);

    call("SELECT * FROM " + usingTable);
    while (query->next())
        offset.append(query->value(field.name()));

    return offset;
}


QList<QVariant> xsDatabase::getRow(int index)
{
    X_PARAMS(index < 0);
    QList<QVariant> out;

    call("SELECT * FROM " + usingTable);
    int fields = getFieldCount();
    int j = 0;

    while(j++ <= index)
        query->next();

    for (int i = 0; i < fields; i++)
            out.append(query->value(i));

    return out;
}

QVariant xsDatabase::findValue(const QSqlField& field, int id)
{
    X_PARAMS(id < 0);

    X_NOT_FOUND_FIELD(field, QVariant(QVariant::Invalid));

    call("SELECT * FROM " + usingTable);
    for (int i = 0; query->next(); i++)
        if(i == id) //TODO: Not ordered data fix!
            return query->value(field.name());

    return QVariant();
}

int xsDatabase::findValue(const QSqlField& field, const QVariant &value)
{
    X_PARAMS(value.isNull());
    X_FIELD(field, value, -1);

    call("SELECT * FROM " + usingTable);
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
    call("DELETE FROM " + usingTable );

    return true;
}

QString xsDatabase::format(const QList<QVariant> &list)
{
    QString offset;

    for(int i = 0; i < list.count(); i++)
        if(list.at(i).type() != QVariant::String)
            offset.append(list.at(i).toString() + ",");
        else
            offset.append("\"" + list.at(i).toString() + "\",");

    return offset.left(offset.length() - 1);
}

QString xsDatabase::format(const QStringList &list)
{
    int x = list.size();
    QString offset;

    for(int i = 0; i < x; i++)
            offset.append(list.at(i) + ",");

    return offset.left(offset.length() - 1);
}

QString xsDatabase::format(const QList<QSqlField> &list, bool create)
{
    QString offset;

    for(int i = 0; i < list.count(); i++)
        if(create)
            offset.append(list.at(i).name() + " " + type(list.at(i).type()) + ",");
        else
            offset.append(list.at(i).name() + ",");

    return offset.left(offset.length() - 1);
}

QStringList xsDatabase::getTables()
{
    return db->tables();
}

QList<QSqlField> xsDatabase::getFields(bool hideID)
{
    QList<QSqlField> offset;

    for(int i = hideID ? 1 : 0; i < getFieldCount(); i++)
        offset.append(driver->record(usingTable).field(i));

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
    call("SELECT * FROM " + usingTable);
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
    return type(var.type());
}

QString xsDatabase::type(const QVariant::Type &t)
{
    switch(t)
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
        qWarning() << __func__ << "() -> Type in data" << t << "is invalid!";
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

bool xsDatabase::call(const QString &textquery)
{
    if(!query->exec(textquery))
    {
        qDebug() << "QUERY ERROR -> " << getLastQuery() << endl << "ERROR MESSAGE -> " << getMessage();
        return false;
    }
    return true;
}

bool xsDatabase::Import(const QString &table, const QString &dir)
{
    X_PARAMS(dir.isEmpty() || table.isEmpty());

    QString oldTable = usingTable;
    QFile file(dir);
    file.open(QFile::ReadOnly);
    QString header = file.readLine(); //first row (name field and type)
    header.replace('\n',"");
    call("CREATE TABLE "+ table + "(" + header + ")");

    usingTable = table;

    QList<QByteArray> buffer;
    QList<QVariant> values;
    QVariant val;
    while(file.bytesAvailable())
    {
        buffer = file.readLine().split(',');
        for(int i = 0; i < buffer.count() - 1; i++)
        {
            val = QVariant(buffer.at(i));
            val.convert(driver->record(table).field(i).type());
            values.append(val);
        }
        if(!addValue(getFields(), values))
            qDebug() << query->lastQuery() << "\n" << query->lastError().text();
        values.clear();
    }
    usingTable = oldTable;
    file.close();
    return true;
}

bool xsDatabase::Export(const QString &dir)
{
    X_PARAMS(dir.isEmpty());

    QFile file(dir);
    file.open(QFile::WriteOnly);
    file.write(format(getFields(), true).toLatin1() + "\n"); //TODO: add PRIMARY KEY

    call("SELECT * FROM " + usingTable);
    while(query->next())
    {
        for(int column = 0; column < getFieldCount(); column++)
            file.write(query->value(column).toString().toLatin1() + ",");
        file.write("\n");
    }
    file.close();
    return true;
}
