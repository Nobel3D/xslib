#ifndef XSLIB_GLOBAL_H
#define XSLIB_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QDebug>

//#define DEBUG
#define XSLIB "0.18"

#define QSL(text) QStringLiteral(text)

#define OK 1
#define FAIL 0

#define X_PARAMS(ex) { if(ex) qWarning() << __FILE__ << ":" << __LINE__ << "in" << __func__ << "() -> Malformed parameters!"; }
#define X_NOT_FOUND_FIELD(field, out) { if(!existField(field)) { qWarning() <<__func__ << "() -> Field" << field.name() << "not found!"; return out; } } //field = field value - out = return value
#define X_FIELD(field, data, out) { if(!existField(field) || field.type() != data.type()) { qWarning() <<__func__ << "() -> Field" << field.name() << field.type() << "not found or invalid input data! data =" << data.typeName(); return out; } }
//field = field value - data = variant data - out = return value
#ifdef WIN32
    #define GETUSER getenv("USERNAME")
#endif
#ifdef linux
    #define GETUSER getenv("USER")
#endif

#ifdef DEBUG
#define XSDBG_DB {                      \
qDebug() << db->connectionName();       \
qDebug() << db->databaseName();         \
qDebug() << db->driverName();           \
qDebug() << db->userName();             \
qDebug() << driver->isOpen();           \
qDebug() << driver->lastError().text(); \
qDebug() << query->executedQuery();     \
qDebug() << query->isSelect();          \
qDebug() << query->isValid();           \
qDebug() << query->isActive();          \
qDebug() << query->size();              \
}
#else
#define XSDBG_DB
#endif

#if defined(XSLIB_LIBRARY)
#  define XSLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define XSLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XSLIB_GLOBAL_H
