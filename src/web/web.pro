CLEAN_TARGET     = QxtWeb
DEFINES         += BUILD_QXT_WEB
QT               = core network script
QXT              = core network script
CONVENIENCE     += $$CLEAN_TARGET

include(web.pri)
include(../qxtbase.pri)

unix|win32-g++*:QMAKE_PKGCONFIG_REQUIRES = QxtCore QxtNetwork QtScript
