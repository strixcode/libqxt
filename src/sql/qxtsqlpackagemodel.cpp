
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

#include "qxtsqlpackagemodel.h"




/*!
\class QxtSqlPackageModel

\inmodule QxtSql

\brief The QxtSqlPackageModel class provides a read-only data model for QxtSqlPackage result..

example usage:
\code
    QSqlQuery q;
    q.exec("SELECT *");

    QxtSqlPackage p;
    p.insert(q);

    QxtSqlPackageModel m;
    m.setQuery(p);

    QTableView v;
    v.setModel(&m);
    v.show();
\endcode

*/

/*!
\fn void QxtSqlPackageModel::setQuery(QxtSqlPackage package)
\brief set the \a package for the model.

\warning do this before any access.
*/

/*!
    Creates a QxtSqlPackageModel with \a parent.
 */
QxtSqlPackageModel::QxtSqlPackageModel(QObject * parent) : QAbstractTableModel(parent)
{
}

void QxtSqlPackageModel::setQuery(QxtSqlPackage package)
{
    pack = package;
}

/*!
    \reimp
 */
int QxtSqlPackageModel::rowCount(const QModelIndex &) const
{
    return pack.count();
}

/*!
    \reimp
 */
int QxtSqlPackageModel::columnCount(const QModelIndex &) const
{
    QxtSqlPackage p = pack;
    return p.hash(0).count();
}

/*!
    \reimp
 */
QVariant QxtSqlPackageModel::data(const QModelIndex  & index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();



    if ((index.row() < 0)  || (index.column() < 0)) return QVariant();
    QxtSqlPackage p = pack;

    return  p.hash(index.row()).values().at(index.column());


}

/*!
    \reimp
 */
QVariant QxtSqlPackageModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (orientation == Qt::Vertical && role == Qt::DisplayRole)
        return section;


    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        QxtSqlPackage p = pack;
        return p.hash(0).keys().at(section)    ;
    }

    return QAbstractItemModel::headerData(section, orientation, role);

}
