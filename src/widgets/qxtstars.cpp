#include "qxtstars.h"
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


#include <QStyleOptionSlider>
#include <QPainterPath>
#include <QMouseEvent>
#include <QPainter>

class QxtStarsPrivate : public QxtPrivate<QxtStars>
{
public:
    QXT_DECLARE_PUBLIC(QxtStars)
    QxtStarsPrivate();
    int pixelPosToRangeValue(int pos) const;
    inline int pick(const QPoint& pt) const;
    QStyleOptionSlider getStyleOption() const;
    QSize getStarSize() const;
    int snapBackPosition;
    bool readOnly;
    QSize starSize;
    QPainterPath star;
};

QxtStarsPrivate::QxtStarsPrivate()
        : snapBackPosition(0), readOnly(false)
{
    star.moveTo(14.285716, -43.352104);
    star.lineTo(38.404536, 9.1654726);
    star.lineTo(95.804846, 15.875014);
    star.lineTo(53.310787, 55.042197);
    star.lineTo(64.667306, 111.7065);
    star.lineTo(14.285714, 83.395573);
    star.lineTo(-36.095881, 111.7065);
    star.lineTo(-24.739359, 55.042198);
    star.lineTo(-67.233416, 15.875009);
    star.lineTo(-9.8331075, 9.1654728);
    star.closeSubpath();
}

int QxtStarsPrivate::pixelPosToRangeValue(int pos) const
{
    const QxtStars* p = &qxt_p();
    QStyleOptionSlider opt = getStyleOption();
    QRect gr = p->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, p);
    QRect sr = p->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, p);
    int sliderMin, sliderMax, sliderLength;

    gr.setSize(qxt_p().sizeHint());
    if (p->orientation() == Qt::Horizontal)
    {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    }
    else
    {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }
    return QStyle::sliderValueFromPosition(p->minimum(), p->maximum(), pos - sliderMin,
                                           sliderMax - sliderMin, opt.upsideDown);
}

inline int QxtStarsPrivate::pick(const QPoint& pt) const
{
    return qxt_p().orientation() == Qt::Horizontal ? pt.x() : pt.y();
}

// TODO: get rid of this in Qt 4.3
QStyleOptionSlider QxtStarsPrivate::getStyleOption() const
{
    const QxtStars* p = &qxt_p();
    QStyleOptionSlider opt;
    opt.initFrom(p);
    opt.subControls = QStyle::SC_None;
    opt.activeSubControls = QStyle::SC_None;
    opt.orientation = p->orientation();
    opt.maximum = p->maximum();
    opt.minimum = p->minimum();
    opt.upsideDown = (p->orientation() == Qt::Horizontal) ?
                     (p->invertedAppearance() != (opt.direction == Qt::RightToLeft)) : (!p->invertedAppearance());
    opt.direction = Qt::LeftToRight; // we use the upsideDown option instead
    opt.sliderPosition = p->sliderPosition();
    opt.sliderValue = p->value();
    opt.singleStep = p->singleStep();
    opt.pageStep = p->pageStep();
    if (p->orientation() == Qt::Horizontal)
        opt.state |= QStyle::State_Horizontal;
    return opt;
}

QSize QxtStarsPrivate::getStarSize() const
{
    QSize size = starSize;
    if (!size.isValid() || size.isNull())
    {
        const int width = qxt_p().style()->pixelMetric(QStyle::PM_SmallIconSize);
        size = QSize(width, width);
    }
    return size;
}

/*!
    \class QxtStars
    \inmodule QxtWidgets
    \brief The QxtStars widget is a stars assessment widget.

    QxtStars is an optionally interactive stars assessment widget.

    By default, orientation is Qt::Horizontal and range is from \c 0 to \c 5.

    The stars are rendered accoring to palette with the following color roles:
    \table
    \header \o ColorRole \o Element
    \row \o QPalette::Text \o outlines
    \row \o QPalette::Base \o unselected stars
    \row \o QPalette::Highlight \o selected stars
    \endtable

    \image qxtstars.png "QxtStars in action."
 */

/*!
    Constructs a new QxtStars with \a parent.
 */
QxtStars::QxtStars(QWidget* parent) : QAbstractSlider(parent)
{
    QXT_INIT_PRIVATE(QxtStars);
    setOrientation(Qt::Horizontal);
    setFocusPolicy(Qt::FocusPolicy(style()->styleHint(QStyle::SH_Button_FocusPolicy)));
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setRange(0, 5);
}

/*!
    Destructs the stars.
 */
QxtStars::~QxtStars()
{}

/*!
    \property QxtStars::readOnly
    \brief whether stars are adjustable

    In read-only mode, the user is not able to change the value.
    The default value is \c false.
 */
bool QxtStars::isReadOnly() const
{
    return qxt_d().readOnly;
}

void QxtStars::setReadOnly(bool readOnly)
{
    if (qxt_d().readOnly != readOnly)
    {
        qxt_d().readOnly = readOnly;
        if (readOnly)
            setFocusPolicy(Qt::NoFocus);
        else
            setFocusPolicy(Qt::FocusPolicy(style()->styleHint(QStyle::SH_Button_FocusPolicy)));
    }
}

/*!
    \property QxtStars::starSize
    \brief the size of star icons

    The default value is QStyle::PM_SmallIconSize.
 */
QSize QxtStars::starSize() const
{
    return qxt_d().starSize;
}

void QxtStars::setStarSize(const QSize& size)
{
    if (qxt_d().starSize != size)
    {
        qxt_d().starSize = size;
        updateGeometry();
        update();
    }
}

/*!
    \reimp
 */
QSize QxtStars::sizeHint() const
{
    return minimumSizeHint();
}

/*!
    \reimp
 */
QSize QxtStars::minimumSizeHint() const
{
    QSize size = qxt_d().getStarSize();
    size.rwidth() *= maximum() - minimum();
    if (orientation() == Qt::Vertical)
        size.transpose();
    return size;
}

/*!
    \reimp
 */
void QxtStars::paintEvent(QPaintEvent* event)
{
    QAbstractSlider::paintEvent(event);

    QPainter painter(this);
    painter.save();
    painter.setPen(palette().color(QPalette::Text));
    painter.setRenderHint(QPainter::Antialiasing);

    const bool invert = invertedAppearance();
    const QSize size = qxt_d().getStarSize();
    const QRectF star = qxt_d().star.boundingRect();
    painter.scale(size.width() / star.width(), size.height() / star.height());
    const int count = maximum() - minimum();
    if (orientation() == Qt::Horizontal)
    {
        painter.translate(-star.x(), -star.y());
        if (invert != isRightToLeft())
            painter.translate((count - 1) * star.width(), 0);
    }
    else
    {
        painter.translate(-star.x(), -star.y());
        if (!invert)
            painter.translate(0, (count - 1) * star.height());
    }
    for (int i = 0; i < count; ++i)
    {
        if (value() > minimum() + i)
            painter.setBrush(palette().highlight());
        else
            painter.setBrush(palette().base());
        painter.drawPath(qxt_d().star);

        if (orientation() == Qt::Horizontal)
            painter.translate(invert != isRightToLeft() ? -star.width() : star.width(), 0);
        else
            painter.translate(0, invert ? star.height() : -star.height());
    }

    painter.restore();
    if (hasFocus())
    {
        QStyleOptionFocusRect opt;
        opt.initFrom(this);
        opt.rect.setSize(sizeHint());
        style()->drawPrimitive(QStyle::PE_FrameFocusRect, &opt, &painter, this);
    }
}

/*!
    \reimp
 */
void QxtStars::keyPressEvent(QKeyEvent* event)
{
    if (qxt_d().readOnly)
    {
        event->ignore();
        return;
    }
    QAbstractSlider::keyPressEvent(event);
}

/*!
    \reimp
 */
void QxtStars::mousePressEvent(QMouseEvent* event)
{
    if (qxt_d().readOnly)
    {
        event->ignore();
        return;
    }
    QAbstractSlider::mousePressEvent(event);

    if (maximum() == minimum() || (event->buttons() ^ event->button()))
    {
        event->ignore();
        return;
    }

    event->accept();
    QStyleOptionSlider opt = qxt_d().getStyleOption();
    const QRect sliderRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    const QPoint center = sliderRect.center() - sliderRect.topLeft();
    const int pos = qxt_d().pixelPosToRangeValue(qxt_d().pick(event->pos() - center));
    setSliderPosition(pos);
    triggerAction(SliderMove);
    setRepeatAction(SliderNoAction);
    qxt_d().snapBackPosition = pos;
    update();
}

/*!
    \reimp
 */
void QxtStars::mouseMoveEvent(QMouseEvent* event)
{
    if (qxt_d().readOnly)
    {
        event->ignore();
        return;
    }
    QAbstractSlider::mouseMoveEvent(event);

    event->accept();
    int newPosition = qxt_d().pixelPosToRangeValue(qxt_d().pick(event->pos()));
    QStyleOptionSlider opt = qxt_d().getStyleOption();
    int m = style()->pixelMetric(QStyle::PM_MaximumDragDistance, &opt, this);
    if (m >= 0)
    {
        QRect r = rect();
        r.adjust(-m, -m, m, m);
        if (!r.contains(event->pos()))
            newPosition = qxt_d().snapBackPosition;
    }
    setSliderPosition(newPosition);
}

/*!
    \reimp
 */
void QxtStars::mouseReleaseEvent(QMouseEvent* event)
{
    if (qxt_d().readOnly)
    {
        event->ignore();
        return;
    }
    QAbstractSlider::mouseReleaseEvent(event);

    if (event->buttons())
    {
        event->ignore();
        return;
    }

    event->accept();
    setRepeatAction(SliderNoAction);
}
