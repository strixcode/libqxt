CLEAN_TARGET     = QxtWeb
DEFINES         += BUILD_QXT_WEB
QT               = core network 
QXT              = core network 
CONVENIENCE     += $$CLEAN_TARGET

include(web.pri)
include(../qxtbase.pri)

unix|win32-g++*:QMAKE_PKGCONFIG_REQUIRES = QxtCore QxtNetwork 
