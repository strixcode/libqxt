
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

#include "qxtloggerengine.h"

/*! \class QxtLoggerEngine
    \brief The QxtLoggerEngine class is the parent class of all extended Engine Plugins.
    \inmodule QxtCore

    \section1 Overview
    QxtLoggerEngine is what gives the QxtLogger it's power.  While the Logger itself
    manages memory, logic, and log levels, it is the various Engine Plugins that
    do the actual work of logging the data.
*/

class QxtLoggerEnginePrivate : public QxtPrivate<QxtLoggerEngine>
{
    QXT_DECLARE_PUBLIC(QxtLoggerEngine)

public:
    QxtLoggerEnginePrivate();

    QxtLogger::LogLevels    bm_logLevel;
    bool                    b_isLogging;
};

QxtLoggerEnginePrivate::QxtLoggerEnginePrivate()
        : bm_logLevel(QxtLogger::AllLevels), b_isLogging(true)
{
}

/*!
    Constructor
 */
QxtLoggerEngine::QxtLoggerEngine()
{
    QXT_INIT_PRIVATE(QxtLoggerEngine);
}

/*!
    Destructor
 */
QxtLoggerEngine::~QxtLoggerEngine()
{
    QxtLogger::getInstance()->removeLoggerEngine(this);
}

/*!
    \fn virtual void QxtLoggerEngine::initLoggerEngine() = 0

    Initializes the logger engine.

    This function is called by QxtLogger. Reimplement this function when creating a subclass of QxtLoggerEngine.
 */

/*!
    \fn virtual void QxtLoggerEngine::killLoggerEngine() = 0

    Kills the logger engine.

    This function is called by QxtLogger. Reimplement this function when creating a subclass of QxtLoggerEngine.
 */

/*!
    \fn virtual bool QxtLoggerEngine::isInitialized() const = 0

    Returns \c true if the logger engine is initialized.

    This function is called by QxtLogger. Reimplement this function when creating a subclass of QxtLoggerEngine.
 */

/*!
    \fn virtual void QxtLoggerEngine::writeFormatted(QxtLogger::LogLevel level, const QList<QVariant>& messages) = 0

    Writes formatted \a messages with given \a level.

    This function is called by QxtLogger. Reimplement this function when creating a subclass of QxtLoggerEngine.
 */

/*!
    Returns \c true if logging is enabled and \c false otherwise.
 */
bool QxtLoggerEngine::isLoggingEnabled() const
{
    return qxt_d().b_isLogging;
}

/*!
    Enables logging.
 */
void QxtLoggerEngine::enableLogging()
{
    setLoggingEnabled();
}

/*!
    Disables logging.
 */
void QxtLoggerEngine::disableLogging()
{
    setLoggingEnabled(false);
}

/*!
    Sets logging enabled if \a enable is \c true, or disabled otherwise.
 */
void QxtLoggerEngine::setLoggingEnabled(bool enable)
{
    qxt_d().b_isLogging = enable;
}

/*!
    Sets log \a levels enabled if \a enable is \c true, or disabled otherwise.
 */
void QxtLoggerEngine::setLogLevelsEnabled(QxtLogger::LogLevels levels, bool enable)
{
    if (enable)
    {
        qxt_d().bm_logLevel |= levels;
    }
    else
    {
        qxt_d().bm_logLevel &= ~levels;
    }
}

/*!
    Enables log \a levels.
 */
void QxtLoggerEngine::enableLogLevels(QxtLogger::LogLevels levels)
{
    setLogLevelsEnabled(levels, true);
}

/*!
    Disables log \a levels.
 */
void QxtLoggerEngine::disableLogLevels(QxtLogger::LogLevels levels)
{
    setLogLevelsEnabled(levels, false);
}

/*!
    Returns \c true if log \a level is enabled and \c false otherwise.
 */
bool QxtLoggerEngine::isLogLevelEnabled(QxtLogger::LogLevel level) const
{
    return (qxt_d().bm_logLevel & level);
}
