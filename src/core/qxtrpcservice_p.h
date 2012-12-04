
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

#ifndef QXTRPCSERVICE_P_H
#define QXTRPCSERVICE_P_H

#include "qxtrpcservice.h"
#include <QPointer>
#include <QHash>
#include <QByteArray>
#include <QString>
#include <QPair>

class QxtBoundFunction;

class QxtRPCServiceIntrospector;
class QxtRPCServicePrivate : public QObject, public QxtPrivate<QxtRPCService>
{
Q_OBJECT
public:
    QxtRPCServicePrivate();
    QXT_DECLARE_PUBLIC(QxtRPCService)

    QxtRPCServiceIntrospector* introspector;
    QxtAbstractConnectionManager* manager;
    QxtAbstractSignalSerializer* serializer;
    QPointer<QIODevice> device;

    // One buffer is needed for the "server" connection, and one buffer is needed for each connected client.
    QByteArray serverBuffer;
    QHash<quint64, QByteArray> buffers;

    // A Qt invokable, such as a signal or slot, can be identified by the metaobject containing its description plus
    // its signature or name. It is worth noting that QxtRPCService uses the same structure for both signals and slots,
    // but in slightly different ways: For identifying incoming signals, this structure contains the signature. For
    // identifying outgoing slots, this structure only contains the name, and the parameters coming from the signal are
    // used to determine which overload to use.
    // TODO: Is this actually safe?
    typedef QPair<const QMetaObject*, QByteArray> MetaMethodDef;

    // A slot connection can be identified by the object receiving it and the name of the function. Additionally, a
    // connection can be Direct, Queued, or BlockingQueued.
    struct SlotDef
    {
        QObject* recv;
        QByteArray slot;
        Qt::ConnectionType type;
        inline bool operator==(const SlotDef& other) const {
            // Two slots are equivalent only if they refer to the same slot on the same object with the same
            // connection type.
            return (recv == other.recv) && (slot == other.slot) && (type == other.type);
        }
    };

    // Maps an RPC function name to a list of slot connections.
    QHash<QString, QList<SlotDef> > connectedSlots;

    // Maps a slot's metamethod to an array of parameter type names.
    QHash<MetaMethodDef, QList<QByteArray> > slotParameters;

    // Maps the connected client id with bound parameters of the slot
    QHash<quint64, QxtBoundFunction*>  clientsDataArgument;

    // As described in the main class's documentation, QMetaObject::invokeMethod is limited to 10 parameters, so
    // QxtRPCService is limited to 8.
    void dispatchFromServer(const QString& fn, const QVariant& p0 = QVariant(), const QVariant& p1 = QVariant(),
                            const QVariant& p2 = QVariant(), const QVariant& p3 = QVariant(),
                            const QVariant& p4 = QVariant(), const QVariant& p5 = QVariant(),
                            const QVariant& p6 = QVariant(), const QVariant& p7 = QVariant()) const;
    void dispatchFromClient(quint64 id, const QString& fn, const QVariant& p0 = QVariant(), 
                            const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(),
                            const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(),
                            const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(),
                            const QVariant& p7 = QVariant()) const;

public Q_SLOTS:
    void clientConnected(QIODevice* dev, quint64 id);
    void clientDisconnected(QIODevice* dev, quint64 id);
    void clientData(quint64 id);

    void serverData();
};

#endif
