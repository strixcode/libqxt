CLEAN_TARGET     = QxtBerkeley
DEFINES         += BUILD_QXT_BERKELEY
QT               = core
QXT              = core
CONVENIENCE     += $$CLEAN_TARGET

include(bdb.pri)
include(../qxtbase.pri)

!win32:LIBS     += -ldb

unix|win32-g++*:QMAKE_PKGCONFIG_REQUIRES += QxtCore
