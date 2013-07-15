
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

#ifndef QXTDATASTREAMSIGNALSERIALIZER_H
#define QXTDATASTREAMSIGNALSERIALIZER_H

#include <qxtglobal.h>
#include <qxtabstractsignalserializer.h>

class QxtDataStreamSignalSerializerPrivate;

class QXT_CORE_EXPORT QxtDataStreamSignalSerializer : public QxtAbstractSignalSerializer
{
    QXT_DECLARE_PRIVATE(QxtDataStreamSignalSerializer)

public:

    QxtDataStreamSignalSerializer();

    /*!
     * Serializes a signal into a form suitable for sending to an I/O device.
     */
    virtual QByteArray serialize(const QString& fn, const QVariant& p1 = QVariant(), const QVariant& p2 = QVariant(), const QVariant& p3 = QVariant(),
                                 const QVariant& p4 = QVariant(), const QVariant& p5 = QVariant(), const QVariant& p6 = QVariant(),
                                 const QVariant& p7 = QVariant(), const QVariant& p8 = QVariant()) const;

    /*!
     * Deserializes binary data into a signal name and a list of parameters.
     */
    virtual DeserializedData deserialize(QByteArray& data);

    /*!
     * Indicates whether the data currently in the buffer can be deserialized.
     */
    virtual bool canDeserialize(const QByteArray& buffer) const;

    enum  {
        DefaultDataStreamVersion = 0
    };

    /*!
     * Get version of data stream to use for serialization/deserialization.
     * \return Either a QDataStream::Version or DefaultDataStreamVersion
     */
    int dataStreamVersion() const;

    /*!
     * Set version of data stream to use for serialization/deserialization.
     * This allows RPC between programs built with different versions of Qt.
     *
     * \param version Either a QDataStream::Version or DefaultDataStreamVersion
     */
    void setDataStreamVersion(int version);
};

#endif
