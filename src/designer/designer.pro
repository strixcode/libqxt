CLEAN_TARGET     = QxtDesignerPlugins
DEFINES         += BUILD_QXT_DESIGNER
QT               = core gui widgets
QXT              = core widgets
CONVENIENCE     +=

include(designer.pri)
include(../qxtbase.pri)

CONFIG          += designer plugin
target.path      = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS         = target

unix|win32-g++*:QMAKE_PKGCONFIG_REQUIRES += QxtCore QxtGui
