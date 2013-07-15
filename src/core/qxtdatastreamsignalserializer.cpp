
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

#include <qxtdatastreamsignalserializer.h>
#include <QIODevice>
#include <QDataStream>
#include <QtGlobal>
#include <QtDebug>
#include <qendian.h>

class QxtDataStreamSignalSerializerPrivate : public QxtPrivate<QxtDataStreamSignalSerializer>
{
public:
    QXT_DECLARE_PUBLIC(QxtDataStreamSignalSerializer)
    int dataStreamVersion;

    void applyVersion(QDataStream &str) const
    {
        if (dataStreamVersion != (int) QxtDataStreamSignalSerializer::DefaultDataStreamVersion) {
            str.setVersion((QDataStream::Version) dataStreamVersion);
        }
    }
};

QxtDataStreamSignalSerializer::QxtDataStreamSignalSerializer()
{
    QXT_INIT_PRIVATE(QxtDataStreamSignalSerializer);
    qxt_d().dataStreamVersion = DefaultDataStreamVersion;
}

QByteArray QxtDataStreamSignalSerializer::serialize(const QString& fn, const QVariant& p1, const QVariant& p2, const QVariant& p3,
        const QVariant& p4, const QVariant& p5, const QVariant& p6, const QVariant& p7, const QVariant& p8) const
{
    QByteArray rv;
    QDataStream str(&rv, QIODevice::WriteOnly);
    qxt_d().applyVersion(str);
    str << fn;
    unsigned char ct = 8;
    if (!p1.isValid()) ct = 0;
    else if (!p2.isValid()) ct = 1;
    else if (!p3.isValid()) ct = 2;
    else if (!p4.isValid()) ct = 3;
    else if (!p5.isValid()) ct = 4;
    else if (!p6.isValid()) ct = 5;
    else if (!p7.isValid()) ct = 6;
    else if (!p8.isValid()) ct = 7;
    str << ct;
    if (ct-- > 0) str << p1;
    if (ct-- > 0) str << p2;
    if (ct-- > 0) str << p3;
    if (ct-- > 0) str << p4;
    if (ct-- > 0) str << p5;
    if (ct-- > 0) str << p6;
    if (ct-- > 0) str << p7;
    if (ct-- > 0) str << p8;
    char sizeData[4];
    qToLittleEndian(quint32(rv.size()), (uchar*)sizeData);
    return QByteArray(sizeData, 4) + rv;
}

QxtAbstractSignalSerializer::DeserializedData QxtDataStreamSignalSerializer::deserialize(QByteArray& data)
{
    QByteArray cmd;
    quint32 len = qFromLittleEndian<quint32>(reinterpret_cast<const uchar*>(data.constData()));

    cmd = data.mid(4, len);
    data = data.mid(len + 4);
    if (cmd.length() == 0) return NoOp();

    QDataStream str(cmd);
    qxt_d().applyVersion(str);

    QString signal;
    unsigned char argCount;
    QList<QVariant> v;
    QVariant t;
    str >> signal >> argCount;

    if (str.status() == QDataStream::ReadCorruptData) return ProtocolError();

    for (int i = 0; i < argCount; i++)
    {
        str >> t;
        v << t;
    }
    return qMakePair(signal, v);
}

bool QxtDataStreamSignalSerializer::canDeserialize(const QByteArray& buffer) const
{
    if(buffer.length() < int(sizeof(quint32))) return false;
    quint32 headerLen = qFromLittleEndian<quint32>(reinterpret_cast<const uchar*>(buffer.constData()));
    quint32 bodyLen = quint32(buffer.length() - 4);
    return headerLen <= bodyLen;
}

int QxtDataStreamSignalSerializer::dataStreamVersion() const
{
    return qxt_d().dataStreamVersion;
}

void QxtDataStreamSignalSerializer::setDataStreamVersion(int version)
{
    qxt_d().dataStreamVersion = version;
}
