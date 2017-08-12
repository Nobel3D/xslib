#include "xsui.h"

xsUi::xsUi()
{

}

QIcon xsUi::getFromIcons(const QString &icon)
{
    if(QIcon::hasThemeIcon(icon))
        return QIcon::fromTheme(icon);
    else
        return QIcon(icon);
}
