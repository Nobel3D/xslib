#include "xsconsole.h"
#include <unistd.h>
#include <stdio.h>
#ifdef linux
    #include <term.h>
    #include <sys/ioctl.h>
#endif

#ifdef WIN32
    #include<conio.h>
    #define ENTER_KEY 13
#endif
#ifdef linux
    #include <termios.h>
    #define ENTER_KEY 10
    xsPassword xsConsole::ReadPasswd(bool copyClear, QCryptographicHash::Algorithm type, int maxhit)
    {
        QString offset;
        termios oldattr, newattr;
        int ch;
        tcgetattr( STDIN_FILENO, &oldattr );
        newattr = oldattr;
        newattr.c_lflag &= ~( ICANON | ECHO );
        tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
        while(true)
        {
        ch = getchar();
        if(ch == ENTER_KEY)
            break;
        putchar('*');
        offset.append(ch);
        }
        putchar('\n');
        tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
        return xsPassword(offset, copyClear, type, maxhit);
    }
#endif

QString xsConsole::Shell(QString var1, QString var2) //TODO: ADD LAST INPUT
{
    QString offset;
    xsConsole() << "[" << var1 << ":" << var2 << "]$ ";
    xsConsole() >> offset;
    return offset;
}

xsConsole::xsConsole()
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
