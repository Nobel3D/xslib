 
#include "xslogin.h"

xsLogin::xsLogin()
{
    tLogin = NOLOGIN;
}

xsLogin::xsLogin(const QString &file)
{
    QFile f(file);
    if(f.exists())
    {
        qDebug() << "[E] xsLogin: login file on " << file << " doesn't exist!";
        tLogin = ERROR;
        return;
    }
    QList<QByteArray> s;
    int i = 0;
    f.open(QFile::ReadOnly);
    while(f.bytesAvailable())
    {
        s = f.readLine().split(':');
        if(s.count() != 2)
        {
            qDebug() << "[W] xsLogin: login file on " << file << ":" << QString::number(i)
                       << " has " << QString::number(s.count()) << " values!";
        }
        else
        {
            strUser.append(s.at(0));
            strPasswd.append(s.at(1));
        }
        i++;
    }
    tLogin = PLAIN;
}

xsLogin::xsLogin(xsDatabase db, const QString &table, const QString &field_user, const QString &field_pw)
{
    if(!db.useTable(table))
    {
        qDebug() << "[E] xsLogin: table " << table << " of login database on " << db.name() << " doesn't exist!";
        tLogin = ERROR;
        return;
    }
    if(!db.existField(QSqlField(field_user)))
    {
        qDebug() << "[E] xsLogin: field name " << field_user << " of login database on " << db.name() << " doesn't exist!";
        tLogin = ERROR;
        return;
    }
    if(!db.existField(QSqlField(field_pw)))
    {
        qDebug() << "[E] xsLogin: field name " << field_pw << " of login database on " << db.name() << " doesn't exist!";
        tLogin = ERROR;
        return;
    }

    QList<QVariant> u;
    QList<QVariant> p;

    u = db.getColumn(QSqlField(field_user));
    p = db.getColumn(QSqlField(field_pw));

    for(int i = 0; i < u.count(); i++)
    {
        strUser.append(u.at(i).toString());
        strPasswd.append(p.at(i).toString());
    }
    tLogin = DATABASE;
}

xsLogin::xsLogin(const QStringList &user, const QStringList &passwd)
{
    int u = user.count();
    int p = passwd.count();
    if(u == 0 || p == 0 || u != p)
    {
        qDebug() << "[E] xsLogin: direct login is malformed with user.count = " << QString::number(u)
                 << " and passwd.count = " << QString::number(p);
        tLogin = ERROR;
        return;
    }
    strUser = user;
    strPasswd = passwd;
    tLogin = DIRECT;
}

bool xsLogin::login(const QString &user, const QString &hit)
{
    if(tLogin == ERROR)
    {
        qDebug() << "[E] xsLogin: error into data loading, please check logs in order to get more informations!";
        return false;
    }
    if(tLogin == NOLOGIN)
        return true;

    for(int i = 0; i < strUser.count(); i++)
        if(strUser.at(i) == user)
        {
           if(strPasswd.at(i) == hit)
               return true;
        }
    return false;
}
