
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

#include "qxtxmlfileloggerengine.h"
#include <QTime>

/*!
    \class QxtXmlFileLoggerEngine
    \brief The QxtXmlFileLoggerEngine class provides an XML file logger engine.
    \inmodule QxtCore

    Example XML log output:
    \code
    <?xml version="1.0" encoding="UTF-8"?>
    <log>
        <entry type="Error" time="22:38:33.159159">
            <message>Unknown error</message>
        </entry>
        <entry type="Debug" time="22:51:43.488488">
            <message>What's going on?</message>
            <message>Hi there</message>
        </entry>
    </log>
    \endcode

    \sa QxtLogger
 */

class QxtXmlFileLoggerEnginePrivate : public QxtPrivate<QxtXmlFileLoggerEngine>
{
    QXT_DECLARE_PUBLIC(QxtXmlFileLoggerEngine)

public:
    QxtXmlFileLoggerEnginePrivate();
    QString tab;
};

QxtXmlFileLoggerEnginePrivate::QxtXmlFileLoggerEnginePrivate()
        : tab("    ")
{
}

/*!
    Constructs an XML file logger engine with \a fileName.
*/
QxtXmlFileLoggerEngine::QxtXmlFileLoggerEngine(const QString& fileName)
        : QxtAbstractFileLoggerEngine(fileName, QIODevice::ReadWrite | QIODevice::Unbuffered)
{
    QXT_INIT_PRIVATE(QxtXmlFileLoggerEngine);
}

/*!
    \reimp
 */
void QxtXmlFileLoggerEngine::initLoggerEngine()
{
    QxtAbstractFileLoggerEngine::initLoggerEngine();

    // Mkay, we have an open file.  We need to check that it's all valid.
    // at the end of this block of code, we either can't log, or the carat is ready for writing.
    /*
    <?xml version="1.0" encoding="UTF-8"?>
    <log>
        <entry type="Error" time="sometime">
            <message>What's going on?</message>
            <message?Hi there</message>
        </entry>
    </log>
    */
    QIODevice* file = device();
    Q_ASSERT(file);
    if (file->size() == 0)
    {
        file->write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        file->write("<log>\n");
        file->write("</log>");
    }
    else
    {
        QByteArray data = file->read(64);
        if (!data.startsWith(QByteArray("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<log>")))
        {
            QFile* ptr_fileTarget = static_cast<QFile*>(file);
            qxtLog->warning(QString(" is not a valid XML log file.").prepend(ptr_fileTarget->fileName()));
            killLoggerEngine();
            return;
        }
    }
}

/*!
    \reimp
 */
void QxtXmlFileLoggerEngine::writeToFile(const QString &level, const QVariantList &messages)
{
    QIODevice* ptr_fileTarget = device();
    Q_ASSERT(ptr_fileTarget);
    ptr_fileTarget->seek(ptr_fileTarget->size() - 6);
    ptr_fileTarget->write(qxt_d().tab.toUtf8());
    ptr_fileTarget->write("<entry type=\"");
    ptr_fileTarget->write(level.toUtf8());
    ptr_fileTarget->write("\" time=\"");
    ptr_fileTarget->write(QTime::currentTime().toString("hh:mm:ss.zzzz").toUtf8());
    ptr_fileTarget->write("\">");
    ptr_fileTarget->write("\n");
    Q_FOREACH(const QVariant& m, messages)
    {
        ptr_fileTarget->write(qxt_d().tab.toUtf8());
        ptr_fileTarget->write(qxt_d().tab.toUtf8());
        ptr_fileTarget->write("<message>");
        ptr_fileTarget->write(toXmlSafeString(m.toString()).toUtf8());
        ptr_fileTarget->write("</message>\n");
    }

    ptr_fileTarget->write(qxt_d().tab.toUtf8());
    ptr_fileTarget->write("</entry>");
    ptr_fileTarget->write("\n");
    ptr_fileTarget->write("</log>");
}

/*!
    Replaces reserved characters from \a raw with corresponding entities.
 */
QString QxtXmlFileLoggerEngine::toXmlSafeString(const QString &raw)
{
    /* Reserved characters:
    <  &lt;
    &  &amp;
    > &lt;
    ' &apos;
    " &quot;

    Convert ampersands first, then the rest.
    */
    return QByteArray(raw.toUtf8()).replace('&', "&amp;").replace('<', "&lt;").replace('>', "&gt;").replace('\'', "&apos;").replace('"', "&quot;");
}
