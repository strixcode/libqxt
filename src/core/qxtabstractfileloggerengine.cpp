
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

#include "qxtabstractfileloggerengine.h"

/*!
    \class QxtAbstractFileLoggerEngine
    \brief The QxtAbstractFileLoggerEngine class is the base class of file logger engines.
    \inmodule QxtCore

    \sa QxtLogger
 */

class QxtAbstractFileLoggerEnginePrivate : public QxtPrivate<QxtAbstractFileLoggerEngine>
{
    QXT_DECLARE_PUBLIC(QxtAbstractFileLoggerEngine)

public:
    QString logFile;
    QIODevice::OpenMode mode;
};

/*!
    Constructs a QxtAbstractFileLoggerEngine with \a fileName and open \a mode.
 */
QxtAbstractFileLoggerEngine::QxtAbstractFileLoggerEngine(const QString &fileName, QIODevice::OpenMode mode)
        : QxtAbstractIOLoggerEngine(0)
{
    QXT_INIT_PRIVATE(QxtAbstractFileLoggerEngine);
    qxt_d().mode = mode;
    setLogFileName(fileName);
}

/*!
    Destructs the file logger engine.
 */
QxtAbstractFileLoggerEngine::~QxtAbstractFileLoggerEngine()
{
    killLoggerEngine();
}

/*!
    \reimp
 */
void QxtAbstractFileLoggerEngine::initLoggerEngine()
{
    // Are we already logging to a file?  If so, close it and disable logging.
    killLoggerEngine();

    // If the file exists, check if we can write to it.  If we can, we append!
    // If the file doesn't exits, try to create it.
    // If we can't write to a file, disable this plugin.
    if (qxt_d().logFile.isEmpty()) return;  // if there's no filename, disable the engine until one is given

    setDevice(new QFile(qxt_d().logFile));
    if (!device()->open(qxt_d().mode)
            || !device()->isWritable())
    {
        killLoggerEngine();
        return;
    }

    enableLogging();
}

/*!
    \reimp
 */
void QxtAbstractFileLoggerEngine::killLoggerEngine()
{
    if (device() != 0)
    {
        if (device()->isOpen()) device()->close();
        delete device();
        setDevice(0);
    }
}

/*!
    \reimp
 */
bool QxtAbstractFileLoggerEngine::isInitialized() const
{
    return (device() != 0);
}

/*!
    \reimp
 */
void QxtAbstractFileLoggerEngine::writeFormatted(QxtLogger::LogLevel level, const QList<QVariant> &messages)
{
    switch (level)
    {
    case QxtLogger::ErrorLevel:
        writeToFile("Error", messages);
        break;
    case QxtLogger::WarningLevel:
        writeToFile("Warning", messages);
        break;
    case QxtLogger::CriticalLevel:
        writeToFile("Critical", messages);
        break;
    case QxtLogger::FatalLevel:
        writeToFile("Fatal", messages);
        break;
    case QxtLogger::TraceLevel:
        writeToFile("Trace", messages);
        break;
    case QxtLogger::DebugLevel:
        writeToFile("Debug", messages);
        break;
    case QxtLogger::InfoLevel:
        writeToFile("Info", messages);
        break;
    default:
        writeToFile(QString(), messages);
        break;
    }
}

/*!
    Sets the log \a fileName.
 */
void QxtAbstractFileLoggerEngine::setLogFileName(const QString &fileName)
{
    qxt_d().logFile = fileName;
    initLoggerEngine();
}

/*!
    Returns the log file name.
 */
QString QxtAbstractFileLoggerEngine::logFileName() const
{
    return qxt_d().logFile;
}

/*!
    \fn virtual void QxtAbstractFileLoggerEngine::writeToFile( const QString &level, const QVariantList &messages ) = 0

    Writes \a messages to file with given \a level.

    This function is called by QxtAbstractFileLoggerEngine. Reimplement this function when creating a subclass of QxtAbstractFileLoggerEngine.
 */
