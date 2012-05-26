#include "qxtlistwidget.h"
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

#include "qxtlistwidget_p.h"
#include "qxtitemdelegate.h"

QxtListWidgetPrivate::QxtListWidgetPrivate()
{}

void QxtListWidgetPrivate::informStartEditing(const QModelIndex& index)
{
    QListWidgetItem* item = qxt_p().itemFromIndex(index);
    Q_ASSERT(item);
    emit qxt_p().itemEditingStarted(item);
}

void QxtListWidgetPrivate::informFinishEditing(const QModelIndex& index)
{
    QListWidgetItem* item = qxt_p().itemFromIndex(index);
    Q_ASSERT(item);
    emit qxt_p().itemEditingFinished(item);
}

/*!
    \class QxtListWidget
    \inmodule QxtWidgets
    \brief The QxtListWidget class is an extended QListWidget with additional signals.

    QxtListWidget offers a few most commonly requested signals.

    \image qxtlistwidget.png "QxtListWidget in Plastique style."

    \sa QxtListWidgetItem
 */

/*!
    \fn QxtListWidget::itemEditingStarted(QListWidgetItem* item)

    This signal is emitted after the editing of \a item has been started.

    \sa itemEditingFinished()
 */

/*!
    \fn QxtListWidget::itemEditingFinished(QListWidgetItem* item)

    This signal is emitted after the editing of \a item has been finished.

    \sa itemEditingStarted()
 */

/*!
    \fn QxtListWidget::itemCheckStateChanged(QxtListWidgetItem* item)

    This signal is emitted whenever the check state of \a item has changed.

    \bold {Note:} Use QxtListWidgetItem in order to enable this feature.

    \sa QxtListWidgetItem, QListWidgetItem::checkState()
 */

/*!
    Constructs a new QxtListWidget with \a parent.
 */
QxtListWidget::QxtListWidget(QWidget* parent) : QListWidget(parent)
{
    QXT_INIT_PRIVATE(QxtListWidget);
    QxtItemDelegate* delegate = new QxtItemDelegate(this);
    connect(delegate, SIGNAL(editingStarted(const QModelIndex&)),
            &qxt_d(), SLOT(informStartEditing(const QModelIndex&)));
    connect(delegate, SIGNAL(editingFinished(const QModelIndex&)),
            &qxt_d(), SLOT(informFinishEditing(const QModelIndex&)));
    setItemDelegate(delegate);
}

/*!
    Destructs the list widget.
 */
QxtListWidget::~QxtListWidget()
{}
