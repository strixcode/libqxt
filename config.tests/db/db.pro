CONFIG -= app_bundle
TEMPLATE = app
TARGET = db
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += main.cpp
!win32:LIBS+=-ldb
QT=core
