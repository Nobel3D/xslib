#ifndef XSUI_H
#define XSUI_H

#include "xslib_global.h"
#include <QIcon>
#include <QFileInfo>

class XSLIBSHARED_EXPORT xsUi
{
public:
    xsUi();
    static QIcon getFromIcons(const QString &icon);
};

#endif // XSUI_H
