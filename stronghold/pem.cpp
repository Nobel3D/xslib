#include "pem.h"

#include <QFile>
#include <QFileInfoList>
#include <utils/xspassword.h>

PEM::PEM(User *_user)
{
    user = _user;
    db = new xsDatabase(user->getDatabase().replace('~',QDir::homePath()), "pem", RAM, QDir::homePath() + "/xsDatabase.script");
    //qDebug() << dbuffer->clone(QDir::homePath() + "/new.db");
    //db = new xsDatabase(QDir::homePath() + "/new.db", "dbuffer", DirectFile, QDir::homePath() + "/xsDatabase.script");
}

int PEM::add(QStringList &arg)
{
    QList<QVariant> out;
    for(int i = 1; i < arg.size(); i++)
        out.append(user->encrypt(arg.at(i)));

    if(!db->addValue(out))
    {
        qDebug() << "Impossible to add some value into " << db->getTable() << endl <<
                    db->getMessage() << endl << db->getLastQuery() << endl;
        return -1;
    }
    return db->findValue(db->getField(0), db->getRecordCount() - 1).toInt(); //TODO: Better management
}

int PEM::add(const QStringList &fields, const QStringList &values)
{
    int count = fields.count();
    if(count != values.count())
        return FAIL;

    QList<QSqlField> fieldlist;
    QList<QVariant> valuelist;

    for(int i = 0; i < count; i++)
    {
        fieldlist.append(db->getField(fields.at(i)));
        if(fieldlist.at(i).type() == QVariant::String)
            valuelist.append(user->encrypt(values.at(i)));
        else
            valuelist.append(values.at(i));
    }
    db->addValue(fieldlist, valuelist);

    return db->findValue(db->getField(0), db->getRecordCount() - 1).toInt() + 1; //TODO: Better management
}

int PEM::add()
{
    db->addValue();
    return db->findValue(db->getField(0), db->getRecordCount() - 1).toInt() + 1; //TODO: Better management
}

QStringList PEM::get(const QString& field, const QString& value)
{
    QStringList offset;

    int x = db->findValue(db->getField(field), user->encrypt(value));
    if(x < 0)
        return offset;
    for(int i = 1; i < db->getFieldCount(); i++)
        offset.append(user->decrypt(db->findValue(db->getField(i),x)));

    return offset;
}

QStringList PEM::get(const QString& field)
{
    QList<QVariant> offset;

    offset = db->getColumn(db->getField(field));
    for(int i = 0; i < offset.size(); i++)
        offset.replace(i,user->decrypt(offset.at(i)));

    return convert(offset);
}

QStringList PEM::get(int row)
{
    QList<QVariant> offset(db->getRow(row));
    for(int i = 0; i < offset.size(); i++)
        offset.replace(i,user->decrypt(offset.at(i)));
    return convert(offset);
}


int PEM::update(const QString &field, const QString &oldvalue, const QString &newvalue)
{
    if(!db->updateValue(db->getField(field), user->encrypt(oldvalue), user->encrypt(newvalue)))
    {
        qDebug() << db->getMessage() << endl << db->getLastQuery();
        return FAIL;
    }
    return OK;
}

int PEM::update(const int field, const QString &oldvalue, int id)
{
    if(!db->updateValue(db->getField(field), user->encrypt(oldvalue), id))
    {
        qDebug() << db->getMessage() << endl << db->getLastQuery();
        return FAIL;
    }
    return OK;
}

int PEM::update(const QString &field, int row, const QString &newvalue)
{
    if(!db->updateValue(db->getField(field), user->encrypt(newvalue), row)) //TODO: Check this expr
    {
        //strStatus = db->getMessage() + endl + db->getLastQuery();
        return FAIL;
    }
    return OK;
}

bool PEM::remove(qlonglong id)
{
    if(!db->removeValue(QSqlField("id", QVariant::LongLong), QVariant(id)))
    {
        qDebug() << db->getMessage() << endl << db->getLastQuery();
        return false;
    }
    return true;
}

int PEM::tableUse(const QString &table)
{
    if(!db->useTable(table))
    {
        qDebug() << "Impossible to open table " << table;
        return FAIL;
    }
    return OK;
}

int PEM::tableCreate(const QString &table, const QStringList &fields)
{
    QList<QSqlField> format;

    format.append(QSqlField("id", QVariant::LongLong));

    for(int i = 0; i < fields.size(); i++)
        format.append(QSqlField(fields.at(i), QVariant::String));

    if(db->createTable(table, format))
        return OK;

    return FAIL;
}
bool PEM::tableDelete(const QString &table)
{
    if(!db->deleteTable(table))
        return true;
    else
        return false;
}

QStringList PEM::tableList()
{
    return db->getTables();
}

QStringList PEM::tableField()
{
    QStringList offset;
    QList<QSqlField> buffer = db->getFields();
    for(int i = 0; i < buffer.count(); i++)
        offset.append(buffer.at(i).name());
    return offset;
}

QString PEM::tableActive()
{
    return db->getTable();
}

int PEM::commit()
{
    return db->script();
}

int PEM::commitClear()
{
    return db->scriptClear();
}

bool PEM::exportTable(const QString &dir)
{
    X_PARAMS(dir.isEmpty());

    QStringList buffer;
    QFile file(dir);
    file.open(QFile::WriteOnly);
    file.write(db->format(db->getFields()).toUtf8() + "\n");

    for(int i = 0; i < db->getRecordCount() + 1; i++)
    {
        buffer = get(i);
        for(int column = 0; column < buffer.count(); column++)
            file.write(buffer.at(column).toUtf8() + ",");
        file.write("\n");
    }
    file.close();
    return true;
}

bool PEM::exportDatabase(const QDir &dir)
{
    QStringList tables = db->getTables();
    for(int i = 0; i < tables.count(); i++)
    {
        db->useTable(tables.at(i));
        exportTable(dir.path() + "/" + tables.at(i) + ".csv");
    }
}

bool PEM::importTable(const QString &name, const QString &dir)
{
    X_PARAMS(dir.isEmpty());

    QString oldTable = db->getTable();
    QFile file(dir);
    file.open(QFile::ReadOnly);
    QStringList header = QString(file.readLine()).split(','); //first row (name field and type)
    header.removeFirst();
    header.last().replace('\n',"");
    if(!tableCreate(name,header))
        return false;

    tableUse(name);
    commit();

    QStringList values;
    while(file.bytesAvailable())
    {
        values = QString(file.readLine()).split(',');
        values.removeLast();
        if(!add(tableField(), values))
            qDebug() << db->getLastQuery() << "\n" << db->getMessage();
        values.clear();
    }
    tableUse(oldTable);
    file.close();
    return true;
}

bool PEM::importDatabase(const QFileInfoList &dir)
{
    for(int i = 0; i < dir.count(); i++)
        importTable(dir.at(i).completeBaseName(), dir.at(i).absoluteFilePath());
}

QString PEM::generatePassword(const QStringList &arg)
{
    bool symbols = false, spaces = false, unicode = false, numbers = false, lowers = false, uppers = false, ok = false;
    int length = arg.at(1).toInt(&ok);
    if(arg.count() < 2 || !ok)
        return "";

    if(arg.contains("sym", Qt::CaseInsensitive))
        symbols = true;
    if(arg.contains("space", Qt::CaseInsensitive))
        spaces = true;
    if(arg.contains("uni", Qt::CaseInsensitive))
        unicode = true;
    if(arg.contains("num", Qt::CaseInsensitive))
        numbers = true;
    if(arg.contains("low", Qt::CaseInsensitive))
         lowers = true;
    if(arg.contains("up", Qt::CaseInsensitive))
         uppers = true;

    if(arg.count() == 2)
        return xsPassword::generate(length);
    else if(unicode)
        return xsPassword::generate(length, symbols, spaces, true);
    else
        return xsPassword::generate(length, symbols, spaces, false, numbers, lowers, uppers);
}

QStringList PEM::convert(const QList<QVariant>& data)
{
    QStringList out;
    for(int i = 0; i < data.count(); i++)
        out.append(data.at(i).toString());
    return out;
}

