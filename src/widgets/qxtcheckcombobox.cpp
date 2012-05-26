#include "qxtcheckcombobox.h"
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

#include "qxtcheckcombobox_p.h"
#include <QLineEdit>
#include <QKeyEvent>

QxtCheckComboBoxPrivate::QxtCheckComboBoxPrivate() : containerMousePress(false)
{
    separator = QLatin1String(",");
}

bool QxtCheckComboBoxPrivate::eventFilter(QObject* receiver, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (receiver == &qxt_p() && (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Down))
        {
            qxt_p().showPopup();
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Enter ||
                 keyEvent->key() == Qt::Key_Return ||
                 keyEvent->key() == Qt::Key_Escape)
        {
            // it is important to call QComboBox implementation
            qxt_p().QComboBox::hidePopup();
            if (keyEvent->key() != Qt::Key_Escape)
                return true;
        }
    }
    case QEvent::MouseButtonPress:
        containerMousePress = (receiver == qxt_p().view()->window());
        break;
    case QEvent::MouseButtonRelease:
        containerMousePress = false;;
        break;
    default:
        break;
    }
    return false;
}

void QxtCheckComboBoxPrivate::updateCheckedItems()
{
    QStringList items = qxt_p().checkedItems();
    if (items.isEmpty())
        qxt_p().setEditText(defaultText);
    else
        qxt_p().setEditText(items.join(separator));

    // TODO: find a way to recalculate a meaningful size hint

    emit qxt_p().checkedItemsChanged(items);
}

void QxtCheckComboBoxPrivate::toggleCheckState(int index)
{
    QVariant value = qxt_p().itemData(index, Qt::CheckStateRole);
    if (value.isValid())
    {
        Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
        qxt_p().setItemData(index, (state == Qt::Unchecked ? Qt::Checked : Qt::Unchecked), Qt::CheckStateRole);
    }
}

QxtCheckComboModel::QxtCheckComboModel(QObject* parent)
        : QStandardItemModel(0, 1, parent) // rows,cols
{
}

Qt::ItemFlags QxtCheckComboModel::flags(const QModelIndex& index) const
{
    return QStandardItemModel::flags(index) | Qt::ItemIsUserCheckable;
}

QVariant QxtCheckComboModel::data(const QModelIndex& index, int role) const
{
    QVariant value = QStandardItemModel::data(index, role);
    if (index.isValid() && role == Qt::CheckStateRole && !value.isValid())
        value = Qt::Unchecked;
    return value;
}

bool QxtCheckComboModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    bool ok = QStandardItemModel::setData(index, value, role);
    if (ok && role == Qt::CheckStateRole)
    {
        emit dataChanged(index, index);
        emit checkStateChanged();
    }
    return ok;
}

/*!
    \class QxtCheckComboBox
    \inmodule QxtWidgets
    \brief The QxtCheckComboBox widget is an extended QComboBox with checkable items.

    QxtComboBox is a specialized combo box with checkable items.
    Checked items are collected together in the line edit.

    \code
    QxtCheckComboBox* comboBox = new QxtCheckComboBox(this);
    comboBox->addItems(...);
    comboBox->setItemCheckState(2, Qt::Checked);
    comboBox->setItemCheckState(4, Qt::Checked);
    // OR
    comboBox->setCheckedItems(QStringList() << "dolor" << "amet");
    \endcode

    \image qxtcheckcombobox.png "QxtCheckComboBox in Plastique style."
 */

/*!
    \fn QxtCheckComboBox::checkedItemsChanged(const QStringList& items)

    This signal is emitted whenever the checked \a items have been changed.
 */

/*!
    Constructs a new QxtCheckComboBox with \a parent.
 */
QxtCheckComboBox::QxtCheckComboBox(QWidget* parent) : QComboBox(parent)
{
    QXT_INIT_PRIVATE(QxtCheckComboBox);
    setModel(new QxtCheckComboModel(this));
    connect(this, SIGNAL(activated(int)), &qxt_d(), SLOT(toggleCheckState(int)));
    connect(model(), SIGNAL(checkStateChanged()), &qxt_d(), SLOT(updateCheckedItems()));
    connect(model(), SIGNAL(rowsInserted(const QModelIndex &, int, int)), &qxt_d(), SLOT(updateCheckedItems()));
    connect(model(), SIGNAL(rowsRemoved(const QModelIndex &, int, int)), &qxt_d(), SLOT(updateCheckedItems()));

    // read-only contents
    QLineEdit* lineEdit = new QLineEdit(this);
    lineEdit->setReadOnly(true);
    setLineEdit(lineEdit);
    lineEdit->disconnect(this);
    setInsertPolicy(QComboBox::NoInsert);

    view()->installEventFilter(&qxt_d());
    view()->window()->installEventFilter(&qxt_d());
    view()->viewport()->installEventFilter(&qxt_d());
    this->installEventFilter(&qxt_d());
}

/*!
    Destructs the combo box.
 */
QxtCheckComboBox::~QxtCheckComboBox()
{
}

/*!
    \reimp
 */
void QxtCheckComboBox::hidePopup()
{
    if (qxt_d().containerMousePress)
        QComboBox::hidePopup();
}

/*!
    Returns the check state of the item at \a index.
 */
Qt::CheckState QxtCheckComboBox::itemCheckState(int index) const
{
    return static_cast<Qt::CheckState>(itemData(index, Qt::CheckStateRole).toInt());
}

/*!
    Sets the check \a state of the item at \a index.
 */
void QxtCheckComboBox::setItemCheckState(int index, Qt::CheckState state)
{
    setItemData(index, state, Qt::CheckStateRole);
}

/*!
    \property QxtCheckComboBox::checkedItems
    \brief the checked items.
 */
QStringList QxtCheckComboBox::checkedItems() const
{
    QStringList items;
    if (model())
    {
        QModelIndex index = model()->index(0, modelColumn(), rootModelIndex());
        QModelIndexList indexes = model()->match(index, Qt::CheckStateRole, Qt::Checked, -1, Qt::MatchExactly);
        foreach(const QModelIndex& index, indexes)
        items += index.data().toString();
    }
    return items;
}

void QxtCheckComboBox::setCheckedItems(const QStringList& items)
{
    // not the most efficient solution but most likely nobody
    // will put too many items into a combo box anyway so...
    foreach(const QString& text, items)
    {
        const int index = findText(text);
        setItemCheckState(index, index != -1 ? Qt::Checked : Qt::Unchecked);
    }
}

/*!
    \property QxtCheckComboBox::defaultText
    \brief the default text.

    The default text is shown when there are no checked items.
    The default value is an empty string.
 */
QString QxtCheckComboBox::defaultText() const
{
    return qxt_d().defaultText;
}

void QxtCheckComboBox::setDefaultText(const QString& text)
{
    if (qxt_d().defaultText != text)
    {
        qxt_d().defaultText = text;
        qxt_d().updateCheckedItems();
    }
}

/*!
    \property QxtCheckComboBox::separator
    \brief the default separator.

    The checked items are joined together with the separator string.
    The default value is a comma (",").
 */
QString QxtCheckComboBox::separator() const
{
    return qxt_d().separator;
}

void QxtCheckComboBox::setSeparator(const QString& separator)
{
    if (qxt_d().separator != separator)
    {
        qxt_d().separator = separator;
        qxt_d().updateCheckedItems();
    }
}
