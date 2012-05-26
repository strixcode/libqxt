win32:include(../../depends.pri)
TEMPLATE = app
TARGET = xrandr
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += main.cpp
!win32:LIBS+=-lXrandr
