#include "qxtstdio_p.h"
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

#include <cstdio>

/*!
\class QxtStdio

\inmodule QxtCore

\brief The QxtStdio class provides QIODevice and QxtPipe support for stdin and stdout

including readyRead() signal.
perfect as a counter part for QProcess or debug output into a QxtPipe chain


\sa QIODevice
\sa QxtPipe


\bold {Note:} when using this class, the buffers for stdin/stdout will be disabled, and NOT reenabled on destruction

*/




/*!
    Constructs a new QxtStdio with \a parent.
 */
QxtStdio::QxtStdio(QObject * parent): QxtPipe(parent)
{
    QXT_INIT_PRIVATE(QxtStdio);

    setvbuf(stdin , NULL , _IONBF , 0);
    setvbuf(stdout , NULL , _IONBF , 0);

    setOpenMode(QIODevice::ReadWrite);
    qxt_d().notify = new QSocketNotifier(

#ifdef Q_CC_MSVC
        _fileno(stdin)
#else
        fileno(stdin)
#endif

        , QSocketNotifier::Read, this);
    QObject::connect(qxt_d().notify, SIGNAL(activated(int)), &qxt_d(), SLOT(activated(int)));
}

/*!
    \reimp
 */
qint64 QxtStdio::writeData(const char * data, qint64 maxSize)
{
    qint64 i = 0;
    for (;i < maxSize;i++)
    {
        char c = *data++;
        putchar(c);
    }
//  emit(bytesWritten (i)); ///FIXME: according to the docs this may not be recoursive. how do i prevent that?
    return i;
}



void QxtStdioPrivate::activated(int)
{
    char c = getchar();
    if (c == EOF)
    {
#if QT_VERSION >= 0x040400
        emit qxt_p().readChannelFinished();
#endif
        hadeof = true;
        return;
    }
    QByteArray b(1, c);
    qxt_p().enqueData(b);
    qxt_p().sendData(b);
}

/*!
    Receive \a data.
 */
void   QxtStdio::receiveData(QByteArray data, const QxtPipe *)
{
    writeData(data.data(), data.size());
}

/*!
    \reimp
 */
bool QxtStdio::waitForReadyRead(int)
{
    if (qxt_d().hadeof)
        return false;


    char c = getchar();
    if (c == EOF)
    {
#if QT_VERSION >= 0x040400
        emit readChannelFinished();
#endif
        qxt_d().hadeof = true;
        return false;
    }
    QByteArray b(1, c);
    enqueData(b);
    sendData(b);
    return true;
}

/*!Blocks until EOF is received.*/
void QxtStdio::waitForEOF()
{
    if (qxt_d().hadeof)
        return;

    forever
    {
        char c = getchar();
        if (c == EOF)
        {
#if QT_VERSION >= 0x040400
            emit readChannelFinished();
#endif
            qxt_d().hadeof = true;
            return;
        }
        QByteArray b(1, c);
        enqueData(b);
        sendData(b);
    }
}


