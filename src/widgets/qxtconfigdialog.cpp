#include "qxtconfigdialog_p.h"
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

#include "qxtconfigdialog.h"
#include "qxtconfigwidget.h"
#include <QDialogButtonBox>
#include <QApplication>
#include <QVBoxLayout>

void QxtConfigDialogPrivate::init( QxtConfigWidget::IconPosition pos )
{
    QxtConfigDialog* p = &qxt_p();
    configWidget = new QxtConfigWidget(pos);
    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, p);
    QObject::connect(buttons, SIGNAL(accepted()), p, SLOT(accept()));
    QObject::connect(buttons, SIGNAL(rejected()), p, SLOT(reject()));
    layout = new QVBoxLayout(p);
    layout->addWidget(configWidget);
    layout->addWidget(buttons);
}

/*!
    \class QxtConfigDialog
    \inmodule QxtWidgets
    \brief The QxtConfigDialog class provides a configuration dialog.

    QxtConfigDialog provides a convenient interface for building
    common configuration dialogs. QxtConfigDialog consists of a
    list of icons and a stack of pages.

    Example usage:
    \code
    QxtConfigDialog dialog;
    dialog.configWidget()->addPage(new ConfigurationPage(&dialog), QIcon(":/images/config.png"), tr("Configuration"));
    dialog.configWidget()->addPage(new UpdatePage(&dialog), QIcon(":/images/update.png"), tr("Update"));
    dialog.configWidget()->addPage(new QueryPage(&dialog), QIcon(":/images/query.png"), tr("Query"));
    dialog.exec();
    \endcode

    \image qxtconfigdialog.png "QxtConfigDialog with page icons on the left (QxtConfigDialog::West)."

    \sa QxtConfigWidget
 */

/*!
    \fn QxtConfigDialog::currentIndexChanged(int index)

    This signal is emitted whenever the current page \a index changes.

    \sa currentIndex()
 */

/*!
    Constructs a new QxtConfigDialog with \a parent and \a flags.
 */
QxtConfigDialog::QxtConfigDialog(QWidget* parent, Qt::WindowFlags flags)
        : QDialog(parent, flags)
{
    QXT_INIT_PRIVATE(QxtConfigDialog);
    qxt_d().init(QxtConfigWidget::West);
}

/*!
    Constructs a new QxtConfigDialog with icon \a position, \a parent and \a flags.
 */
QxtConfigDialog::QxtConfigDialog(QxtConfigWidget::IconPosition position, QWidget* parent, Qt::WindowFlags flags)
        : QDialog(parent, flags)
{
    QXT_INIT_PRIVATE(QxtConfigDialog);
    qxt_d().init(position);
}

/*!
    Destructs the config dialog.
 */
QxtConfigDialog::~QxtConfigDialog()
{
}

/*!
    Returns the dialog button box.

    The default buttons are QDialogButtonBox::Ok and QDialogButtonBox::Cancel.

    \sa setDialogButtonBox()
*/
QDialogButtonBox* QxtConfigDialog::dialogButtonBox() const
{
    return qxt_d().buttons;
}

/*!
    Sets the dialog \a buttonBox. The previous button box
    is deleted if the parent equals \c this.

    \sa dialogButtonBox()
*/
void QxtConfigDialog::setDialogButtonBox(QDialogButtonBox* buttonBox)
{
    if (qxt_d().buttons != buttonBox)
    {
        if (qxt_d().buttons && qxt_d().buttons->parent() == this)
            delete qxt_d().buttons;
        qxt_d().buttons = buttonBox;
        if (qxt_d().buttons)
            qxt_d().layout->addWidget(qxt_d().buttons);
    }
}

/*!
    Returns the config widget.

    \sa setConfigWidget()
*/
QxtConfigWidget* QxtConfigDialog::configWidget() const
{
    return qxt_d().configWidget;
}

/*!
    Sets the \a configWidget. The previous config widget
    is deleted if the parent equals \c this.

    \sa configWidget()
*/
void QxtConfigDialog::setConfigWidget(QxtConfigWidget* configWidget)
{
    if (qxt_d().configWidget != configWidget)
    {
        if (qxt_d().configWidget && qxt_d().configWidget->parent() == this)
            qxt_d().configWidget->deleteLater();
        qxt_d().configWidget = configWidget;
        if (qxt_d().configWidget)
            qxt_d().layout->insertWidget(0, qxt_d().configWidget);
    }
}

/*!
    \bold {Note:} The default implementation calls SLOT(accept()) of
    each page page provided that such slot exists.

    \sa reject()
 */
void QxtConfigDialog::accept()
{
    qxt_d().configWidget->accept();
    QDialog::accept();
}

/*!
    \bold {Note:} The default implementation calls SLOT(reject()) of
    each page provided that such slot exists.

    \sa accept()
 */
void QxtConfigDialog::reject()
{
    qxt_d().configWidget->reject();
    QDialog::reject();
}
