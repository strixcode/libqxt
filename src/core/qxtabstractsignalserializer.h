
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

#ifndef QXTABSTRACTSIGNALSERIALIZER_H
#define QXTABSTRACTSIGNALSERIALIZER_H

#include <qxtglobal.h>
#include <QString>
#include <QVariant>
#include <QList>
#include <QPair>
#include <QByteArray>

/*!
 * \class QxtAbstractSignalSerializer
 * \inmodule QxtCore
 * \brief The QxtAbstractSignalSerializer class provides an interface for classes that convert signals to a binary form.
 *
 * QxtAbstractSignalSerializer is an abstract interface used by, for example, QxtRPCService to convert signals into a
 * serialized binary format suitable for storing or transmitting over an I/O device.
 *
 * Qxt provides a default implementation in the form of QxtDataStreamSignalSerializer, which is generally sufficient for
 * most applications. Implement other subclasses of QxtAbstractSignalSerializer to allow QxtRPCService and similar tools
 * to integrate with existing protocols.
 */

class QXT_CORE_EXPORT QxtAbstractSignalSerializer
{
public:
    /*!
     * The first half of QxtAbstractSignalSerializer::DeserializedData is the name of the deserialized signal. The
     * second half is an array of QVariants containing the parameters of the signal.
     */
    typedef QPair<QString, QList<QVariant> > DeserializedData;

    /*!
     * Destroys the QxtAbstractSignalSerializer.
     */
    virtual ~QxtAbstractSignalSerializer() {}

    /*!
     * Serializes a signal into a form suitable for sending to an I/O device.
     */
    virtual QByteArray serialize(const QString& fn, const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(),
                                 const QVariant& p3 = QVariant(), const QVariant& p4 = QVariant(),
                                 const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(),
                                 const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant()) const = 0;

    /*!
     * Deserializes binary data into a signal name and a list of parameters. When implementing this function, be sure
     * to remove the processed portion of the data from the reference parameter.
     * Return QxtAbstractSignalSerializer::NoOp() if the deserialized data does not invoke a signal.
     * Return QxtAbstractSignalSerializer::ProtocolError() if the protocol has been violated and the connection should
     * be severed.
     */
    virtual DeserializedData deserialize(QByteArray& data) = 0;

    /*!
     * Indicates whether the data currently in the buffer can be deserialized.
     */
    virtual bool canDeserialize(const QByteArray& buffer) const = 0;

    /*!
     * Returns an object that indicates that the deserialized data does not invoke a signal.
     */
    static inline DeserializedData NoOp()
    {
        static DeserializedData rv = qMakePair(QString(), QList<QVariant>());
        return rv;
    }

    /*!
     * Returns an object that indicates that the deserialized data indicates a fatal protocol error.
     */
    static inline DeserializedData ProtocolError()
    {
        static DeserializedData rv = qMakePair(QString(), QList<QVariant>() << QVariant());
        return rv;
    }

    /*!
     * Checks to see if the provided object does not invoke a signal.
     */
    static inline bool isNoOp(const DeserializedData& value)
    {
        return value.first.isEmpty() && value.second.isEmpty();
    }

    /*!
     * Checks to see if the provided object indicates a fatal protocol error.
     */
    static inline bool isProtocolError(const DeserializedData& value)
    {
        return value.first.isEmpty() && !value.second.isEmpty();
    }
};

#endif
