
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

#ifndef QXTABSTRACTCONNECTIONMANAGER_H
#define QXTABSTRACTCONNECTIONMANAGER_H

#include <QObject>
#include <qxtglobal.h>

QT_FORWARD_DECLARE_CLASS(QIODevice)

class QxtAbstractConnectionManagerPrivate;
class QXT_CORE_EXPORT QxtAbstractConnectionManager : public QObject
{
    Q_OBJECT
public:
    QxtAbstractConnectionManager(QObject* parent);
    virtual ~QxtAbstractConnectionManager();

    int clientCount() const;
    QList<quint64> clients() const;
    QIODevice* client(quint64 clientID) const;

    /*!
     * Returns true if the connection manager is currently accepting connections.
     * Returns false otherwise.
     */
    virtual bool isAcceptingConnections() const = 0;

Q_SIGNALS:
    /*!
     * Indicates that the specified device, with the specified client ID, is ready for use.
     */
    void newConnection(QIODevice* device, quint64 clientID);

    /*!
     * Indicates that the device with the specified client ID has been disconnected.
     */
    void disconnected(QIODevice* device, quint64 clientID);

public Q_SLOTS:
    void disconnect(quint64 clientID);

protected:
    void addConnection(QIODevice* device, quint64 clientID);

    /*!
     * Stops managing a connection. This function is invoked by disconnect().
     * Implementations should perform whatever steps are necessary to close the connection
     * and clean up any internal data structures, including deleting the QIODevice object.
     */
    virtual void removeConnection(QIODevice* device, quint64 clientID) = 0;

private:
    QXT_DECLARE_PRIVATE(QxtAbstractConnectionManager)
};

#endif
