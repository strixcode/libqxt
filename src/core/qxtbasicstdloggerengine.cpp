
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

#include "qxtbasicstdloggerengine.h"
#include <QTextStream>
#include <QTime>

#define QXT_REQUIRED_LEVELS (QxtLogger::WarningLevel | QxtLogger::ErrorLevel | QxtLogger::CriticalLevel | QxtLogger::FatalLevel)

/*!
    \class QxtBasicSTDLoggerEngine
    \brief The QxtBasicSTDLoggerEngine class provides a basic STD logger engine.
    \inmodule QxtCore

    Example basic STD log output:
    \code
    [22:38:33.159] [Error] Unknown error
    [22:51:43.488] [Debug] What's going on?
                           Hi there!
    \endcode

    \sa QxtLogger
 */

class QxtBasicSTDLoggerEnginePrivate : public QxtPrivate<QxtBasicSTDLoggerEngine>
{
    QXT_DECLARE_PUBLIC(QxtBasicSTDLoggerEngine)

public:
    QxtBasicSTDLoggerEnginePrivate();

    QTextStream *errstream, *outstream;
};

QxtBasicSTDLoggerEnginePrivate::QxtBasicSTDLoggerEnginePrivate()
{
    errstream = new QTextStream(stderr);
    outstream = new QTextStream(stdout);
}

/*!
    Constructor.
 */
QxtBasicSTDLoggerEngine::QxtBasicSTDLoggerEngine()
{
    QXT_INIT_PRIVATE(QxtBasicSTDLoggerEngine);
#ifndef QT_NO_DEBUG
    setLogLevelsEnabled(QXT_REQUIRED_LEVELS);
#else
    setLogLevelsEnabled(QXT_REQUIRED_LEVELS | QxtLogger::DebugLevel);
#endif
    enableLogging();
}

/*!
    Destructor.
 */
QxtBasicSTDLoggerEngine::~QxtBasicSTDLoggerEngine()
{
    if (qxt_d().errstream)
    {
        qxt_d().errstream->flush();
        delete qxt_d().errstream;
        qxt_d().errstream = 0;
    }
    if (qxt_d().outstream)
    {
        qxt_d().outstream->flush();
        delete qxt_d().outstream;
        qxt_d().errstream = 0;
    }
}

/*!
    \reimp
 */
void QxtBasicSTDLoggerEngine::initLoggerEngine()
{
    return; // Should work out of the box!
}

/*!
    \reimp
 */
void QxtBasicSTDLoggerEngine::killLoggerEngine()
{
    return; // I do nothing.
}

/*!
    \reimp
 */
bool QxtBasicSTDLoggerEngine::isInitialized() const
{
    return qxt_d().errstream && qxt_d().outstream;
}

/*!
    Returns the stderr stream.
 */
QTextStream* QxtBasicSTDLoggerEngine::stdErrStream() const
{
    return qxt_d().errstream;
}

/*!
    Returns the stdout stream.
 */
QTextStream* QxtBasicSTDLoggerEngine::stdOutStream() const
{
    return qxt_d().outstream;
}

/*!
    Enables \a level if \a enable is \c true, disables otherwise.
 */
void QxtBasicSTDLoggerEngine::setLogLevelEnabled(QxtLogger::LogLevels level, bool enable)
{
    QxtLoggerEngine::setLogLevelsEnabled(level | QXT_REQUIRED_LEVELS, enable);
    if (!enable) QxtLoggerEngine::setLogLevelsEnabled(QXT_REQUIRED_LEVELS);
}

/*!
    \reimp
 */
void QxtBasicSTDLoggerEngine::writeFormatted(QxtLogger::LogLevel level, const QList<QVariant> &msgs)
{
    switch (level)
    {
    case QxtLogger::ErrorLevel:
        writeToStdErr("Error", msgs);
        break;
    case QxtLogger::WarningLevel:
        writeToStdOut("Warning", msgs);
        break;
    case QxtLogger::CriticalLevel:
        writeToStdErr("Critical", msgs);
        break;
    case QxtLogger::FatalLevel:
        writeToStdErr("!!FATAL!!", msgs);
        break;
    case QxtLogger::TraceLevel:
        writeToStdOut("Trace", msgs);
        break;
    case QxtLogger::DebugLevel:
        writeToStdErr("DEBUG", msgs);
        break;
    case QxtLogger::InfoLevel:
        writeToStdOut("INFO", msgs);
        break;
    default:
        writeToStdOut("", msgs);
        break;
    }
}

/*!
    A helper function that actually writes \a msgs to stderr with given \a level.

    This function is called by QxtBasicSTDLoggerEngine. Reimplement this function when creating a subclass of QxtBasicSTDLoggerEngine.
 */
void QxtBasicSTDLoggerEngine::writeToStdErr(const QString &level, const QList<QVariant> &msgs)
{
    if (msgs.isEmpty()) return;
    QString header = '[' + QTime::currentTime().toString("hh:mm:ss.zzz") + "] [" + level + "] ";
    QString padding;
    QTextStream* errstream = stdErrStream();
    Q_ASSERT(errstream);
    *errstream << header;
    for (int i = 0; i < header.size(); i++) padding.append(" ");
    int count = 0;
    Q_FOREACH(const QVariant& out, msgs)
    {
        if (!out.isNull())
        {
            if (count != 0) *errstream << padding;
            *errstream << out.toString() << '\n';
        }
        count++;
    }
    *errstream << endl;
}

/*!
    A helper function that actually writes \a msgs to stdout with given \a level.

    This function is called by QxtBasicSTDLoggerEngine. Reimplement this function when creating a subclass of QxtBasicSTDLoggerEngine.
 */
void QxtBasicSTDLoggerEngine::writeToStdOut(const QString& level, const QList<QVariant> &msgs)
{
    /* Message format...
        [time] [error level] First message.....
                    second message
                    third message
    */
    if (msgs.isEmpty()) return;
    QString header = '[' + QTime::currentTime().toString("hh:mm:ss.zzz") + "] [" + level + "] ";
    QString padding;
    QTextStream* outstream = stdOutStream();
    Q_ASSERT(outstream);
    *outstream << header;
    for (int i = 0; i < header.size(); i++) padding.append(' ');
    int count = 0;
    Q_FOREACH(const QVariant& out, msgs)
    {
        if (!out.isNull())
        {
            if (count != 0) *outstream << padding;
            *outstream << out.toString() << '\n';
        }
        count++;
    }
    *outstream << endl;
}
