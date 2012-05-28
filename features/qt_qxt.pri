include(qxtvars.prf)

QT.qxtcore.VERSION = $$QXT_VERSION
QT.qxtcore.MAJOR_VERSION = 0
QT.qxtcore.MINOR_VERSION = 7
QT.qxtcore.PATCH_VERSION = 0

QT.qxtcore.name = QxtCore
QT.qxtcore.bins = $$QXT_INSTALL_BINS
QT.qxtcore.includes = $$QXT_INSTALL_HEADERS $$QXT_INSTALL_HEADERS/QxtCore/ $$QXT_INSTALL_PREFIX/src/core/
QT.qxtcore.libs = $$QXT_INSTALL_LIBS
QT.qxtcore.depends = core

QT_CONFIG += qxtcore


QT.qxtwidgets.VERSION = $$QXT_VERSION
QT.qxtwidgets.MAJOR_VERSION = 0
QT.qxtwidgets.MINOR_VERSION = 7
QT.qxtwidgets.PATCH_VERSION = 0

QT.qxtwidgets.name = QxtWidgets
QT.qxtwidgets.bins = $$QXT_INSTALL_BINS
QT.qxtwidgets.includes = $$QXT_INSTALL_HEADERS $$QXT_INSTALL_HEADERS/QxtWidgets $$QXT_INSTALL_PREFIX/src/widgets/
QT.qxtwidgets.libs = $$QXT_INSTALL_LIBS
QT.qxtwidgets.depends = core qxtcore gui widgets

QT_CONFIG += qxtwidgets


QT.qxtnetwork.VERSION = $$QXT_VERSION
QT.qxtnetwork.MAJOR_VERSION = 0
QT.qxtnetwork.MINOR_VERSION = 7
QT.qxtnetwork.PATCH_VERSION = 0

QT.qxtnetwork.name = QxtNetwork
QT.qxtnetwork.bins = $$QXT_INSTALL_BINS
QT.qxtnetwork.includes = $$QXT_INSTALL_HEADERS $$QXT_INSTALL_HEADERS/QxtNetwork $$QXT_INSTALL_PREFIX/src/network
QT.qxtnetwork.libs = $$QXT_INSTALL_LIBS
QT.qxtnetwork.depends = core qxtcore network

QT_CONFIG += qxtnetwork


QT.qxtsql.VERSION = $$QXT_VERSION
QT.qxtsql.MAJOR_VERSION = 0
QT.qxtsql.MINOR_VERSION = 7
QT.qxtsql.PATCH_VERSION = 0

QT.qxtsql.name = QxtSql
QT.qxtsql.bins = $$QXT_INSTALL_BINS
QT.qxtsql.includes = $$QXT_INSTALL_HEADERS $$QXT_INSTALL_HEADERS/QxtSql $$QXT_INSTALL_PREFIX/src/sql
QT.qxtsql.libs = $$QXT_INSTALL_LIBS
QT.qxtsql.depends = core qxtcore sql

QT_CONFIG += qxtsql


QT.qxtweb.VERSION = $$QXT_VERSION
QT.qxtweb.MAJOR_VERSION = 0
QT.qxtweb.MINOR_VERSION = 7
QT.qxtweb.PATCH_VERSION = 0

QT.qxtweb.name = QxtSql
QT.qxtweb.bins = $$QXT_INSTALL_BINS
QT.qxtweb.includes = $$QXT_INSTALL_HEADERS $$QXT_INSTALL_HEADERS/QxtWeb $$QXT_INSTALL_PREFIX/src/web
QT.qxtweb.libs = $$QXT_INSTALL_LIBS
QT.qxtweb.depends = core network qxtcore qxtnetwork

QT_CONFIG += qxtweb

