#include "sum.h"
#include <QFileInfo>

SUM::SUM(const QString &_db)
{
    db = new xsDatabase(_db, "sum");
    if(!db->getTables().contains("stronghold"))
    {
        QList<QSqlField> fields;
        fields.append(QSqlField("id", QVariant::LongLong));
        fields.append(QSqlField("name", QVariant::String));
        fields.append(QSqlField("password", QVariant::String));
        fields.append(QSqlField("database", QVariant::String));
        fields.append(QSqlField("level", QVariant::Int));
        db->createTable("stronghold", fields);
    }

    db->useTable("stronghold");

}

bool SUM::login(const QString &name, const QString &password)
{
    int index = db->findValue(QSqlField("name", QVariant::String), name);
    if(index < 0)
        return false;
    QList<QVariant> values = db->getRow(index);

    pwhit.setPassword(password);
    if(values.isEmpty() || !pwhit.CheckDirect(values.at(2).toString()))
        return false;
    user = new User(values.at(0).toInt(), values.at(1).toString(), password, values.at(3).toString(), values.at(4).toInt());
    return true;
}


bool SUM::isFirstRun()
{
    return db->getRecordCount() <= 0;
}

QString SUM::hit()
{
    return "(" + QString::number(pwhit.getHit()) + "/" + QString::number(pwhit.getMaxHit()) + ")";
}

QStringList SUM::get(int index)
{
    QList<QVariant> in(db->getRow(index));
    QStringList out;
    for(int i = 0; i < in.size(); i++)
        out.append(in.at(i).toString());
    return out;
}

QStringList SUM::getFields()
{
    QStringList offset;
    QList<QSqlField> buffer = db->getFields();
    for(int i = 0; i < buffer.count(); i++)
        offset.append(buffer.at(i).name());
    return offset;
}

bool SUM::add(const QString &name, const QString &password, const QString &database, int level) //TODO: xsPassword only for internal use
{
    if(user->getLevel() < level)
        return false;

    db->useTable("stronghold");
    QList<QVariant> values;
    xsPassword pw(password);
    values.append(name);
    values.append(pw.getPassword());
    values.append(database);
    values.append(level);

    if(db->addValue(values))
        return true;
    else
    {
        qDebug() << db->getLastQuery() << endl << db->getMessage() << endl;
        return false;
    }
}

int SUM::add()
{
    db->addValue();
}

bool SUM::addRoot(const QString &name, const QString &password, const QString &database)
{
    db->useTable("stronghold");
    QList<QVariant> values;
    xsPassword pw(password);
    values.append(name);
    values.append(pw.getPassword());
    values.append(database);
    values.append(100);

    if(db->addValue(values))
    {
        user = new User(0, name, password, database, 100);
        return true;
    }
    else
    {
        qDebug() << db->getLastQuery() << endl << db->getMessage() << endl;
        return false;
    }
}

bool SUM::update(int field, int row, const QVariant &value)
{
    QSqlField in = db->getField(field);
    QVariant buffer = value;
    if(in.name() == "password")
    {
        xsPassword password;
        password.setPassword(value.toString());
        buffer = password.getPassword();
    }
    else if(in.name() == "level" && value.canConvert(QVariant::Int))
    {
        if(user->getLevel() < value.toInt())
            return false;
        buffer.convert(QVariant::Int);
    }
    db->updateValue(in, buffer, row);
    return true;
}

bool SUM::remove(qlonglong id)
{
    if(!db->removeValue(QSqlField("id", QVariant::LongLong), QVariant(id)))
    {
        qDebug() << db->getMessage() << endl << db->getLastQuery();
        return false;
    }
    return true;
}

QStringList SUM::getUsers()
{
    QList<QVariant> in = db->getColumn(QSqlField("name", QVariant::String));
    QStringList out;
    for(int i = 0; i < in.count(); i++)
        out.append(in.at(i).toString());
    return out;
}

User* SUM::getUser()
{
    return user;
}
