#ifndef XSLIB_GLOBAL_H
#define XSLIB_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QDebug>

#define OK 1
#define FAIL 0

#define X_PARAMS(ex) { if(ex) qWarning() << __FILE__ << ":" << __LINE__ << "in" << __func__ << "() -> Malformed parameters!"; }
#define X_NOT_FOUND_FIELD(field, out) { if(!existField(field)) { qWarning() <<__func__ << "() -> Field" << field.name() << "not found!"; return out; } } //field = field value - out = return value
#define X_FIELD(field, data, out) { if(!existField(field) || field.type() != data.type()) { qWarning() <<__func__ << "() -> Field" << field.name() << field.type() << "not found or invalid input data! data =" << data.typeName(); return out; } }
//field = field value - data = variant data - out = return value
#define X_INVALID_TYPE(data) { qWarning() << __func__ << "() -> Type in data" << data.typeName() << "is invalid!"; }
#ifdef WIN32
    #define GETUSER getenv("USERNAME")
#endif
#ifdef linux
    #define GETUSER getenv("USER")
#endif

#if defined(XSLIB_LIBRARY)
#  define XSLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define XSLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XSLIB_GLOBAL_H
