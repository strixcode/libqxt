require "qt-support"


qt_modules["QxtCore"] = {
        depends =  { "QtCore" }
}
qt_modules["QxtNetwork"] = {
        depends =  { "QtCore", "QxtCore" },
        -- only if ssh was enabled
        links = { "crypto" }
}
qt_modules["QxtWidgets"] = {
        depends =  { "QtCore", "QxtCore", "QtGui", "QtWidgets" }
}


