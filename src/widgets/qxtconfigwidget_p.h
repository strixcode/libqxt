#ifndef QXTCONFIGWIDGET_P_H
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

#define QXTCONFIGWIDGET_P_H

#include "qxtconfigwidget.h"
#include <QItemDelegate>
#include <QTableWidget>

QT_FORWARD_DECLARE_CLASS(QSplitter)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)

class QxtConfigTableWidget : public QTableWidget
{
public:
    QxtConfigTableWidget(QWidget* parent = 0);
    QStyleOptionViewItem viewOptions() const;
    QSize sizeHint() const;

    bool hasHoverEffect() const;
    void setHoverEffect(bool enabled);
};

class QxtConfigDelegate : public QItemDelegate
{
public:
    QxtConfigDelegate(QObject* parent = 0);
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    bool hover;
};

class QxtConfigWidgetPrivate : public QObject, public QxtPrivate<QxtConfigWidget>
{
    Q_OBJECT

public:
    QXT_DECLARE_PUBLIC(QxtConfigWidget)

    void init(QxtConfigWidget::IconPosition position = QxtConfigWidget::West);
    void initTable();
    void relayout();
    QTableWidgetItem* item(int index) const;

    QSplitter* splitter;
    QStackedWidget* stack;
    QxtConfigTableWidget* table;
    QxtConfigWidget::IconPosition pos;

public Q_SLOTS:
    void setCurrentIndex(int row, int column);
    void setCurrentIndex(int index);
};

#endif // QXTCONFIGWIDGET_P_H
