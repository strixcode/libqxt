#include "qxtmodelserializer.h"
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

#include <QAbstractItemModel>
#include <QDataStream>

/*
    \class QxtModelSerializer
    \inmodule QxtCore
    \brief The QxtModelSerializer class provides serialization of QAbstractItemModel
 */

struct QxtModelItem
{
    QMap<int, QVariant> itemData;
    int rowCount;
    int columnCount;
};

inline QDataStream& operator<<(QDataStream& out, const QxtModelItem& item)
{
    out << item.itemData;
    out << item.rowCount;
    out << item.columnCount;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, QxtModelItem& item)
{
    in >> item.itemData;
    in >> item.rowCount;
    in >> item.columnCount;
    return in;
}

class QxtModelSerializerPrivate : public QxtPrivate<QxtModelSerializer>
{
public:
    QxtModelSerializerPrivate() : model(0) { }

    void save(QDataStream& stream, const QModelIndex& index) const;
    bool restore(QDataStream& stream, const QModelIndex& index);
    void print(const QByteArray& data) const;

    QAbstractItemModel* model;
};

void QxtModelSerializerPrivate::save(QDataStream& stream, const QModelIndex& index) const
{
    QxtModelItem item;
    item.itemData = model->itemData(index);
    item.rowCount = model->rowCount(index);
    item.columnCount = model->columnCount(index);
    stream << item;
    for (int r = 0; r < item.rowCount; ++r)
        for (int c = 0; c < item.columnCount; ++c)
            save(stream, model->index(r, c, index));
}

bool QxtModelSerializerPrivate::restore(QDataStream& stream, const QModelIndex& index)
{
    QxtModelItem item;
    stream >> item;

    if (index.isValid())
        model->setItemData(index, item.itemData);
    if (item.rowCount > 0)
        model->insertRows(0, item.rowCount, index);
    if (item.columnCount > 0)
        model->insertColumns(0, item.columnCount, index);

    for (int r = 0; r < item.rowCount; ++r)
    {
        for (int c = 0; c < item.columnCount; ++c)
            restore(stream, model->index(r, c, index));
    }
    return stream.status() == QDataStream::Ok;
}

void QxtModelSerializerPrivate::print(const QByteArray& data) const
{
    QDataStream stream(data);
    while (!stream.atEnd())
    {
        QxtModelItem item;
        stream >> item;
    }
}

QxtModelSerializer::QxtModelSerializer(QAbstractItemModel* model)
{
    qxt_d().model = model;
}

QxtModelSerializer::~QxtModelSerializer()
{
}

QAbstractItemModel* QxtModelSerializer::model() const
{
    return qxt_d().model;
}

void QxtModelSerializer::setModel(QAbstractItemModel* model)
{
    qxt_d().model = model;
}

QByteArray QxtModelSerializer::saveModel(const QModelIndex& index) const
{
    if (!qxt_d().model)
    {
        qWarning("QxtModelSerializer::saveModel(): model == null");
        return QByteArray();
    }

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    qxt_d().save(stream, index);
    qxt_d().print(data);
    return data;
}

bool QxtModelSerializer::restoreModel(const QByteArray& data, const QModelIndex& index)
{
    if (!qxt_d().model)
    {
        qWarning("QxtModelSerializer::restoreModel(): model == null");
        return false;
    }

    QDataStream stream(data);
    qxt_d().print(data);
    return qxt_d().restore(stream, index);
}
