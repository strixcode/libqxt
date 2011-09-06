
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

#include "qxtrpcpeer.h"
#include "qxttcpconnectionmanager.h"
#include <QTcpSocket>

class QxtRPCPeerPrivate : public QxtPrivate<QxtRPCPeer>
{
public:
    QXT_DECLARE_PUBLIC(QxtRPCPeer)

    QTcpSocket* getTcpSocket()
    {
        QxtRPCPeer* p = &qxt_p();
        QTcpSocket* socket = new QTcpSocket(p);
        QObject::connect(socket, SIGNAL(connected()), p, SIGNAL(connectedToServer()));
        QObject::connect(socket, SIGNAL(disconnected()), p, SIGNAL(disconnectedFromServer()));
        QObject::connect(socket, SIGNAL(disconnected()), p, SLOT(disconnectServer()));
        QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), p, SIGNAL(serverError(QAbstractSocket::SocketError)));
        QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), p, SLOT(disconnectServer()));
        p->setDevice(socket);
        return socket;
    }
};

/*!
 * \class QxtRPCPeer
 * \inmodule QxtNetwork
 * \brief The QxtRPCPeer class transmits Qt signals over a network connection
 *
 * QxtRPCPeer is a tool that encapsulates Qt signals and transmits them over a network connection.
 * The signal is subsequently re-emitted on the receiving end of the connection.
 *
 * As a QxtRPCService subclass, QxtRPCPeer can act as a client or a server. When acting as a server,
 * it uses a QxtTcpConnectionManager to accept connections. When acting as a client, applications
 * may use connect() to establish a TCP connection with a server.
 *
 * All data types used in attached signals and slots must be declared and registered with QMetaType using
 * Q_DECLARE_METATYPE and qRegisterMetaType. Additional requirements may be imposed by the
 * QxtAbstractSignalSerializer subclass in use; the default QxtDataStreamSignalSerializer requires
 * that they have stream operators registered with qRegisterMetaTypeStreamOperators.
 *
 * Due to a restriction of Qt's signals and slots mechanism, the number of parameters that can be passed
 * to call() and its related functions, as well as the number of parameters to any signal or slot attached
 * to QxtRPCPeer, is limited to 8.
 */

/*!
 * Creates a QxtRPCPeer object with the given \a parent.
 */
QxtRPCPeer::QxtRPCPeer(QObject* parent) : QxtRPCService(parent)
{
    QXT_INIT_PRIVATE(QxtRPCPeer);
    setConnectionManager(new QxtTcpConnectionManager(this));
}

/*!
 * Connects to the specified server \a addr on the selected \a port.
 *
 * When the connection is complete, the connectedToServer() signal will be emitted.  If an error occurs, the serverError() signal will be emitted.
 */
void QxtRPCPeer::connect(QHostAddress addr, int port)
{
    qxt_d().getTcpSocket()->connectToHost(addr, port);
}

/*!
 * Connects to the specified peer or server \a addr on the selected \a port.
 *
 * When the connection is complete, the connectedToServer() signal will be emitted.  If an error occurs, the serverError() signal will be emitted.
 */
void QxtRPCPeer::connect(QString addr, int port)
{
    qxt_d().getTcpSocket()->connectToHost(addr, port);
}

/*!
 * Listens on the specified interface \a iface on the specified \a port for connections.
 *
 * Attempting to listen while in Client mode or while connected in Peer mode will be ignored with a warning.  In Peer mode, only one connection
 * can be active at a time. Additional incoming connections while connected to a peer will be dropped. When a peer connects, the peerConnected()
 * signal will be emitted. In Server mode, multiple connections can be active at a time. Each client that connects will be provided a unique ID,
 * included in the clientConnected() signal that will be emitted. Returns \c true if succeeds, \c false otherwise.
 */
bool QxtRPCPeer::listen(QHostAddress iface, int port)
{
    QxtTcpConnectionManager* mgr = qobject_cast<QxtTcpConnectionManager*>(connectionManager());
    if (!mgr)
    {
        qWarning() << "QxtRPCPeer::listen: cannot listen with non-QxtTcpConnectionManager";
        return false;
    }
    return mgr->listen(iface, port);
}

/*!
 * Stops listening for connections. Any connections still open will remain connected.
 */
void QxtRPCPeer::stopListening()
{
    QxtTcpConnectionManager* mgr = qobject_cast<QxtTcpConnectionManager*>(connectionManager());
    if (!mgr)
    {
        qWarning() << "QxtRPCPeer::listen: cannot stop listening with non-QxtTcpConnectionManager";
    }
    else
    {
        mgr->stopListening();
    }
}

