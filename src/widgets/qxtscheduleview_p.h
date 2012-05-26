
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

#ifndef QXTSCHEDULEVIEW_P_INCLUDED
#define QXTSCHEDULEVIEW_P_INCLUDED

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


#include <QList>
#include <QLinkedList>
#include <QVector>
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QPainterPath>
#include <QTimer>
#include "qxtnamespace.h"
#include "qxtscheduleitemdelegate.h"

class QxtScheduleView;
class QxtScheduleHeaderWidget;

class QxtScheduleInternalItem : public QObject
{
    Q_OBJECT
    friend class QxtScheduleView;

public:

    QxtScheduleInternalItem(QxtScheduleView *parent , QModelIndex index , QVector<QRect> geometries = QVector<QRect>());

    bool                setData(QVariant data , int role);
    QVariant            data(int role) const;

    int                 visualStartTableOffset() const;
    int                 visualEndTableOffset() const;

    int                 startTableOffset() const;
    int                 endTableOffset() const;
    void                setStartTableOffset(int iOffset);

    int                 rows() const;
    void                setRowsUsed(int rows);
    QxtScheduleView*    parentView() const;

    void                startMove();
    void                resetMove();
    void                stopMove();

    bool                contains(const QPoint &pt);
    void                setGeometry(const QVector<QRect> geo);
    QModelIndex         modelIndex() const;
    void                setDirty(bool state = true)
    {
        isDirty = state;
    }

    QVector<QRect>      geometry() const;

Q_SIGNALS:
    void                geometryChanged(QxtScheduleInternalItem * item , QVector<QRect> oldGeometry);

public:
    bool m_moving;
    bool isDirty;
    int m_iModelRow;
    QVector<QRect> m_geometries;
    QVector<QRect> m_SavedGeometries;
    QVector<QPixmap> m_cachedParts;
};

class QxtScheduleViewPrivate : public QObject, public QxtPrivate<QxtScheduleView>
{
    Q_OBJECT
public:

    QXT_DECLARE_PUBLIC(QxtScheduleView)
    QxtScheduleViewPrivate();


    int offsetToVisualColumn(const int iOffset) const;
    int offsetToVisualRow(const int iOffset) const;
    int visualIndexToOffset(const int iRow, const int iCol) const;
    int unixTimeToOffset(const uint constUnixTime, bool indexEndTime = false) const;
    int offsetToUnixTime(const int offset, bool indexEndTime = false) const;
    QVector< QRect > calculateRangeGeometries(const int iStartOffset, const int iEndOffset) const;
    int pointToOffset(const QPoint & point);
    void handleItemConcurrency(const int from, const int to);
    QList< QLinkedList<QxtScheduleInternalItem *> >findConcurrentItems(const int from, const int to) const;


    QxtScheduleInternalItem *internalItemAt(const QPoint &pt);
    QxtScheduleInternalItem *internalItemAtModelIndex(const QModelIndex &index);


    void init();
    void reloadItemsFromModel();

    QxtScheduleInternalItem * itemForModelIndex(const QModelIndex &index)const
    {
        for (int iLoop = 0; iLoop < m_Items.size(); iLoop++)
        {
            if (m_Items.at(iLoop)->modelIndex() == index)
                return m_Items.at(iLoop);
        }
        return 0;
    }

    void handleItemConcurrency(QxtScheduleInternalItem *item)
    {
        if (item)
        {
            int startOffset = item->startTableOffset();
            int endOffset = startOffset +  item->rows() - 1 ;
            handleItemConcurrency(startOffset, endOffset);
        }
    }

    QxtScheduleInternalItem *m_currentItem;
    QxtScheduleInternalItem *m_selectedItem;

    int m_lastMousePosOffset;
    int m_currentZoomDepth;
    int m_zoomStepWidth;
    int m_currentViewMode;
    uint m_startUnixTime;
    uint m_endUnixTime;

    QList<QxtScheduleInternalItem* > m_Items ;
    QList<QxtScheduleInternalItem* > m_InactiveItems; /*used for items that are not in the range of our view but we need to look if they get updates*/

    QTimer scrollTimer;

    QxtScheduleHeaderWidget *m_vHeader;
    QxtScheduleHeaderWidget *m_hHeader;

    int m_Cols;

    QAbstractItemModel *m_Model;
    bool handlesConcurrency;
    QxtScheduleItemDelegate *delegate;
    QxtScheduleItemDelegate *defaultDelegate;

public Q_SLOTS:
    void itemGeometryChanged(QxtScheduleInternalItem * item, QVector<QRect> oldGeometry);
    void scrollTimerTimeout();

};

bool qxtScheduleItemLessThan(const QxtScheduleInternalItem * item1, const QxtScheduleInternalItem * item2);

#endif
