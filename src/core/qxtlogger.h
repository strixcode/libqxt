
/****************************************************************************
** Copyright (c) 2006 - 2011, the LibQxt project.
** See the Qxt AUTHORS file for a list of authors and copyright holders.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the LibQxt project nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** <http://libqxt.org>  <foundation@libqxt.org>
*****************************************************************************/

#ifndef QXTLOGGER_H
#define QXTLOGGER_H

#include "qxtglobal.h"
#include <QObject>
#include <QVariant>
#include <QString>
#include <QStringList>
#include <QFlags>

class QxtLoggerPrivate;
class QxtLogStream;
class QxtLoggerEngine;

void QxtLoggerMessageHandler(QtMsgType type, const char *msg);

class QXT_CORE_EXPORT QxtLogger : public QObject
{
    Q_OBJECT
    QXT_DECLARE_PRIVATE(QxtLogger)

    // Constructor & Destructor.  Made private as QxtLogger is implemented as a singleton.
    QxtLogger();
    ~QxtLogger();

public:
    /*******************************************************************************
    Defines for a bitmask to enable/disable logging levels.
    Arranged in levels from (assumed) most verbose to most important.
    *******************************************************************************/
    enum LogLevel
    {
        NoLevels       = 0,     /**< No Levels enabled */
        TraceLevel     = 1 << 0,  /**< The most verbose, flags trace() messages to be logged */
        DebugLevel     = 1 << 1,  /**< Flags debug() messages to be logged */
        InfoLevel      = 1 << 2,  /**< Flags info() messages to be logged */
        WarningLevel   = 1 << 3,  /**< Flags warning() messages to be logged */
        ErrorLevel     = 1 << 4,  /**< Flags error() messages to be logged */
        CriticalLevel  = 1 << 5,  /**< Flags critical() messages to be logged */
        FatalLevel     = 1 << 6,  /**< Flags fatal() messages to be logged */
        WriteLevel     = 1 << 7,  /**< The most important, flags write() messages to be logged */
        AllLevels      = TraceLevel | DebugLevel | InfoLevel | WarningLevel | ErrorLevel | CriticalLevel | FatalLevel | WriteLevel /**< Enables all log levels */
    };
    Q_DECLARE_FLAGS(LogLevels, LogLevel)

    /* Sone useful things */
    static QString logLevelToString(LogLevel level);
    static QxtLogger::LogLevel stringToLogLevel(const QString& level);
    static QxtLogger* getInstance();

    void initLoggerEngine(const QString& engineName);
    void killLoggerEngine(const QString& engineName);

    // Functions to install or remove QxtLogger as a handler for qDebug, qFatal, etc...
    void installAsMessageHandler();
    void removeAsMessageHandler();

    //Functions for adding and removing loggers.
    void addLoggerEngine(const QString& engineName, QxtLoggerEngine *engine);
    void removeLoggerEngine(const QString& engineName);
    void removeLoggerEngine(QxtLoggerEngine *engine);
    QxtLoggerEngine* takeLoggerEngine(const QString& engineName);
    QxtLoggerEngine* engine(const QString& engineName);

    // Functions for checking loggers.
    QStringList allLoggerEngines() const;
    QStringList allEnabledLoggerEngines() const;
    QStringList allEnabledLoggerEngines(LogLevel level) const;
    QStringList allDisabledLoggerEngines() const;

    bool   isLogLevelEnabled(const QString& engineName, LogLevel level) const;
    bool   isLoggerEngine(const QString& engineName) const;
    bool   isLoggerEngineEnabled(const QString& engineName) const;

    /*******************************************************************************
    Streaming!
    *******************************************************************************/
    QxtLogStream stream(LogLevel level);
    QxtLogStream trace();
    QxtLogStream debug();
    QxtLogStream info();
    QxtLogStream warning();
    QxtLogStream error();
    QxtLogStream critical();
    QxtLogStream fatal();
    QxtLogStream write();

    /*******************************************************************************
    Log Level enable and disable: The 1-param functions enable/disable that level on
    ALL log engines.  The 2-param functions enable/disable that on a named logger.
    *******************************************************************************/
    void enableLogLevels(LogLevels levels);
    void enableLogLevels(const QString& engineName, LogLevels levels);
    void enableAllLogLevels();
    void enableAllLogLevels(const QString& engineName);
    void enableLoggerEngine(const QString& engineName);

    void disableLogLevels(LogLevels levels);
    void disableLogLevels(const QString& engineName, LogLevels levels);
    void disableAllLogLevels();
    void disableAllLogLevels(const QString& engineName);
    void disableLoggerEngine(const QString& engineName);

    void setMinimumLevel(LogLevel level);
    void setMinimumLevel(const QString& engineName, LogLevel level);

public Q_SLOTS:
    /*******************************************************************************
    Logging Functions: what the QxtLogger is all about.
    *******************************************************************************/
    void info(const QVariant& message, const QVariant& msg1 = QVariant(),
              const QVariant& msg2 = QVariant(), const QVariant& msg3 = QVariant(),
              const QVariant& msg4 = QVariant(), const QVariant& msg5 = QVariant(),
              const QVariant& msg6 = QVariant(), const QVariant& msg7 = QVariant(),
              const QVariant& msg8 = QVariant(), const QVariant& msg9 = QVariant());
    void trace(const QVariant& message, const QVariant& msg1 = QVariant(),
               const QVariant& msg2 = QVariant(), const QVariant& msg3 = QVariant(),
               const QVariant& msg4 = QVariant(), const QVariant& msg5 = QVariant(),
               const QVariant& msg6 = QVariant(), const QVariant& msg7 = QVariant(),
               const QVariant& msg8 = QVariant(), const QVariant& msg9 = QVariant());
    void warning(const QVariant& message, const QVariant& msg1 = QVariant(),
                 const QVariant& msg2 = QVariant(), const QVariant& msg3 = QVariant(),
                 const QVariant& msg4 = QVariant(), const QVariant& msg5 = QVariant(),
                 const QVariant& msg6 = QVariant(), const QVariant& msg7 = QVariant(),
                 const QVariant& msg8 = QVariant(), const QVariant& msg9 = QVariant());
    void error(const QVariant& message, const QVariant& msg1 = QVariant(),
               const QVariant& msg2 = QVariant(), const QVariant& msg3 = QVariant(),
               const QVariant& msg4 = QVariant(), const QVariant& msg5 = QVariant(),
               const QVariant& msg6 = QVariant(), const QVariant& msg7 = QVariant(),
               const QVariant& msg8 = QVariant(), const QVariant& msg9 = QVariant());
    void debug(const QVariant& message, const QVariant& msg1 = QVariant(),
               const QVariant& msg2 = QVariant(), const QVariant& msg3 = QVariant(),
               const QVariant& msg4 = QVariant(), const QVariant& msg5 = QVariant(),
               const QVariant& msg6 = QVariant(), const QVariant& msg7 = QVariant(),
               const QVariant& msg8 = QVariant(), const QVariant& msg9 = QVariant());
    void critical(const QVariant& message, const QVariant& msg1 = QVariant(),
                  const QVariant& msg2 = QVariant(), const QVariant& msg3 = QVariant(),
                  const QVariant& msg4 = QVariant(), const QVariant& msg5 = QVariant(),
                  const QVariant& msg6 = QVariant(), const QVariant& msg7 = QVariant(),
                  const QVariant& msg8 = QVariant(), const QVariant& msg9 = QVariant());
    void fatal(const QVariant& message, const QVariant& msg1 = QVariant(),
               const QVariant& msg2 = QVariant(), const QVariant& msg3 = QVariant(),
               const QVariant& msg4 = QVariant(), const QVariant& msg5 = QVariant(),
               const QVariant& msg6 = QVariant(), const QVariant& msg7 = QVariant(),
               const QVariant& msg8 = QVariant(), const QVariant& msg9 = QVariant());
    void write(const QVariant& message, const QVariant& msg1 = QVariant(),
               const QVariant& msg2 = QVariant(), const QVariant& msg3 = QVariant(),
               const QVariant& msg4 = QVariant(), const QVariant& msg5 = QVariant(),
               const QVariant& msg6 = QVariant(), const QVariant& msg7 = QVariant(),
               const QVariant& msg8 = QVariant(), const QVariant& msg9 = QVariant());

    /*******************************************************************************
    Logging Functions in QList<QVariant> form.
    *******************************************************************************/
    void info(const QList<QVariant>& args);
    void trace(const QList<QVariant>& args);
    void warning(const QList<QVariant>& args);
    void error(const QList<QVariant>& args);
    void debug(const QList<QVariant>& args);
    void critical(const QList<QVariant>& args);
    void fatal(const QList<QVariant>& args);
    void write(const QList<QVariant>& args);

    /*******************************************************************************
    And now a generic Logging function
    *******************************************************************************/
    void log(LogLevel level, const QList<QVariant>& args);

Q_SIGNALS:
    void loggerEngineAdded(const QString& engineName);
    void loggerEngineRemoved(const QString& engineName);
    void loggerEngineEnabled(const QString& engineName);
    void loggerEngineDisabled(const QString& engineName);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QxtLogger::LogLevels)
Q_DECLARE_METATYPE(QxtLogger::LogLevel)
Q_DECLARE_METATYPE(QxtLogger::LogLevels)

#define qxtLog QxtLogger::getInstance()

#include "qxtlogstream.h"

#endif // QXTLOGGER_H
