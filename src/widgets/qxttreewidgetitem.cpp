#include "qxttreewidgetitem.h"
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

#include "qxttreewidget.h"

/*!
    \class QxtTreeWidgetItem
    \inmodule QxtWidgets
    \brief The QxtTreeWidgetItem class is an extended QTreeWidgetItem.

    QxtTreeWidgetItem provides means for offering check state change signals and
    convenience methods for testing and setting flags.

    \sa QxtTreeWidget
 */

/*!
    Constructs a new QxtTreeWidgetItem with \a type.
 */
QxtTreeWidgetItem::QxtTreeWidgetItem(int type)
        : QTreeWidgetItem(type)
{
}

/*!
    Constructs a new QxtTreeWidgetItem with \a strings and \a type.
 */
QxtTreeWidgetItem::QxtTreeWidgetItem(const QStringList& strings, int type)
        : QTreeWidgetItem(strings, type)
{
}

/*!
    Constructs a new QxtTreeWidgetItem with \a parent and \a type.
 */
QxtTreeWidgetItem::QxtTreeWidgetItem(QTreeWidget* parent, int type)
        : QTreeWidgetItem(parent, type)
{
}

/*!
    Constructs a new QxtTreeWidgetItem with \a parent, \a strings and \a type.
 */
QxtTreeWidgetItem::QxtTreeWidgetItem(QTreeWidget* parent, const QStringList& strings, int type)
        : QTreeWidgetItem(parent, strings, type)
{
}

/*!
    Constructs a new QxtTreeWidgetItem with \a parent, \a preceding and \a type.
 */
QxtTreeWidgetItem::QxtTreeWidgetItem(QTreeWidget* parent, QTreeWidgetItem* preceding, int type)
        : QTreeWidgetItem(parent, preceding, type)
{
}

/*!
    Constructs a new QxtTreeWidgetItem with \a parent and \a type.
 */
QxtTreeWidgetItem::QxtTreeWidgetItem(QTreeWidgetItem* parent, int type)
        : QTreeWidgetItem(parent, type)
{
}

/*!
    Constructs a new QxtTreeWidgetItem with \a parent, \a strings and \a type.
 */
QxtTreeWidgetItem::QxtTreeWidgetItem(QTreeWidgetItem* parent, const QStringList& strings, int type)
        : QTreeWidgetItem(parent, strings, type)
{
}

/*!
    Constructs a new QxtTreeWidgetItem with \a parent, \a preceding and \a type.
 */
QxtTreeWidgetItem::QxtTreeWidgetItem(QTreeWidgetItem* parent, QTreeWidgetItem* preceding, int type)
        : QTreeWidgetItem(parent, preceding, type)
{
}

/*!
    Constructs a copy of \a other.
 */
QxtTreeWidgetItem::QxtTreeWidgetItem(const QxtTreeWidgetItem& other)
        : QTreeWidgetItem(other)
{
}

/*!
    Destructs the tree widget item.
 */
QxtTreeWidgetItem::~QxtTreeWidgetItem()
{
}

/*!
    Returns \c true if the \a flag is set, otherwise \c false.

    \sa setFlag(), QTreeWidgetItem::flags(), Qt::ItemFlag
 */
bool QxtTreeWidgetItem::testFlag(Qt::ItemFlag flag) const
{
    return (flags() & flag);
}

/*!
    If \a enabled is \c true, the item \a flag is enabled; otherwise, it is disabled.

    \sa testFlag(), QTreeWidgetItem::setFlags(), Qt::ItemFlag
 */
void QxtTreeWidgetItem::setFlag(Qt::ItemFlag flag, bool enabled)
{
    if (enabled)
        setFlags(flags() | flag);
    else
        setFlags(flags() & ~flag);
}

/*!
    \reimp
 */
void QxtTreeWidgetItem::setData(int column, int role, const QVariant& value)
{
    if (role == Qt::CheckStateRole)
    {
        const Qt::CheckState newState = static_cast<Qt::CheckState>(value.toInt());
        const Qt::CheckState oldState = static_cast<Qt::CheckState>(data(column, role).toInt());

        QTreeWidgetItem::setData(column, role, value);

        if (newState != oldState)
        {
            QxtTreeWidget* tree = qobject_cast<QxtTreeWidget*>(treeWidget());
            if (tree)
            {
                emit tree->itemCheckStateChanged(this);
            }
        }
    }
    else
    {
        QTreeWidgetItem::setData(column, role, value);
    }
}
