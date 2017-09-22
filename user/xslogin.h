 
#ifndef XSLOGIN_H
#define XSLOGIN_H

#include "xslib_global.h"
#include "database/xsdatabase.h"

enum typeLogin {PLAIN, DATABASE, DIRECT, NOLOGIN, ERROR};

class XSLIBSHARED_EXPORT xsLogin
{
public:
    xsLogin();
    xsLogin(const QString &file);
    xsLogin(xsDatabase db, const QString &table, const QString &field_user, const QString &field_pw);
    xsLogin(const QStringList &user, const QStringList &passwd);

    bool login(const QString &user, const QString& hit);
private:
    bool bLogin = false;
    typeLogin tLogin;
    QStringList strPasswd;
    QStringList strUser;
};

#endif // XSLOGIN_H
