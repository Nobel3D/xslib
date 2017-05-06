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
    src/xsconsole.cpp \
    src/xslog.cpp \
    src/xsarg.cpp \
    src/xsblowfish.cpp \
    src/xsdatabase.cpp \
    src/xsserverssl.cpp \
    src/xsclientssl.cpp \
    src/xspassword.cpp

HEADERS +=\
    src/xslib_global.h \
    src/xsconsole.h \
    src/xslog.h \
    src/xslib.h \
    src/xsarg.h \
    src/xsblowfish.h \
    src/hexpi.h \
    src/xsdatabase.h \
    src/xsserverssl.h \
    src/xsclientssl.h \
    src/xspassword.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix: LIBS += -lcurses -lncurses
