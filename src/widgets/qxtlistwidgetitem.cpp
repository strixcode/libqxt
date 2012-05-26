#include "qxtlistwidgetitem.h"
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

#include "qxtlistwidget.h"

/*!
    \class QxtListWidgetItem
    \inmodule QxtWidgets
    \brief The QxtListWidgetItem class is an extended QListWidgetItem.

    QxtListWidgetItem provides means for offering check state change signals and
    convenience methods for testing and setting flags.

    \sa QxtListWidget
 */

/*!
    Constructs a new QxtListWidgetItem with \a parent and \a type.
 */
QxtListWidgetItem::QxtListWidgetItem(QListWidget* parent, int type)
        : QListWidgetItem(parent, type)
{
}

/*!
    Constructs a new QxtListWidgetItem with \a text, \a parent and \a type.
 */
QxtListWidgetItem::QxtListWidgetItem(const QString& text, QListWidget* parent, int type)
        : QListWidgetItem(text, parent, type)
{
}

/*!
    Constructs a new QxtListWidgetItem with \a icon, \a text, \a parent and \a type.
 */
QxtListWidgetItem::QxtListWidgetItem(const QIcon& icon, const QString& text, QListWidget* parent, int type)
        : QListWidgetItem(icon, text, parent, type)
{
}

/*!
    Constructs a copy of \a other.
 */
QxtListWidgetItem::QxtListWidgetItem(const QxtListWidgetItem& other)
        : QListWidgetItem(other)
{
}

/*!
    Destructs the list widget item.
 */
QxtListWidgetItem::~QxtListWidgetItem()
{
}

/*!
    Returns \c true if the \a flag is set, otherwise \c false.

    \sa setFlag(), QListWidgetItem::flags(), Qt::ItemFlag
 */
bool QxtListWidgetItem::testFlag(Qt::ItemFlag flag) const
{
    return (flags() & flag);
}

/*!
    If \a enabled is \c true, the item \a flag is enabled; otherwise, it is disabled.

    \sa testFlag(), QListWidgetItem::setFlags(), Qt::ItemFlag
 */
void QxtListWidgetItem::setFlag(Qt::ItemFlag flag, bool enabled)
{
    if (enabled)
        setFlags(flags() | flag);
    else
        setFlags(flags() & ~flag);
}

/*!
    \reimp
 */
void QxtListWidgetItem::setData(int role, const QVariant& value)
{
    if (role == Qt::CheckStateRole)
    {
        const Qt::CheckState newState = static_cast<Qt::CheckState>(value.toInt());
        const Qt::CheckState oldState = static_cast<Qt::CheckState>(data(role).toInt());

        QListWidgetItem::setData(role, value);

        if (newState != oldState)
        {
            QxtListWidget* list = qobject_cast<QxtListWidget*>(listWidget());
            if (list)
            {
                emit list->itemCheckStateChanged(this);
            }
        }
    }
    else
    {
        QListWidgetItem::setData(role, value);
    }
}
