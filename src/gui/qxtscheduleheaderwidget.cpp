#include "qxtscheduleheaderwidget.h"
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

#include "qxtscheduleview.h"
#include "qxtscheduleviewheadermodel_p.h"

#include <QPainter>
#include <QDateTime>
#include <QDate>
#include <QTime>


/*!
 *  @internal the QxtAgendaHeaderWidget operates on a internal model , that uses the QxtScheduleView as DataSource
 *
 */

QxtScheduleHeaderWidget::QxtScheduleHeaderWidget(Qt::Orientation orientation , QxtScheduleView *parent) : QHeaderView(orientation, parent)
{
    QxtScheduleViewHeaderModel *model = new QxtScheduleViewHeaderModel(this);
    setModel(model);

    if (parent)
    {
        model->setDataSource(parent);
    }
}

void QxtScheduleHeaderWidget::paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const
{
    if (model())
    {
        switch (orientation())
        {
        case Qt::Horizontal:
        {
            QHeaderView::paintSection(painter, rect, logicalIndex);
        }
        break;
        case Qt::Vertical:
        {
            QTime time = model()->headerData(logicalIndex, Qt::Vertical, Qt::DisplayRole).toTime();
            if (time.isValid())
            {
                QRect temp = rect;
                temp.adjust(1, 1, -1, -1);

                painter->fillRect(rect, this->palette().background());

                if (time.minute() == 0)
                {
                    painter->drawLine(temp.topLeft() + QPoint(temp.width() / 3, 0), temp.topRight());
                    painter->drawText(temp, Qt::AlignTop | Qt::AlignRight, time.toString("hh:mm"));
                }
            }
        }
        break;
        default:
            Q_ASSERT(false); //this will never happen... normally
        }
    }
}

void QxtScheduleHeaderWidget::setModel(QxtScheduleViewHeaderModel *model)
{
    QHeaderView::setModel(model);
}
