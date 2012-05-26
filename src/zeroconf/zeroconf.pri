INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += qxtdiscoverableservice.h
HEADERS += qxtdiscoverableservice_p.h
HEADERS += qxtservicebrowser.h
HEADERS += qxtservicebrowser_p.h
HEADERS += qxtdiscoverableservicename.h
HEADERS += qxtzeroconf.h

SOURCES += qxtdiscoverableservice.cpp
SOURCES += qxtservicebrowser.cpp
SOURCES += qxtdiscoverableservicename.cpp

!contains(CONFIG,NO_AVAHI): unix : !macx {
        INCLUDEPATH += $$PWD/avahi
        DEFINES += USE_AVAHI
        SOURCES += avahi/qxtmdns_avahi.cpp
        SOURCES += avahi/qxtavahipoll.cpp
        HEADERS += avahi/qxtmdns_avahi.h
        HEADERS += avahi/qxtmdns_avahi_p.h
        HEADERS += avahi/qxtavahipoll.h
        HEADERS += avahi/qxtavahipoll_p.h
} else {
        INCLUDEPATH += $$PWD/bonjour
        SOURCES += bonjour/qxtmdns_bonjour.cpp
        HEADERS += bonjour/qxtmdns_bonjour.h
}
