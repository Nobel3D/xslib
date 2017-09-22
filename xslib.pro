#-------------------------------------------------
#
# Project created by QtCreator 2017-04-14T13:35:44
#
#-------------------------------------------------

QT       += core
QT       += network
QT       += sql

TARGET = xs
TEMPLATE = lib

DEFINES += XSLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    plugin/xsplugin.h \
    database/xsdatabase.h \
    net/xsclient.h \
    net/xsclientssl.h \
    net/xsserver.h \
    net/xsserverssl.h \
    pty/xsconsole.h \
    utils/hexpi.h \
    utils/xsarg.h \
    utils/xsblowfish.h \
    utils/xslog.h \
    utils/xspassword.h \
    xslib.h \
    xslib_global.h \
    user/xslogin.h \
    utils/xshash.h \
    stronghold/pem.h \
    stronghold/sum.h \
    stronghold/user.h \
    ui/xsui.h \

SOURCES += \
    plugin/xsplugin.cpp \
    database/xsdatabase.cpp \
    net/xsclient.cpp \
    net/xsclientssl.cpp \
    net/xsserver.cpp \
    net/xsserverssl.cpp \
    pty/xsconsole.cpp \
    utils/xsarg.cpp \
    utils/xsblowfish.cpp \
    utils/xslog.cpp \
    utils/xspassword.cpp \
    user/xslogin.cpp \
    utils/xshash.cpp \
    stronghold/pem.cpp \
    stronghold/sum.cpp \
    stronghold/user.cpp \
    ui/xsui.cpp \


unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix: LIBS += -lcurses -lncurses
