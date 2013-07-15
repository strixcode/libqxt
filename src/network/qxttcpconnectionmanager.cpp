
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

#include "qxttcpconnectionmanager.h"
#include "qxttcpconnectionmanager_p.h"
#include <QTcpSocket>
#include <QtDebug>

/*!
 * \class QxtTcpConnectionManager
 * \inmodule QxtNetwork
 * \brief The QxtTcpConnectionManager class accepts TCP connections and maintains a connection pool
 *
 * QxtTcpConnectionManager is a standardized interface for accepting and tracking
 * incoming TCP connections.
 *
 * Each incoming connection is assigned an arbitrary, opaque client ID number. This
 * number can be used to retrieve the QTcpSocket associated with it. A list of IDs
 * for all current connections can be retrieved with the clients() function.
 *
 * Like QTcpServer, QxtTcpConnectionManager can listen for incoming connections on
 * a specified interface and port, and like QTcpServer you may override the
 * incomingConnection() function to change the handling of new connections. This
 * is, for instance, where you could create a QSslSocket to encrypt communications
 * (but see QxtSslConnectionManager).
 *
 * \sa QTcpServer, QxtSslConnectionManager
 */

/*!
 * Constructs a new QxtTcpConnectionManager object with the specified \a parent.
 */
QxtTcpConnectionManager::QxtTcpConnectionManager(QObject* parent) : QxtAbstractConnectionManager(parent)
{
    QXT_INIT_PRIVATE(QxtTcpConnectionManager);
#ifndef QT_NO_OPENSSL
    qxt_d().setAutoEncrypt(false);
#endif
}

QxtTcpConnectionManagerPrivate::QxtTcpConnectionManagerPrivate()
#ifndef QT_NO_OPENSSL
: QxtSslServer(0)
#else
: QTcpServer(0)
#endif
{
    QObject::connect(&mapper, SIGNAL(mapped(QObject*)), this, SLOT(socketDisconnected(QObject*)));
}

#if QT_VERSION >= 0x050000
void QxtTcpConnectionManagerPrivate::incomingConnection(qintptr socketDescriptor)
#else
void QxtTcpConnectionManagerPrivate::incomingConnection(int socketDescriptor)
#endif
{
    QIODevice* device = qxt_p().incomingConnection(socketDescriptor);
    if (device)
    {
        qxt_p().addConnection(device, (quint64)static_cast<QObject*>(device));
        mapper.setMapping(device, device);
        QObject::connect(device, SIGNAL(destroyed()), &mapper, SLOT(map()));
        QTcpSocket* sock = qobject_cast<QTcpSocket*>(device);
        if (sock)
        {
            QObject::connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), &mapper, SLOT(map()));
            QObject::connect(sock, SIGNAL(disconnected()), &mapper, SLOT(map()));
        }
    }
}

/*!
 * Listens on the specified interface \a iface on the specified \a port for connections.
 * If \a iface is QHostAddress::Any, listens on all interfaces.
 *
 * Returns \c true on success; otherwise returns \c false.
 */
bool QxtTcpConnectionManager::listen(QHostAddress iface, int port)
{
    return qxt_d().listen(iface, port);
}

/*!
 * Stops listening for connections. Any connections still open will remain connected.
 */
void QxtTcpConnectionManager::stopListening()
{
    if (!qxt_d().isListening())
    {
        qWarning() << "QxtTcpConnectionManager: Not listening";
        return;
    }
    qxt_d().close();
}

/*!
 * \reimp
 */
bool QxtTcpConnectionManager::isAcceptingConnections() const
{
    return qxt_d().isListening();
}

/*!
 * This function is called when a new TCP connection becomes available. The parameter
 * is the native \a socketDescriptor for the connection, suitable for use in
 * QTcpSocket::setSocketDescriptor.
 *
 * The default implementation returns a new QTcpSocket with the specified descriptor.
 * Subclasses may return QTcpSocket subclasses, such as QSslSocket.
 */
#if QT_VERSION >= 0x050000
QIODevice* QxtTcpConnectionManager::incomingConnection(qintptr socketDescriptor)
#else
QIODevice* QxtTcpConnectionManager::incomingConnection(int socketDescriptor)
#endif
{
    QTcpSocket* device = new QTcpSocket(this);
    device->setSocketDescriptor(socketDescriptor);
    return device;
}

/*!
 * \reimp
 */
void QxtTcpConnectionManager::removeConnection(QIODevice* device, quint64 clientID)
{
    Q_UNUSED(clientID);
    if (device)
    {
        QAbstractSocket* sock = qobject_cast<QAbstractSocket*>(device);
        if (sock) sock->disconnectFromHost();
        device->close();
        device->deleteLater();
    }
}

/*!
 * Sets an explicit network \a proxy for the connection manager.
 *
 * \sa QTcpServer::setProxy()
 */
void QxtTcpConnectionManager::setProxy(const QNetworkProxy& proxy)
{
    qxt_d().setProxy(proxy);
}

/*!
 * Returns the proxy in use for the connection manager.
 *
 * \sa QTcpServer::proxy()
 */
QNetworkProxy QxtTcpConnectionManager::proxy() const
{
    return qxt_d().proxy();
}

void QxtTcpConnectionManagerPrivate::socketDisconnected(QObject* client)
{
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(client);
    if (sock)
    {
        QObject::disconnect(sock, SIGNAL(error(QAbstractSocket::SocketError)), &mapper, SLOT(map()));
        QObject::disconnect(sock, SIGNAL(disconnected()), &mapper, SLOT(map()));
    }
    qxt_p().disconnect((quint64)(client));
}
