
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qxt API.  It exists for the convenience
// of other Qxt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//


#ifndef QXTSCHEDULEVIEWHEADERMODEL_P_H_INCLUDED
#define QXTSCHEDULEVIEWHEADERMODEL_P_H_INCLUDED

#include <QAbstractTableModel>
#include <QtGlobal>
#include <QPointer>

/*!
 *@author Benjamin Zeller <zbenjamin@users.sourceforge.net>
 *@desc This Model is used to tell the Header how many rows and columns the view has
 */

class QxtScheduleView;

class QxtScheduleViewHeaderModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    QxtScheduleViewHeaderModel(QObject *parent = 0);

    void                    setDataSource(QxtScheduleView *dataSource);

    virtual QModelIndex     parent(const QModelIndex & index) const;
    virtual int             rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int             columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual  QVariant       data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool            setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual bool            insertRow(int row, const QModelIndex & parent = QModelIndex());
    virtual QVariant        headerData(int section, Qt::Orientation orientation,  int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags   flags(const QModelIndex &index) const;
    virtual bool            hasChildren(const QModelIndex & parent = QModelIndex()) const;

public Q_SLOTS:
    void                    newZoomDepth(const int zoomDepth);
    void                    viewModeChanged(const int viewMode);

private:
    QPointer<QxtScheduleView> m_dataSource;
    int                       m_rowCountBuffer;
    int                       m_colCountBuffer;
};

#endif // SPLITVIEWHEADERMODEL_H
