
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

#include "qxtbasicfileloggerengine.h"
#include <QDateTime>

/*!
    \class QxtBasicFileLoggerEngine
    \brief The QxtBasicFileLoggerEngine class provides a basic file logger engine.
    \inmodule QxtCore

    Example basic file log output:
    \code
    [22:38:33.159] [Error] Unknown error
    [22:51:43.488] [Debug] What's going on?
                           Hi there!
    \endcode

    \sa QxtLogger
 */

class QxtBasicFileLoggerEnginePrivate : public QxtPrivate<QxtBasicFileLoggerEngine>
{
public:
    QXT_DECLARE_PUBLIC(QxtBasicFileLoggerEngine)
    QString dateFormat;
};

/*!
    Constructs a basic file logger engine with \a fileName.
*/
QxtBasicFileLoggerEngine::QxtBasicFileLoggerEngine(const QString &fileName)
        : QxtAbstractFileLoggerEngine(fileName, QIODevice::ReadWrite | QIODevice::Append | QIODevice::Unbuffered)
{
    QXT_INIT_PRIVATE(QxtBasicFileLoggerEngine);
    qxt_d().dateFormat = "hh:mm:ss.zzz";
}

/*!
    Returns the date format in use by this logger engine.
    \sa QDateTime::toString()
 */
QString QxtBasicFileLoggerEngine::dateFormat() const
{
    return qxt_d().dateFormat;
}

/*!
    Sets the date \a format used by this logger engine.
    \sa QDateTime::toString()
 */
void QxtBasicFileLoggerEngine::setDateFormat(const QString& format)
{
    qxt_d().dateFormat = format;
}

/*!
    \reimp
 */
void QxtBasicFileLoggerEngine::writeToFile(const QString &level, const QVariantList &messages)
{
    if (messages.isEmpty()) return;
    QString header = '[' + QDateTime::currentDateTime().toString(qxt_d().dateFormat) + "] [" + level + "] ";
    QString padding;
    QIODevice* file = device();
    Q_ASSERT(file);
    file->write(header.toUtf8());
    for (int i = 0; i < header.size(); i++) padding.append(" ");
    int count = 0;
    Q_FOREACH(const QVariant& out, messages)
    {
        if (!out.isNull())
        {
            if (count != 0) file->write(padding.toUtf8());
            file->write(out.toString().toUtf8());
            file->write("\n");
        }
        count++;
    }
}
