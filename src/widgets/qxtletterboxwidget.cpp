#include "qxtletterboxwidget.h"
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

#include "qxtletterboxwidget_p.h"
#include <QLayout>
#include <QStyle>

QxtLetterBoxWidgetPrivate::QxtLetterBoxWidgetPrivate() : center(0), timer(), margin(0)
{
    timer.setSingleShot(true);
}

/*!
    \class QxtLetterBoxWidget
    \inmodule QxtWidgets
    \brief The QxtLetterBoxWidget widget is a letter box widget.

    QxtLetterBoxWidget preserves the aspect ratio of its content widget.

    Use QWidget::sizeIncrement to define the aspect ratio:
    \code
    QxtLetterBoxWidget letterBox;
    widget->setSizeIncrement(16, 9);
    letterBox.setWidget(widget);
    letterBox.show();
    \endcode
 */

/*!
    Constructs a new QxtLetterBoxWidget with \a parent.
 */
QxtLetterBoxWidget::QxtLetterBoxWidget(QWidget* parent) : QFrame(parent)
{
    QXT_INIT_PRIVATE(QxtLetterBoxWidget);
    connect(&qxt_d().timer, SIGNAL(timeout()), this, SLOT(resizeWidget()));
}

/*!
    Destructs the group box.
 */
QxtLetterBoxWidget::~QxtLetterBoxWidget()
{
}

/*!
    \property QxtLetterBoxWidget::backgroundColor
    \brief the background color

    This property is provided for convenience.
    This property corresponds to QPalette::Window.

    \bold {Note:} Setting or clearing the property also sets the property
    QWidget::autoFillBackground as \c true or \c false, respectively.
 */
QColor QxtLetterBoxWidget::backgroundColor() const
{
    return palette().color(QPalette::Window);
}

void QxtLetterBoxWidget::setBackgroundColor(const QColor& color)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, color);
    setPalette(pal);
    setAutoFillBackground(true);
}

void QxtLetterBoxWidget::clearBackgroundColor()
{
    setBackgroundColor(QColor());
    setAutoFillBackground(false);
}

/*!
    \property QxtLetterBoxWidget::margin
    \brief the margin

    The default value is \c 0.
 */
int QxtLetterBoxWidget::margin() const
{
    return qxt_d().margin;
}

void QxtLetterBoxWidget::setMargin(int margin)
{
    if (qxt_d().margin != margin)
    {
        qxt_d().margin = margin;
        resizeWidget();
    }
}

/*!
    Returns the widget for the letter box.
    This function returns zero if the widget has not been set.

    \sa setWidget()
 */
QWidget* QxtLetterBoxWidget::widget() const
{
    return qxt_d().center;
}

/*!
    Sets the given \a widget to be the letter box's widget.

    \bold {Note:} QxtLetterBox takes ownership of the widget pointer
    and deletes it at the appropriate time.

    \sa widget()
 */
void QxtLetterBoxWidget::setWidget(QWidget* widget)
{
    if (qxt_d().center && qxt_d().center != widget)
        qxt_d().center->deleteLater();
    qxt_d().center = widget;
    if (widget)
    {
        widget->setParent(this);
        resizeWidget();
    }
}

/*!
    \property QxtLetterBoxWidget::resizeDelay
    \brief the delay of resize

    The default value is \c 0 which means immediate resize.

    Using a short resize delay might be useful if the
    widget is complex and resizing it is expensive.
 */
uint QxtLetterBoxWidget::resizeDelay() const
{
    return qxt_d().timer.interval();
}

void QxtLetterBoxWidget::setResizeDelay(uint delay)
{
    qxt_d().timer.setInterval(delay);
}

/*!
    Resizes the content widget.
 */
void QxtLetterBoxWidget::resizeWidget()
{
    if (qxt_d().center)
    {
        QSize s = qxt_d().center->sizeIncrement();
        s.scale(size(), Qt::KeepAspectRatio);
        s -= QSize(2 * qxt_d().margin, 2 * qxt_d().margin);
        s = QLayout::closestAcceptableSize(qxt_d().center, s);
        QRect r = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, s, rect());
        qxt_d().center->setGeometry(r);
    }
}

/*!
    \reimp
 */
void QxtLetterBoxWidget::resizeEvent(QResizeEvent* event)
{
    QFrame::resizeEvent(event);
    if (resizeDelay() > 0)
        qxt_d().timer.start();
    else
        resizeWidget();
}
