TEMPLATE = app
TARGET = qxtjsonrpc
DESTDIR = ../../bin
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += main.cpp
CONFIG += console
QXT = core network
include($$QXT_SOURCE_TREE/src/qxtlibs.pri)
INSTALLS += target
target.path = $${QXT_INSTALL_BINS}
