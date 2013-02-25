
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

#include "qxtscheduleviewheadermodel_p.h"
#include "qxtscheduleview.h"
#include "qxtscheduleview_p.h"
#include <QDateTime>
#include <QDebug>

QxtScheduleViewHeaderModel::QxtScheduleViewHeaderModel(QObject *parent) : QAbstractTableModel(parent)
        , m_rowCountBuffer(0)
        , m_colCountBuffer(0)
{

}

void QxtScheduleViewHeaderModel::newZoomDepth(const int zoomDepth)
{
    Q_UNUSED(zoomDepth);

    if (this->m_dataSource)
    {
        /*
        qDebug()<<"old rows "<<m_rowCountBuffer<<" new rows "<<m_dataSource->rows();
        beginRemoveRows(QModelIndex(),0,m_rowCountBuffer);
        m_rowCountBuffer = 0;
        endRemoveRows();

        beginInsertRows(QModelIndex(),0,m_dataSource->rows());
        m_rowCountBuffer = m_dataSource->rows();
        endInsertRows();
        */
		beginResetModel();
        m_rowCountBuffer = m_dataSource->rows();
        endResetModel();

    }
}

void QxtScheduleViewHeaderModel::viewModeChanged(const int viewMode)
{
    Q_UNUSED(viewMode);

    if (this->m_dataSource)
    {
        beginRemoveRows(QModelIndex(), 0, m_rowCountBuffer);
        m_rowCountBuffer = 0;
        endRemoveRows();

        beginInsertRows(QModelIndex(), 0, m_dataSource->rows());
        m_rowCountBuffer = m_dataSource->rows();
        endInsertRows();

        beginRemoveColumns(QModelIndex(), 0, m_colCountBuffer);
        m_colCountBuffer = 0;
        endRemoveColumns();

        beginInsertColumns(QModelIndex(), 0, m_dataSource->cols());
        m_colCountBuffer = m_dataSource->cols();
        endInsertColumns();
    }
}

void QxtScheduleViewHeaderModel::setDataSource(QxtScheduleView *dataSource)
{
    if (this->m_dataSource)
    {
        disconnect(m_dataSource, SIGNAL(newZoomDepth(const int)), this, SLOT(newZoomDepth(const int)));
        disconnect(m_dataSource, SIGNAL(viewModeChanged(const int)), this, SLOT(viewModeChanged(const int)));

        emit beginRemoveRows(QModelIndex(), 0, m_rowCountBuffer);
        m_rowCountBuffer = 0;
        emit endRemoveRows();

        emit beginRemoveColumns(QModelIndex(), 0, m_colCountBuffer);
        m_colCountBuffer = 0;
        emit endRemoveColumns();
    }

    if (dataSource)
    {
        connect(dataSource, SIGNAL(newZoomDepth(const int)), this, SLOT(newZoomDepth(const int)));
        connect(dataSource, SIGNAL(viewModeChanged(const int)), this, SLOT(viewModeChanged(const int)));

        emit beginInsertRows(QModelIndex(), 0, dataSource->rows());
        m_rowCountBuffer = dataSource->rows();
        emit endInsertRows();

        emit beginInsertColumns(QModelIndex(), 0, dataSource->cols());
        m_colCountBuffer = dataSource->cols();
        emit endInsertColumns();
    }

    m_dataSource = dataSource;
}

QModelIndex QxtScheduleViewHeaderModel::parent(const QModelIndex & index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

int QxtScheduleViewHeaderModel::rowCount(const QModelIndex & parent) const
{
    if (!parent.isValid())
    {
        if (this->m_dataSource)
            return m_dataSource->rows();
    }
    return 0;
}

int QxtScheduleViewHeaderModel::columnCount(const QModelIndex & parent) const
{
    if (!parent.isValid())
    {
        if (this->m_dataSource)
            return m_dataSource->cols();
    }
    return 0;
}

QVariant QxtScheduleViewHeaderModel::data(const QModelIndex & index, int role) const
{
    Q_UNUSED(index);
    Q_UNUSED(role);
    return QVariant();
}

bool QxtScheduleViewHeaderModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    return false;
}

bool QxtScheduleViewHeaderModel::insertRow(int row, const QModelIndex & parent)
{
    Q_UNUSED(row);
    Q_UNUSED(parent);
    return false;
}

QVariant QxtScheduleViewHeaderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!m_dataSource)
        return QVariant();
#if 0
    if (role == Qt::SizeHintRole)
    {
        if (this->viewModel)
        {
            return viewModel->headerData(section, orientation, role);
        }
    }
#endif

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (Qt::Horizontal == orientation)
        {
            int iTableOffset = m_dataSource->qxt_d().visualIndexToOffset(0, section);
            QDateTime startTime = QDateTime::fromTime_t(m_dataSource->qxt_d().offsetToUnixTime(iTableOffset));
            return QVariant(startTime.date().toString());
        }
        else
        {
            int iTableOffset = m_dataSource->qxt_d().visualIndexToOffset(section, 0);
            QTime time = QDateTime::fromTime_t(m_dataSource->qxt_d().offsetToUnixTime(iTableOffset)).time();
            return QVariant(time.toString());
        }
    }

    return QVariant();
}

Qt::ItemFlags QxtScheduleViewHeaderModel::flags(const QModelIndex & index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled;
}

bool QxtScheduleViewHeaderModel::hasChildren(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return false;
}


