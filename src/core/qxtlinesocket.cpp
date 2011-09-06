
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

#include "qxtlinesocket_p.h"
#include <QIODevice>

/*!
    \class QxtLineSocket

    \inmodule QxtCore

    \brief The QxtLineSocket class acts on a QIODevice as baseclass for line-based protocols
*/

/*!
    \fn QxtLineSocket::newLineReceived(const QByteArray& line)

    This signal is emitted whenever a new \a line is received.
 */

/*!
    Constructs a new QxtLineSocket with \a parent.
 */
QxtLineSocket::QxtLineSocket(QObject* parent) : QObject(parent)
{
    QXT_INIT_PRIVATE(QxtLineSocket);
}

/*!
    Constructs a new QxtLineSocket with \a socket and \a parent.
 */
QxtLineSocket::QxtLineSocket(QIODevice* socket, QObject* parent) : QObject(parent)
{
    QXT_INIT_PRIVATE(QxtLineSocket);
    setSocket(socket);
}

/*!
    Sets the \a socket.
 */
void QxtLineSocket::setSocket(QIODevice* socket)
{
    if (qxt_d().socket)
        disconnect(qxt_d().socket, SIGNAL(readyRead()), &qxt_d(), SLOT(readyRead()));
    qxt_d().socket = socket;
    if (qxt_d().socket)
        connect(qxt_d().socket, SIGNAL(readyRead()), &qxt_d(), SLOT(readyRead()));
}

/*!
    Returns the socket.
 */
QIODevice* QxtLineSocket::socket() const
{
    return qxt_d().socket;
}

/*!
    Sends a \a line.
 */
void QxtLineSocket::sendLine(const QByteArray& line)
{
    QByteArray copy(line);
    copy.replace(QByteArray("\n"), ""); //krazy:exclude=doublequote_chars
    qxt_d().socket->write(copy + '\n');
}

/*!
    This virtual function is called by QxtLineSocket whenever a \a line was received.
    Reimplement this function when creating a subclass of QxtLineSocket.

    \note The default implementation does nothing.
 */
void QxtLineSocket::newLine(const QByteArray& line)
{
    Q_UNUSED(line);
}

void QxtLineSocketPrivate::readyRead()
{
    buffer += socket->readAll();

    int i = 0;
    while ((i = buffer.indexOf('\n')) > -1)
    {
        QByteArray line = buffer.left(i);
        emit qxt_p().newLineReceived(line);
        qxt_p().newLine(line);
        buffer = buffer.mid(i + 1);
    }
}
