#include "qxttreewidget.h"
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

#include "qxttreewidget_p.h"
#include "qxtitemdelegate.h"
#include <QHeaderView>

QxtTreeWidgetPrivate::QxtTreeWidgetPrivate()
{}

QxtItemDelegate* QxtTreeWidgetPrivate::delegate() const
{
    QxtItemDelegate* del = qobject_cast<QxtItemDelegate*>(qxt_p().itemDelegate());
    Q_ASSERT(del);
    return del;
}

void QxtTreeWidgetPrivate::informStartEditing(const QModelIndex& index)
{
    QTreeWidgetItem* item = qxt_p().itemFromIndex(index);
    Q_ASSERT(item);
    emit qxt_p().itemEditingStarted(item);
}

void QxtTreeWidgetPrivate::informFinishEditing(const QModelIndex& index)
{
    QTreeWidgetItem* item = qxt_p().itemFromIndex(index);
    Q_ASSERT(item);
    emit qxt_p().itemEditingFinished(item);
}

void QxtTreeWidgetPrivate::expandCollapse(QTreeWidgetItem* item)
{
    if (item && !item->parent() && delegate()->decorationStyle() != Qxt::NoDecoration)
        qxt_p().setItemExpanded(item, !qxt_p().isItemExpanded(item));
}

/*!
    \class QxtTreeWidget
    \inmodule QxtWidgets
    \brief The QxtTreeWidget class is an extended QTreeWidget with additional signals.

    QxtTreeWidget offers an optional top level item decoration
    and a few most commonly requested signals.

    \image qxttreewidget.png "QxtTreeWidget with Qxt::Menulike and Qxt::Buttonlike decoration styles, respectively."

    \sa QxtTreeWidgetItem
 */

/*!
    \fn QxtTreeWidget::itemEditingStarted(QTreeWidgetItem* item)

    This signal is emitted after the editing of \a item has been started.

    \sa itemEditingFinished()
 */

/*!
    \fn QxtTreeWidget::itemEditingFinished(QTreeWidgetItem* item)

    This signal is emitted after the editing of \a item has been finished.

    \sa itemEditingStarted()
 */

/*!
    \fn QxtTreeWidget::itemCheckStateChanged(QxtTreeWidgetItem* item)

    This signal is emitted whenever the check state of \a item has changed.

    \bold {Note:} Use QxtTreeWidgetItem in order to enable this feature.

    \sa QxtTreeWidgetItem, QTreeWidgetItem::checkState()
 */

/*!
    Constructs a new QxtTreeWidget with \a parent.
 */
QxtTreeWidget::QxtTreeWidget(QWidget* parent) : QTreeWidget(parent)
{
    QXT_INIT_PRIVATE(QxtTreeWidget);
    QxtItemDelegate* delegate = new QxtItemDelegate(this);
    connect(delegate, SIGNAL(editingStarted(const QModelIndex&)),
            &qxt_d(), SLOT(informStartEditing(const QModelIndex&)));
    connect(delegate, SIGNAL(editingFinished(const QModelIndex&)),
            &qxt_d(), SLOT(informFinishEditing(const QModelIndex&)));
    connect(this, SIGNAL(itemPressed(QTreeWidgetItem*, int)),
            &qxt_d(), SLOT(expandCollapse(QTreeWidgetItem*)));
    setItemDelegate(delegate);
}

/*!
    Destructs the tree widget.
 */
QxtTreeWidget::~QxtTreeWidget()
{}

/*!
    \property QxtTreeWidget::decorationStyle
    \brief the top level item decoration style

    Top level items are decorated according to this property.
    The default value is Qxt::NoDecoration.

    \bold {Note:} Setting the property to anything else than Qxt::NoDecoration
    hides the header and sets QTreeView::rootIsDecorated to \c false
    (to avoid multiple branch indicators).

    \sa Qxt::DecorationStyle, QTreeView::rootIsDecorated
 */
Qxt::DecorationStyle QxtTreeWidget::decorationStyle() const
{
    return qxt_d().delegate()->decorationStyle();
}

void QxtTreeWidget::setDecorationStyle(Qxt::DecorationStyle style)
{
    if (qxt_d().delegate()->decorationStyle() != style)
    {
        qxt_d().delegate()->setDecorationStyle(style);

        if (style != Qxt::NoDecoration)
        {
            setRootIsDecorated(false);
            header()->hide();
        }
        reset();
    }
}

/*!
    \property QxtTreeWidget::elideMode
    \brief the text elide mode

    The text of a decorated top level item is elided according to this property.
    The default value is Qt::ElideMiddle.

    \bold {Note:} The property has effect only for decorated top level items.

    \sa decorationStyle, Qt::TextElideMode
 */
Qt::TextElideMode QxtTreeWidget::elideMode() const
{
    return qxt_d().delegate()->elideMode();
}

void QxtTreeWidget::setElideMode(Qt::TextElideMode mode)
{
    if (qxt_d().delegate()->elideMode() != mode)
    {
        qxt_d().delegate()->setElideMode(mode);
        reset();
    }
}
