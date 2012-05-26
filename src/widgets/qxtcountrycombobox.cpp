
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

#include "qxtcountrycombobox.h"
#include "qxtcountrycombobox_p.h"
#include "qxtcountrymodel.h"
#include <QDebug>


QxtCountryComboBoxPrivate::QxtCountryComboBoxPrivate()
{
}

void QxtCountryComboBoxPrivate::comboBoxCurrentIndexChanged(int)
{
    emit qxt_p().currentCountryChanged(currentCountry());
    emit qxt_p().currentCountryNameChanged(currentCountryName());
}

QLocale::Country QxtCountryComboBoxPrivate::currentCountry() const
{
    QModelIndex idx = qxt_p().model()->index(qxt_p().currentIndex(), 2);
    return static_cast<QLocale::Country>(idx.data().toInt());
}

QString QxtCountryComboBoxPrivate::currentCountryName() const
{
    return qxt_p().currentText();
}

void QxtCountryComboBoxPrivate::setCurrentCountry(QLocale::Country country)
{
    // column 3 is QLocale::Country
    QModelIndex start = qxt_p().model()->index(0, 2);
    QModelIndexList result = qxt_p().model()->match(start, Qt::DisplayRole, country, 1, Qt::MatchExactly);
    if (!result.isEmpty())
        qxt_p().setCurrentIndex(result.first().row());
}

/*!
    \class QxtCountryComboBox
    \inmodule QxtWidgets
    \brief The QxtCountryComboBox widget is an extended QComboBox to display countries.

    QxtCountryComboBox is a specialized combo box to display countries.
    The languages are taken from QLocale::Country.

    \image qxtcountrycombobox.png "QxtCountryComboBox in Plastique style."
 */

/*!
    \fn QxtCountryComboBox::currentCountryChanged(QLocale::Country country)

    This signal is emitted whenever the current \a country has been changed.
 */

/*!
    \fn QxtCountryComboBox::currentCountryNameChanged(const QString& name)

    This signal is emitted whenever the current country \a name has been changed.
 */

/*!
    Constructs a new QxtCountryComboBox with \a parent.
 */
QxtCountryComboBox::QxtCountryComboBox(QWidget* parent)
        : QComboBox(parent)
{
    QXT_INIT_PRIVATE(QxtCountryComboBox);
#ifdef QXT_DESIGNER_MODE
    setEditable(false);
#else
    setModel(new QxtCountryModel(this));
    setModelColumn(0);

    setCurrentCountry(QLocale::system().country());
#endif

    connect(this, SIGNAL(currentIndexChanged(int)), &qxt_d(), SLOT(comboBoxCurrentIndexChanged(int)));
}

#ifdef QXT_DESIGNER_MODE
void QxtCountryComboBox::paintEvent(QPaintEvent *e)
{
    QComboBox::paintEvent(e);
    QStylePainter painter(this);
    painter.drawItemText(e->rect(), Qt::AlignCenter, palette(), true, tr("DESIGNER MODE  -  DESIGNER MODE"));
}
#endif
/*!
    Destructs the country combo box.
 */
QxtCountryComboBox::~QxtCountryComboBox()
{
}

/*!
    \property QxtCountryComboBox::currentCountry
    \brief the current country.
 */
QLocale::Country QxtCountryComboBox::currentCountry() const
{
    return qxt_d().currentCountry();
}

/*!
    \property QxtCountryComboBox::currentCountryName
    \brief the name of the current country.
 */
QString QxtCountryComboBox::currentCountryName() const
{
    return qxt_d().currentCountryName();
}

void QxtCountryComboBox::setCurrentCountry(QLocale::Country country)
{
    return qxt_d().setCurrentCountry(country);
}

