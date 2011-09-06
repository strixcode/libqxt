#ifndef QXTSORTFILTERPROXYMODEL_H
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

#define QXTSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QRegExp>
#include <QMap>
#include <QVariant>

#include "qxtglobal.h"

class QxtSortFilterProxyModelPrivate;

class QXT_GUI_EXPORT QxtSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    QxtSortFilterProxyModel( QObject *parent = 0 );

    void beginDeclareFilter( );
    void endDeclareFilter( );
    void setFilter( const int column, const QVariant &value, const int role = Qt::DisplayRole, Qt::MatchFlags flags = Qt::MatchContains );
    void setFilterValue( const int column, const QVariant &value );
    void setFilterRole( const int column, const int role = Qt::DisplayRole );
    void setFilterFlags( const int column, const Qt::MatchFlags flags = Qt::MatchContains );
    void removeFilter( const int column );

    QVariant filterValue( const int column ) const;
    int filterRole( const int column ) const;
    Qt::MatchFlags filterFlags( const int column ) const;

    bool isFiltered( const int column );

protected:
    virtual bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const;

private:
    QXT_DECLARE_PRIVATE(QxtSortFilterProxyModel)
};

#endif // QXTSORTFILTERPROXYMODEL_H
