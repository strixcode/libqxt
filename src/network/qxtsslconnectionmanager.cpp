
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

#include "qxtsslconnectionmanager.h"
#include "qxttcpconnectionmanager_p.h"
#include <QSslKey>

/*!
 * \class QxtSslConnectionManager
 * \inmodule QxtNetwork
 * \brief The QxtSslConnectionManager class accepts SSL connections and maintains a connection pool
 *
 * QxtSslConnectionManager is a standardized interface for accepting and tracking
 * incoming SSL connections.
 *
 * Each incoming connection is assigned an arbitrary, opaque client ID number. This
 * number can be used to retrieve the QSslSocket associated with it. A list of IDs
 * for all current connections can be retrieved with the clients() function.
 *
 * Like QxtSslServer, QxtSslConnectionManager can listen for incoming connections on
 * a specified interface and port. Unlike QTcpServer, but like QxtSslServer,
 * overriding incomingConnection() is not recommended.
 *
 * \sa QxtSslServer, QxtSslConnectionManager
 */

#ifndef QT_NO_OPENSSL

/*!
 * Constructs a new QxtSslConnectionManager object with the specified \a parent.
 */
QxtSslConnectionManager::QxtSslConnectionManager(QObject* parent) : QxtTcpConnectionManager(parent)
{
    // no QXT_INIT_PRIVATE here because we're borrowing QxtTcpConnectionManager's
    setAutoEncrypt(true);
}

void QxtSslConnectionManager::setLocalCertificate(const QSslCertificate& cert)
{
    qxt_d().setLocalCertificate(cert);
}

void QxtSslConnectionManager::setLocalCertificate(const QString& path, QSsl::EncodingFormat format)
{
    qxt_d().setLocalCertificate(path, format);
}

QSslCertificate QxtSslConnectionManager::localCertificate() const
{
    return qxt_d().localCertificate();
}

void QxtSslConnectionManager::setPrivateKey(const QSslKey& key)
{
    qxt_d().setPrivateKey(key);
}

void QxtSslConnectionManager::setPrivateKey(const QString& path, QSsl::KeyAlgorithm algo, QSsl::EncodingFormat format, const QByteArray& passPhrase)
{
    qxt_d().setPrivateKey(path, algo, format, passPhrase);
}

QSslKey QxtSslConnectionManager::privateKey() const
{
    return qxt_d().privateKey();
}

void QxtSslConnectionManager::setAutoEncrypt(bool on)
{
    qxt_d().setAutoEncrypt(on);
}

bool QxtSslConnectionManager::autoEncrypt() const
{
    return qxt_d().autoEncrypt();
}

/*!
 * Sets the protocol used when \a autoEncrypt is enabled.
 *
 * \sa protocol
 */
void QxtSslConnectionManager::setProtocol(QSsl::SslProtocol proto)
{
    qxt_d().setProtocol(proto);
}

/*!
 * Returns the protocol used when \a autoEncrypt is enabled.
 * \sa setProtocol
 */
QSsl::SslProtocol QxtSslConnectionManager::protocol() const
{
    return qxt_d().protocol();
}

#if QT_VERSION >= 0x050000
QIODevice* QxtSslConnectionManager::incomingConnection(qintptr socketDescriptor)
#else
QIODevice* QxtSslConnectionManager::incomingConnection(int socketDescriptor)
#endif
{
    QSslSocket* socket = new QSslSocket(this);
    if(socket->setSocketDescriptor(socketDescriptor)) {
        socket->setLocalCertificate(qxt_d().localCertificate());
        socket->setPrivateKey(qxt_d().privateKey());
        if(qxt_d().autoEncrypt()) socket->startServerEncryption();
        return socket;
    } else {
        delete socket;
        return 0;
    }
}

#endif
