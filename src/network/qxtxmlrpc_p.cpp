
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

#include <QVariant>
#include <QVariantList>
#include <QStringList>
#include <QVariantMap>
#include <QDateTime>
#include "qxtxmlrpc_p.h"

using namespace QxtXmlRpc;


QString xmlEncode(QString a)
{
    return a.replace('&', "&amp;")
           .replace('<', "&lt;")
           .replace('>', "&gt;");
}


QString QxtXmlRpc::serialize(QVariant data)
{
    if (data.isNull())
    {
        return "<nil/>";
    }
    int t = data.type();
    if (t == QVariant::String)
    {
        return "<string>" + xmlEncode(data.toString()) + "</string>";
    }
    else if (t == QVariant::Bool)
    {
        return "<boolean>" + (data.toBool() ? QString("1") : QString("0")) + "</boolean>";
    }
    else if (t ==  QVariant::Int)
    {
        return "<int>" + QString::number(data.toInt()) + "</int>";
    }
    else if (t == QVariant::Double)
    {
        return "<double>" + QString::number(data.toDouble()) + "</double>";
    }
    else if (t == QVariant::DateTime)
    {
        return "<dateTime.iso8601>" + data.toDateTime().toString(Qt::ISODate) + "</dateTime.iso8601>";
    }
    else if (t == QVariant::ByteArray)
    {
        return "<base64>" + data.toByteArray().toBase64() + "</base64>";
    }
    else if (t == QVariant::Map)
    {
        QString ret = "<struct>";
        QMap<QString, QVariant> map = data.toMap();
        QMapIterator<QString, QVariant> i(map);
        while (i.hasNext())
        {
            i.next();
            ret += "<member><name>" + i.key() + "</name><value>" + serialize(i.value()) + "</value></member>";
        }
        ret += "</struct>";
        return ret;
    }
#if QT_VERSION >= 0x040500
    else if (t == QVariant::Hash)
    {
        QString ret = "<struct>";
        QHash<QString, QVariant> map = data.toHash();
        QHashIterator<QString, QVariant> i(map);
        while (i.hasNext())
        {
            i.next();
            ret += "<member><name>" + i.key() + "</name><value>" + serialize(i.value()) + "</value></member>";
        }
        ret += "</struct>";
        return ret;
    }
#endif
    else if (t == QVariant::StringList)
    {
        QString ret = "<array><data>";
        QStringList l = data.toStringList();
        foreach(QString i, l)
        {
            ret += "<value>" + xmlEncode(i) + "</value>";
        }
        ret += "</data></array>";
        return ret;
    }
    else if (t == QVariant::List)
    {
        QString ret = "<array><data>";
        QVariantList l = data.toList();
        foreach(QVariant i, l)
        {
            ret += "<value>" + serialize(i) + "</value>";
        }
        ret += "</data></array>";
        return ret;
    }
    else
    {
        return "";
    }
}







QVariant deserializeArray(QXmlStreamReader & xml)
{
    QVariantList l;
    int s = 0;
    while (!xml.atEnd())
    {
        xml.readNext();
        if (xml.isStartElement())
        {
            if (s == 0)
            {
                if (xml.name().toString() == "data")
                {
                    s = 1;
                }
                else
                {
                    xml.raiseError("expected <data>.   got : <" + xml.name().toString() + ">");
                }
            }
            else if (s == 1)
            {
                if (xml.name().toString() == "value")
                {
                    l += deserialize(xml);
                    s = 2;
                    if (xml.isEndElement() && xml.name().toString() == "value"){
                        s=1;
                    }
                }
                else
                {
                    xml.raiseError("expected <value>.   got : <" + xml.name().toString() + ">");
                }
            }
            else if (s == 2)
            {
                xml.raiseError("expected </value>.   got : <" + xml.name().toString() + ">");
            }
        }
        else if (xml.isEndElement())
        {
            if (s == 2)
            {
                if (xml.name().toString() == "value")
                {
                    s = 1;
                }
                else
                {
                    xml.raiseError("expected </value>.   got : </" + xml.name().toString() + ">");
                }
            }
            else if (s == 1)
            {
                if (xml.name().toString() == "data")
                {
                    s = -1;
                }
                else
                {
                    xml.raiseError("expected </data>.   got : </" + xml.name().toString() + ">");
                }
            }
            else if (s == -1)
            {
                if (xml.name().toString() == "array")
                {
                    return l;
                }
                else
                {
                    xml.raiseError("expected </array>.   got : </" + xml.name().toString() + ">");
                }
            }
            else if (s == 0)
            {
                xml.raiseError("expected <data>.   got : </" + xml.name().toString() + ">");
            }
        }
    }
    return QVariant();
}

QVariant deserializeStruct(QXmlStreamReader & xml)
{

    QVariantMap l;
    QString key;
    QVariant value;
    int s = 0;

    while (!xml.atEnd())
    {
        xml.readNext();
        if (xml.isStartElement())
        {
            if (s == 0)
            {
                if (xml.name().toString() == "member")
                {
                    s = 1;
                }
                else
                {
                    xml.raiseError("expected <member>.   got : <" + xml.name().toString() + ">");
                }
            }
            else if (s == 1)
            {
                if (xml.name().toString() == "name")
                {
                    key = xml.readElementText();
                }
                else if (xml.name().toString() == "value")
                {
                    value = deserialize(xml);
                    s = 2;
                    if (xml.isEndElement() && xml.name().toString() == "value"){
                        s=1;
                    }
                }
                else
                {
                    xml.raiseError("expected <name> or <value>.   got : <" + xml.name().toString() + ">");
                }
            }
            else if (s == 2)
            {
                xml.raiseError("expected </name>.   got : <" + xml.name().toString() + ">");
            }
            else if (s == 3)
            {
                xml.raiseError("expected </value>.   got : <" + xml.name().toString() + ">");
            }

        }
        else if (xml.isEndElement())
        {
            if (s == 2)
            {
                if (xml.name().toString() == "value")
                {
                    s = 1;
                }
                else
                {
                    xml.raiseError("expected </value>.   got : </" + xml.name().toString() + ">");
                }
            }
            else if (s == 1)
            {
                if (xml.name().toString() == "member")
                {
                    l[key] = value;
                    s = 0;
                }
                else
                {
                    xml.raiseError("expected </member>.   got : </" + xml.name().toString() + ">");
                }
            }
            else if (s == 0)
            {
                if (xml.name().toString() == "struct")
                {
                    return l;
                }
                else
                {
                    xml.raiseError("expected </struct>.   got : </" + xml.name().toString() + ">");
                }
            }
        }
    }
    return QVariant();
}

QVariant QxtXmlRpc::deserialize(QXmlStreamReader & xml)
{
    QVariant ret;
    while (!xml.atEnd())
    {
        xml.readNext();
        if (xml.isStartElement())
        {
            if (xml.name().toString() == "array")
            {
                return deserializeArray(xml);
            }
            else if (xml.name().toString() == "base64")
            {
                return QByteArray::fromBase64(xml.readElementText().toLatin1());
            }
            else if (xml.name().toString() == "boolean")
            {
                return (xml.readElementText().toInt() == 1);
            }
            else if (xml.name().toString() == "dateTime.iso8601")
            {
                return QDateTime::fromString(xml.readElementText(), Qt::ISODate);
            }
            else if (xml.name().toString() == "double")
            {
                return xml.readElementText().toDouble();
            }
            else if (xml.name().toString() == "integer" || xml.name().toString() == "i4")
            {
                return xml.readElementText().toInt();
            }
            else if (xml.name().toString() == "string")
            {
                return xml.readElementText();
            }
            else if (xml.name().toString() == "struct")
            {
                return deserializeStruct(xml);
            }
            else
            {
                ret = QVariant();
            }
        }
        // The spec say, "If no type is indicated, the type is string."
        else if (xml.isCharacters())
        {
            return xml.text().toString();
        }
        else if (xml.isEndElement() && xml.name().toString() == "value")
        {
            return QString("");
        }
    }
    return QVariant();
}
