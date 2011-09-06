
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

#include "qxtsqlpackage.h"
#include <QBuffer>
#include <QDataStream>
#include <QSqlRecord>
#include <QVector>
#include <QDebug>
#include <QVariant>

/*!
\class QxtSqlPackage

\inmodule QxtSql

\brief The QxtSqlPackage class provides a full serializable QSqlQuery storage


Sometimes you want to send sql results over network or store them into files.
QxtSqlPackage can provide you a storage that is still valid after the actual QSqlQuery has been destroyed.
for confidence the interface is similar to QSqlQuery.
*/


/*!
\fn  bool QxtSqlPackage::isValid()
\brief determinates if the package curently points to a valid row
*/

/*!
\fn int QxtSqlPackage::at()
\brief curent pointer position
*/

/*!
\fn bool QxtSqlPackage::next()
\brief point to next entry

returns false if there is no next entry.
provided for easy porting from QSqlQuery.

\code
while (query.next())
    {
    }
\endcode
*/

/*!
\fn bool QxtSqlPackage::last();
\brief point to last entry in storage
*/

/*!
\fn bool QxtSqlPackage::first();
\brief point to first entry in storage
*/

/*!
\fn QString QxtSqlPackage::value(const QString& key);
\brief return a column in current row
in contrast to QSqlQuery you have to provide the name of the \a key.

the entry is returned as QString becouse in most cases you need QString anyway, and converting to needed data type is easy.
\code
QString name = query.value("name");
\endcode
*/

/*!
\fn void QxtSqlPackage::insert(QSqlQuery query);

\brief read from \a query

read out a QSqlQuery and store the result. you may close the query after reading, the data will stay.

\code
QxSqlPackage::insert(QSqlQuery::exec("select name,foo,bar from table;"));
\endcode
*/

/*!
\fn int QxtSqlPackage::count() const;
\brief Returns the number of rows stored
*/

/*!
\fn QByteArray QxtSqlPackage::data() const;
\brief Returns serialised data
*/

/*!
\fn void QxtSqlPackage::setData(const QByteArray& data);
\brief Deserialise \a data
*/

/*!
\fn QHash<QString,QString> QxtSqlPackage::hash(int index);
\brief return a specific \a index as Hash
*/

/*!
\fn QHash<QString,QString> QxtSqlPackage::hash();
\brief return the curent row as Hash
*/

/*!
\fn QxtSqlPackage& QxtSqlPackage::operator= (const QxtSqlPackage& other);
\brief copy \a other
*/

/*!
Constructs a QxtSqlPackage with \a parent.
*/
QxtSqlPackage::QxtSqlPackage(QObject *parent) : QObject(parent)
{
    record = -1;
}

/*!
Constructs a copy of \a other with \a parent.
*/
QxtSqlPackage::QxtSqlPackage(const QxtSqlPackage & other, QObject *parent) : QObject(parent)
{
    record = -1;

    setData(other.data());
}

/*!
Returns \c true if the package is valid, \c false otherwise.
*/
bool QxtSqlPackage::isValid()
{
    if ((record >= 0) && (record < map.count()))
        return true;
    else
        return false;
}

int QxtSqlPackage::at()
{
    return record;
}

bool QxtSqlPackage::next()
{
    record++;
    if (record > (map.count() - 1))
    {
        last();
        return false;
    }

    return true;
}

bool QxtSqlPackage::last()
{
    record = map.count() - 1;
    if (record >= 0)
        return true;
    else
        return false;
}

bool QxtSqlPackage::first()
{
    if (map.count())
    {
        record = 0;
        return true;
    }
    else
    {
        record = -1;
        return false;
    }
}

QString QxtSqlPackage::value(const QString& key)
{
    if ((record < 0) || !map.count()) return QString();

    return map.at(record).value(key);
}

void QxtSqlPackage::insert(QSqlQuery query)
{
    map.clear();
    record = -1;

    /*query will be invalid if next is not called first*/
    if (!query.isValid())
        query.next();

    QSqlRecord infoRecord = query.record();
    int iNumCols = infoRecord.count();
    QVector<QString> tableMap = QVector<QString>(iNumCols);

    /*first create a map of index->colname pairs*/
    for (int iLoop = 0; iLoop < iNumCols; iLoop++)
    {
        tableMap[iLoop] = infoRecord.fieldName(iLoop);
    }

    /*now use this created map to get column names
     *this should be faster than querying the QSqlRecord every time
     *but that depends on the databasetype and size of the table (number of rows and cols)
     */
    do
    {
        QHash<QString, QString> hash;
        for (int iColLoop = 0; iColLoop < iNumCols; iColLoop++)
        {
            hash[tableMap[iColLoop]] = query.value(iColLoop).toString();
        }
        map.append(hash);

    }
    while (query.next());
}

int QxtSqlPackage::count() const
{
    return map.count();
}

QByteArray QxtSqlPackage::data() const
{
    QBuffer buff;
    buff.open(QBuffer::WriteOnly);
    QDataStream stream(&buff);

    stream << count();
    for (int i = 0; i < count();i++)
        stream << map.at(i);

    buff.close();
    return buff.data();
}

void QxtSqlPackage::setData(const QByteArray& data)
{
    map.clear();
    record = -1;

    QBuffer buff;
    buff.setData(data);
    buff.open(QBuffer::ReadOnly);
    QDataStream stream(&buff);

    int c;
    stream >> c;

    for (int i = 0; i < c;i++)
    {
        QHash<QString, QString> hash;
        stream >> hash;
        map.append(hash);
    }
}


QHash<QString, QString> QxtSqlPackage::hash(int index)
{
    if (index > count()) return QHash<QString, QString>();
    return map.at(index);
}


QHash<QString, QString> QxtSqlPackage::hash()
{
    qDebug() << record;
    return map.at(record);
}


QxtSqlPackage& QxtSqlPackage::operator= (const QxtSqlPackage & other)
{
    setData(other.data());
    return *this;
}

