INCLUDEPATH += $${QXT_SOURCE_TREE}/include
DEPENDPATH += $${QXT_SOURCE_TREE}/include
QMAKE_RPATHDIR += $${QXT_BUILD_TREE}/lib
macx:LIBS += -F$${QXT_BUILD_TREE}/lib
LIBS += -L$${QXT_BUILD_TREE}/lib

# this is just a copy from qt
defineTest(qxtQtAddLibrary) {
    INCLUDEPATH -= $$QMAKE_INCDIR_QT/$$1
    INCLUDEPATH = $$QMAKE_INCDIR_QT/$$1 $$INCLUDEPATH

    LIB_NAME = $$1
    unset(LINKAGE)
    mac {
       CONFIG(qt_framework, qt_framework|qt_no_framework) { #forced
          QMAKE_FRAMEWORKPATH *= $${QMAKE_LIBDIR_QT}
          FRAMEWORK_INCLUDE = $$QMAKE_LIBDIR_QT/$${LIB_NAME}.framework/Headers
          !qt_no_framework_direct_includes:exists($$FRAMEWORK_INCLUDE) {
             INCLUDEPATH -= $$FRAMEWORK_INCLUDE
             INCLUDEPATH = $$FRAMEWORK_INCLUDE $$INCLUDEPATH
           }
           LINKAGE = -framework $${LIB_NAME}$${QT_LIBINFIX}
        } else:!qt_no_framework { #detection
           for(frmwrk_dir, $$list($$QMAKE_LIBDIR_QT $$QMAKE_LIBDIR $$(DYLD_FRAMEWORK_PATH) /Library/Frameworks)) {
              exists($${frmwrk_dir}/$${LIB_NAME}.framework) {
                QMAKE_FRAMEWORKPATH *= $${frmwrk_dir}
                FRAMEWORK_INCLUDE = $$frmwrk_dir/$${LIB_NAME}.framework/Headers
                !qt_no_framework_direct_includes:exists($$FRAMEWORK_INCLUDE) {
                  INCLUDEPATH -= $$FRAMEWORK_INCLUDE
                  INCLUDEPATH = $$FRAMEWORK_INCLUDE $$INCLUDEPATH
                }
                LINKAGE = -framework $${LIB_NAME}
                break()
              }
           }
       }
    }
    symbian {
        isEqual(LIB_NAME, QtCore) {
            #workaround for dependency from f32file.h on e32svr.h which has moved location in symbian3
            INCLUDEPATH *= $$OS_LAYER_SYSTEMINCLUDE
        } else:isEqual(LIB_NAME, QtGui) {
            # Needed for #include <QtGui> because qs60mainapplication.h includes aknapp.h
            INCLUDEPATH *= $$MW_LAYER_SYSTEMINCLUDE
        } else:isEqual(LIB_NAME, QtWebKit) {
            # Needed for because relative inclusion problem in toolchain
            INCLUDEPATH *= $$QMAKE_INCDIR_QT/QtXmlPatterns
            INCLUDEPATH *= $$QMAKE_INCDIR_QT/QtNetwork
            isEmpty(TARGET.CAPABILITY): TARGET.CAPABILITY = NetworkServices
            isEmpty(TARGET.EPOCHEAPSIZE):TARGET.EPOCHEAPSIZE = 0x20000 0x2000000
        } else:isEqual(LIB_NAME, QtXmlPatterns) {
            # Needed for #include <QtXmlPatterns/QtXmlPatterns> because relative inclusion problem in toolchain
            INCLUDEPATH *= $$QMAKE_INCDIR_QT/QtNetwork
        } else:isEqual(LIB_NAME, QtNetwork) {
            isEmpty(TARGET.CAPABILITY): TARGET.CAPABILITY = NetworkServices
        } else:isEqual(LIB_NAME, QtDeclarative) {
            isEmpty(TARGET.CAPABILITY): TARGET.CAPABILITY = NetworkServices
            isEmpty(TARGET.EPOCHEAPSIZE):TARGET.EPOCHEAPSIZE = 0x20000 0x2000000
        }
        export(TARGET.EPOCHEAPSIZE)
        export(TARGET.CAPABILITY)
    }
    isEmpty(LINKAGE) {
       if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
           win32:LINKAGE = -l$${LIB_NAME}$${QT_LIBINFIX}d
           mac:LINKAGE = -l$${LIB_NAME}$${QT_LIBINFIX}_debug
       }
       isEmpty(LINKAGE):LINKAGE = -l$${LIB_NAME}$${QT_LIBINFIX}
    }
    !isEmpty(QMAKE_LSB) {
        QMAKE_LFLAGS *= --lsb-libpath=$$$$QMAKE_LIBDIR_QT
        QMAKE_LFLAGS *= -L/opt/lsb/lib
        QMAKE_LFLAGS *= --lsb-shared-libs=$${LIB_NAME}$${QT_LIBINFIX}
    }
    LIBS += $$LINKAGE
    export(LIBS)
    export(INCLUDEPATH)
    export(QMAKE_FRAMEWORKPATH)
    export(QMAKE_LFLAGS)
    return(true)
}


isEqual(QT_MAJOR_VERSION, 5) {
    defineTest(qxtAddLibrary) {
        INCLUDEPATH += $${QXT_SOURCE_TREE}/src/$$1
        INCLUDEPATH += $${QXT_SOURCE_TREE}/include/$$2
        DEPENDPATH += $${QXT_SOURCE_TREE}/src/$$1
        DEPENDPATH += $${QXT_SOURCE_TREE}/include/$$2
            qxtQtAddLibrary($$2)
    }
}
else {
    defineTest(qxtAddLibrary) {
        INCLUDEPATH += $${QXT_SOURCE_TREE}/src/$$1
        INCLUDEPATH += $${QXT_SOURCE_TREE}/include/$$2
        DEPENDPATH += $${QXT_SOURCE_TREE}/src/$$1
        DEPENDPATH += $${QXT_SOURCE_TREE}/include/$$2
            qtAddLibrary($$2)
    }
}


contains(QXT, berkeley) {
    qxtAddLibrary(berkeley, QxtBerkeley)
    QXT += core
}

contains(QXT, web) {
    qxtAddLibrary(web, QxtWeb)
    QXT += core network
    QT  += network
}


isEqual(QT_MAJOR_VERSION, 5) {
	contains(QXT, widgets) {
		qxtAddLibrary(widgets, QxtWidgets)
		QXT += core
		QT  += gui widgets
	}
} else {
	contains(QXT, widgets) {
		qxtAddLibrary(widgets, QxtWidgets)
		QXT += core
		QT  += gui
	}
}


contains(QXT, network) {
    qxtAddLibrary(network, QxtNetwork)
    QXT += core
    QT  += network
}

contains(QXT, sql) {
    qxtAddLibrary(sql, QxtSql)
    QXT += core
    QT  += sql
}

contains(QXT, core) {
    qxtAddLibrary(core, QxtCore)
}
