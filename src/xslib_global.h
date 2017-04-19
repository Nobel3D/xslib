#ifndef XSLIB_GLOBAL_H
#define XSLIB_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QObject>
#include <QString>

#define OK 1
#define FAIL -1

#if defined(XSLIB_LIBRARY)
#  define XSLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define XSLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XSLIB_GLOBAL_H
