#include "xsui.h"

xsUi::xsUi()
{

}

QIcon xsUi::getFromIcons(const QString &icon)
{
    QString in = QFileInfo(icon).baseName();
    if(QIcon::hasThemeIcon(in))
        return QIcon::fromTheme(in);
    else
        return QIcon(icon);
}
