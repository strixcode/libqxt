win32:include(../../depends.pri)
TEMPLATE = app
TARGET = openssl
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += main.cpp
!win32:LIBS+= -lcrypto -lssl
QT=core
CONFIG -= app_bundle

