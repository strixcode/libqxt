
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

#ifndef QXTRPCSERVICE_H
#define QXTRPCSERVICE_H

#include <QObject>
#include <QList>
#include <QVariant>
#include <QPair>
#include <QString>
#include <qxtglobal.h>

QT_FORWARD_DECLARE_CLASS(QIODevice)
class QxtAbstractConnectionManager;
class QxtAbstractSignalSerializer;

class QxtRPCServicePrivate;
class QXT_CORE_EXPORT QxtRPCService : public QObject
{
Q_OBJECT
public:
    QxtRPCService(QObject* parent = 0);
    explicit QxtRPCService(QIODevice* device, QObject* parent = 0);
    virtual ~QxtRPCService();

    bool isServer() const;
    bool isClient() const;

    QList<quint64> clients() const;
    QIODevice* device() const;
    void setDevice(QIODevice* dev);
    QIODevice* takeDevice();

    QxtAbstractSignalSerializer* serializer() const;
    void setSerializer(QxtAbstractSignalSerializer* serializer);

    QxtAbstractConnectionManager* connectionManager() const;
    void setConnectionManager(QxtAbstractConnectionManager* manager);

    bool attachSignal(QObject* sender, const char* signal, const QString& rpcFunction = QString());
    bool attachSlot(const QString& rpcFunction, QObject* recv, const char* slot,
            Qt::ConnectionType type = Qt::AutoConnection);
    void detachSignals(QObject* obj);
    void detachSlots(QObject* obj);
    void detachObject(QObject* obj);

public Q_SLOTS:
    void disconnectClient(quint64 id);
    void disconnectServer();
    void disconnectAll();

    void call(QString fn, const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(),
              const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(), const QVariant& p5 = QVariant(),
              const QVariant& p6 = QVariant(), const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant());
    void call(QList<quint64> ids, QString fn, const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(),
              const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(), const QVariant& p5 = QVariant(),
              const QVariant& p6 = QVariant(), const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant());
    void call(quint64 id, QString fn, const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(),
              const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(), const QVariant& p5 = QVariant(),
              const QVariant& p6 = QVariant(), const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant());
    void callExcept(quint64 id, QString fn, const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(),
                    const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(), const QVariant& p5 = QVariant(),
                    const QVariant& p6 = QVariant(), const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant());

    void detachSender();

Q_SIGNALS:
    /*!
     * This signal is emitted after a successful connection from a client.
     *
     * The given ID is used for disconnectClient(), call(), and related functions.
     */
    void clientConnected(quint64 id);

    /*!
     * This signal is emitted when a client disconnects. The given ID is no longer valid.
     */
    void clientDisconnected(quint64 id);

private:
    QXT_DECLARE_PRIVATE(QxtRPCService)
};
#endif
