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

SOURCES +=\
    xsconsole.cpp \
    xslog.cpp \
    xsarg.cpp \
    xsblowfish.cpp \
    xsdatabase.cpp \
    xsserverssl.cpp \
    xsclientssl.cpp \
    xspassword.cpp \
    xsobject.cpp \
    xsclient.cpp \
    xsserver.cpp \
    xsui.cpp

HEADERS +=\
    xslib_global.h \
    xsconsole.h \
    xslog.h \
    xslib.h \
    xsarg.h \
    xsblowfish.h \
    hexpi.h \
    xsdatabase.h \
    xsserverssl.h \
    xsclientssl.h \
    xspassword.h \
    xsobject.h \
    xsclient.h \
    xsserver.h \
    xsui.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix: LIBS += -lcurses -lncurses
