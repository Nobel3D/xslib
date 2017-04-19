#include "xsconsole.h"
#include <unistd.h>
#ifdef linux
    #include <term.h>
    #include <sys/ioctl.h>
#endif

xsConsole::xsConsole(QObject *parent) : QObject(parent)
{
    out = new QTextStream(stdout, QIODevice::WriteOnly);
    in = new QTextStream(stdin);
}

void xsConsole::Write(QString text)
{
    *out << text;
    out->flush();
}

void xsConsole::Read(QString& text)
{
    text = in->readLine();
}

#ifdef linux
void xsConsole::setXY(int x, int y)
{
  int err;
  if (!cur_term)
    if (setupterm( NULL, STDOUT_FILENO, &err ) == err)
      return;
  putp( tparm( tigetstr( "cup" ), y, x, 0, 0, 0, 0, 0, 0, 0 ) );
}
void xsConsole::getXY(int& x, int& y)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    y = w.ws_row;
    x = w.ws_col;
}
#endif
xsConsole& xsConsole::operator <<(QString text)
{
    Write(text);
    return *this;
}

xsConsole& xsConsole::operator >>(QString& text)
{
    Read(text);
    return *this;
}
