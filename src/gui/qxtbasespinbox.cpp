#include "qxtbasespinbox.h"
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


class QxtBaseSpinBoxPrivate : public QxtPrivate<QxtBaseSpinBox>
{
public:
    QXT_DECLARE_PUBLIC(QxtBaseSpinBox)
    QxtBaseSpinBoxPrivate();

    int base;
    bool upper;
};

QxtBaseSpinBoxPrivate::QxtBaseSpinBoxPrivate() : base(10), upper(false)
{
}

/*!
    \class QxtBaseSpinBox
    \inmodule QxtGui
    \brief The QxtBaseSpinBox widget is a spin box with support for numbers
    in base between \c 2 and \c 36.

    Example spinbox for hexadecimal input:
    \code
    QxtBaseSpinBox* spinBox = new QxtBaseSpinBox(16, this);
    spinBox->setPrefix("0x");
    spinBox->setValue(0xbabe);
    spinBox->setUpperCase(true);
    \endcode

    \image qxtbasespinbox.png "QxtBaseSpinBox in action."

    \bold {Note:} Notice that QxtBaseSpinBox is not locale-aware.
 */

/*!
    \fn QxtBaseSpinBox::baseChanged(int base)

    This signal is emitted whenever the number \a base has changed.
 */

/*!
    Constructs a new QxtBaseSpinBox with \a parent. Base defaults to \c 10.
 */
QxtBaseSpinBox::QxtBaseSpinBox(QWidget* parent) : QSpinBox(parent)
{
    QXT_INIT_PRIVATE(QxtBaseSpinBox);
}

/*!
    Constructs a new QxtBaseSpinBox with \a base and \a parent.
 */
QxtBaseSpinBox::QxtBaseSpinBox(int base, QWidget* parent) : QSpinBox(parent)
{
    QXT_INIT_PRIVATE(QxtBaseSpinBox);
    qxt_d().base = base;
}

/*!
    Destructs the spin box.
 */
QxtBaseSpinBox::~QxtBaseSpinBox()
{
}

/*!
    \reimp
 */
void QxtBaseSpinBox::fixup(QString& input) const
{
    QString inputWithoutPrefix = input.mid(prefix().length());
    inputWithoutPrefix = qxt_d().upper ? inputWithoutPrefix.toUpper() : inputWithoutPrefix.toLower();
    input = prefix() + inputWithoutPrefix;
}

/*!
    \reimp
 */
QValidator::State QxtBaseSpinBox::validate(QString& input, int& pos) const
{
    // quick rejects
    const QString prefix = QSpinBox::prefix();
    const QString inputWithoutPrefix = input.mid(prefix.length());
    if (pos < prefix.length())
    {
        // do not let modify prefix
        return QValidator::Invalid;
    }
    else if (inputWithoutPrefix.isEmpty())
    {
        // allow empty input => intermediate
        return QValidator::Intermediate;
    }

    // Invalid:      input is invalid according to the string list
    // Intermediate: it is likely that a little more editing will make the input acceptable
    // Acceptable:   the input is valid.
    Q_UNUSED(pos);

    bool ok = false;
    const int min = minimum();
    const int max = maximum();
    const int number = inputWithoutPrefix.toInt(&ok, qxt_d().base);

    QValidator::State state = QValidator::Invalid;
    if (!ok)
    {
        // cannot convert => invalid
        state = QValidator::Invalid;
    }
    else if (number >= min && number <= max)
    {
        // converts ok, between boundaries => acceptable if case matches
        if (qxt_d().upper)
            return (input == prefix + inputWithoutPrefix.toUpper() ? QValidator::Acceptable : QValidator::Intermediate);
        else
            return (input == prefix + inputWithoutPrefix.toLower() ? QValidator::Acceptable : QValidator::Intermediate);
    }
    else
    {
        // converts ok, outside boundaries => intermediate
        state = QValidator::Intermediate;
    }
    return state;
}

/*!
    \property QxtBaseSpinBox::base
    \brief the number base.

    The base must be between \c 2 and \c 36.

    The default value is \c 10.
 */
int QxtBaseSpinBox::base() const
{
    return qxt_d().base;
}

void QxtBaseSpinBox::setBase(int base)
{
    if (base < 2 || base > 36)
        qWarning("QxtBaseSpinBox: base must be between 2 and 36");

    base = qBound(2, base, 36);
    if (qxt_d().base != base)
    {
        qxt_d().base = base;
        emit baseChanged(base);
        setValue(value());
    }
}

/*!
    \property QxtBaseSpinBox::upperCase
    \brief whether letters are shown in upper case.

    Naturally, this applies to only bases which can contain letters.

    The default value is \c false.
 */
bool QxtBaseSpinBox::isUpperCase() const
{
    return qxt_d().upper;
}

void QxtBaseSpinBox::setUpperCase(bool upperCase)
{
    if (qxt_d().upper != upperCase)
    {
        qxt_d().upper = upperCase;
        setValue(value());
    }
}

/*!
    \reimp
 */
QString QxtBaseSpinBox::textFromValue(int value) const
{
    QString text = QString::number(value, qxt_d().base);
    if (qxt_d().upper)
        return text.toUpper();
    return text;
}

/*!
    \reimp
 */
int QxtBaseSpinBox::valueFromText(const QString& text) const
{
    return text.toInt(0, qxt_d().base);
}
