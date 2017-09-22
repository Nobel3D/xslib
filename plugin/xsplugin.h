#ifndef XSPLUGIN_H
#define XSPLUGIN_H

#include <QObject>

class xsPlugin
{
public:
    virtual int use() = 0;
};

Q_DECLARE_INTERFACE(xsPlugin, "org.xsoftware.stronghold.plugin")

#endif // XSPLUGIN_H
