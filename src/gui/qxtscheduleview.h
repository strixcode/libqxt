#ifndef QXTSCHEDULEVIEW_H_INCLUDED
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

#define QXTSCHEDULEVIEW_H_INCLUDED

#include <QAbstractScrollArea>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QHeaderView>
#include <QRect>

#include "qxtglobal.h"
#include "qxtnamespace.h"

QT_FORWARD_DECLARE_CLASS(QAbstractItemModel)
class QxtScheduleItemDelegate;
class QxtScheduleViewPrivate;
class QxtScheduleInternalItem;

#if 0
enum
{
    TIMERANGEPERCOL = 86400,
    TIMESTEP = 900
};
#endif


class QXT_GUI_EXPORT QxtScheduleView : public QAbstractScrollArea
{
    Q_OBJECT

    QXT_DECLARE_PRIVATE(QxtScheduleView)
    friend class QxtScheduleInternalItem;
    friend class QxtScheduleViewHeaderModel;

public:

    enum ViewMode
    {
        MinuteView,
        HourView,
        DayView,
        CustomView
    };

    QxtScheduleView(QWidget *parent = 0);

    void                        setModel(QAbstractItemModel *model);
    QAbstractItemModel*         model() const;

    void                        setViewMode(const QxtScheduleView::ViewMode mode);
    QxtScheduleView::ViewMode   viewMode() const;
    
    QHeaderView*                verticalHeader ( ) const;
    QHeaderView*                horizontalHeader ( ) const;

    void                        setDateRange(const QDate & fromDate , const QDate & toDate);
    void                        setTimeRange(const QDateTime &fromDateTime , const QDateTime &toTime);
    
    QxtScheduleItemDelegate*    delegate () const;
    void                        setItemDelegate (QxtScheduleItemDelegate * delegate);

    void                        setZoomStepWidth(const int zoomWidth , const Qxt::Timeunit unit = Qxt::Second);
    void                        setCurrentZoomDepth(const int depth , const Qxt::Timeunit unit = Qxt::Second);
    int                         currentZoomDepth(const Qxt::Timeunit unit = Qxt::Second);

    QPoint                      mapFromViewport(const QPoint& point) const;
    QPoint                      mapToViewport(const QPoint& point) const;

    QModelIndex                 indexAt(const QPoint &pt);
    void                        raiseItem(const QModelIndex &index);
    void                        handleItemConcurrency(const QModelIndex &index);
    QModelIndex                 currentIndex();
    int                         rows() const;
    int                         cols() const;

Q_SIGNALS:
    void                        itemMoved(int rows, int cols, QModelIndex index);
    void                        indexSelected(QModelIndex index);
    void                        indexDoubleClicked(QModelIndex index);
    void                        contextMenuRequested(QModelIndex index);
    void                        newZoomDepth(const int newDepthInSeconds);
    void                        viewModeChanged(const int newViewMode);


protected:
    virtual int                 timePerColumn() const;
    virtual void                adjustRangeToViewMode(QDateTime *startTime, QDateTime *endTime) const;

    virtual void                scrollContentsBy(int dx, int dy);
    virtual void                paintEvent(QPaintEvent  *e);
    virtual void                mouseMoveEvent(QMouseEvent  * e);
    virtual void                mousePressEvent(QMouseEvent  * e);
    virtual void                mouseReleaseEvent(QMouseEvent  * e);
    virtual void                mouseDoubleClickEvent ( QMouseEvent * e );
    virtual void                resizeEvent(QResizeEvent * e);
    virtual void                wheelEvent(QWheelEvent  * e);

public Q_SLOTS:
    void                        dataChanged(const QModelIndex & topLeft, const  QModelIndex & bottomRight);
    void                        updateGeometries();
    void                        zoomIn();
    void                        zoomOut();

protected Q_SLOTS:
    virtual void                rowsAboutToBeRemoved(const QModelIndex & parent, int start, int end);
    virtual void                rowsAboutToBeInserted(const QModelIndex & parent, int start, int end);
    virtual void                rowsRemoved(const QModelIndex & parent, int start, int end);
    virtual void                rowsInserted(const QModelIndex & parent, int start, int end);
};

#endif //QXTSCHEDULEVIEW_H_INCLUDED
