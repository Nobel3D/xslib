#include "xsarg.h"

xsArg::xsArg(QObject *parent) : QObject(parent)
{

}
xsArg::xsArg(int argc, char **argv)
{
    while(argc--)
        args.append(argv[argc]);
}

int xsArg::check(int value)
{
    return args.size() <= value ? OK : FAIL;
}

QString xsArg::operator [] (QString flag)
{
    for(int i = 0; i < args.size(); i++)
        if(args.value(i) == flag)
            return args[i + 1];
}

QString xsArg::operator [] (int index)
{
    QString h = args.value(0);
    return  args.size() >= index ? args.value(index) : "";
}
