#include "qxtlineedit.h"
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

#include "qxtmetaobject.h"
#include <QStyleOptionFrameV2>
#include <QPainter>
#include <QStyle>
#include <QToolButton>
#include <QActionEvent>
#include <QApplication>

// copied from qlineedit.cpp:
#define vMargin 1
#define hMargin 2

class QxtLineEditPrivate : public QxtPrivate<QxtLineEdit>
{
    QXT_DECLARE_PUBLIC(QxtLineEdit);
public:
    QxtLineEditPrivate() : userBtn(0), resetBtn(0), btnPosn(QxtLineEdit::NoButton), resetMode(QxtLineEdit::HideResetButton) {}

    QString sampleText;
    QToolButton * userBtn; // Internal "user" button
    QToolButton * resetBtn; // Internal "reset" button
    QxtLineEdit::ButtonPosition btnPosn; // Current posn of user button
    QxtLineEdit::ResetButtonMode resetMode; // Current mode of reset button

    /*! \internal
     *	Constructs the user and reset buttons as well as setting the initial
     *	defaults for each. Buttons are initially hidden.
     *	This MUST be called by every QxtLineEdit constructor AFTER
     *	the standard QXT_INIT_PRIVATE().
     */
    void initButtons()
    {
	QXT_P(QxtLineEdit);
	// Construct the user button (hidden)
	userBtn = new QToolButton(&p);
	userBtn->setVisible(false);
	userBtn->setCursor(Qt::ArrowCursor);
	userBtn->setFocusPolicy(Qt::NoFocus);
	userBtn->setIcon(QIcon(":/icons/search.png"));
	QObject::connect(userBtn, SIGNAL(clicked(bool)), &p,
		SIGNAL(buttonClicked()));
	// Construct the reset button (also hidden)
	resetBtn = new QToolButton(&p);
	resetBtn->setVisible(false);
//	resetBtn->setBackgroundRole(QPalette::Base);
	resetBtn->setAutoRaise(true);
	resetBtn->setCursor(Qt::ArrowCursor);
	resetBtn->setFocusPolicy(Qt::NoFocus);
//	resetBtn->setForegroundRole(QPalette::Text);
	resetBtn->setIcon(QIcon(":/icons/reset.png"));
	QObject::connect(resetBtn, SIGNAL(clicked(bool)), &p, SLOT(clear()));
	// Connect a handler for adjusting the reset button visibilty
	p.connect(&p, SIGNAL(textChanged(const QString &)),
		SLOT(_qxt_textChanged(const QString &)));
    }
    /*! \internal
     *  Updates the button's geometry according to the specified position. This
     *  assures it is displayed in the proper location.
     */
    void updateButtonGeom()
    {
	if(btnPosn == QxtLineEdit::NoButton &&
		resetMode == QxtLineEdit::HideResetButton)
	    return;
	QXT_P(QxtLineEdit);
	// Compute the pixel-size (unadjusted) for button area
	int pixUser = std::min(std::max(userBtn->sizeHint().width(),
		    resetBtn->sizeHint().width()),
		p.testAttribute(Qt::WA_Resized) ? p.height() :
		p.sizeHint().height());
	int pixReset = pixUser - 4; // Inside
	// Get the on-screen rectangle (physical geometry) and adjust if needed
	QRect rUser = p.rect();
	QRect rReset = rUser.adjusted(2, 2, -2, -2);
	if(!(btnPosn & QxtLineEdit::PositionOuter)){
	    // Use inside voices
	    rUser = rReset;
	    pixUser = pixReset;
	}
	// Actually set the user button's geometry
	if(btnPosn != QxtLineEdit::NoButton){
	    if(btnPosn & QxtLineEdit::PositionLeft){
		userBtn->setGeometry(rUser.left(), rUser.top(), pixUser,
			rUser.height());
		rReset.adjust(pixUser, 0, 0, 0);
	    }
	    else{
		userBtn->setGeometry(rUser.right()-pixUser+1, rUser.top(),
			pixUser, rUser.height());
		rReset.adjust(0, 0, -pixUser, 0);
	    }
	}
	// And that of the reset button
	if(resetMode != QxtLineEdit::HideResetButton){
	    if(QApplication::isRightToLeft())
		resetBtn->setGeometry(rReset.left(), rReset.top(), pixReset,
			rReset.height());
	    else
		resetBtn->setGeometry(rReset.right()-pixReset+1, rReset.top(),
			pixReset, rReset.height());
	}
    }
    /*! \internal
     *  Adjusts sizes & visibility for changes in button settings.
     *	\param posn New user button position
     *	\param mode New reset button mode
     */
    void adjustButtons(QxtLineEdit::ButtonPosition posn,
	    QxtLineEdit::ResetButtonMode mode)
    {
	QXT_P(QxtLineEdit);
	// Determine visibility
	bool showReset = (mode == QxtLineEdit::ShowResetAlways ||
		(mode == QxtLineEdit::ShowResetNotEmpty &&
		 !qxt_p().text().isEmpty()));
	bool showUser = (posn != QxtLineEdit::NoButton);
	// Get pixels to reserve for button size
	int pix = std::min(std::max(userBtn->sizeHint().width(),
		    resetBtn->sizeHint().width()),
		p.testAttribute(Qt::WA_Resized) ? p.height() :
		p.sizeHint().height());
	// Compute horizontal space needed for reset button
	int rbl = 0, rbr = 0;
	if((resetMode = mode) != QxtLineEdit::HideResetButton){
	    if(QApplication::isRightToLeft())
		rbl = pix; // Reset is on the left
	    else
		rbr = pix; // Reset is on the right
	}
	// Handle user button space requirements (and reset along the way)
	if((btnPosn = posn) == QxtLineEdit::NoButton){
	    // Hide it and unreserve all space
	    p.setContentsMargins(0, 0, 0, 0);
	    p.setTextMargins(rbl, 0, rbr, 0);
	}
	else{
	    // Handle automatic positioning according to locale/language
	    if(posn & QxtLineEdit::PositionAuto){
		if(QApplication::isRightToLeft())
		    btnPosn = static_cast<QxtLineEdit::ButtonPosition>(
			    posn | QxtLineEdit::PositionLeft);
		else
		    btnPosn = static_cast<QxtLineEdit::ButtonPosition>(
			    posn & ~QxtLineEdit::PositionLeft);
	    }
	    // Reserve space appropriately
	    if(btnPosn & QxtLineEdit::PositionOuter){
		// Outside -- text margins ziltch and contents set +2 for frame
		p.setTextMargins(rbl, 0, rbr, 0);
		if(btnPosn & QxtLineEdit::PositionLeft)
		    p.setContentsMargins(pix+2, 0, 0, 0);
		else
		    p.setContentsMargins(0, 0, pix+2, 0);
	    }
	    else{
		// Inside -- contents ziltch and text margins set
		p.setContentsMargins(0, 0, 0, 0);
		if(btnPosn & QxtLineEdit::PositionLeft)
		    p.setTextMargins(pix+rbl, 0, rbr, 0);
		else
		    p.setTextMargins(rbl, 0, pix+rbr, 0);
	    }
	}
	// Update button geometry & redraw everything
	updateButtonGeom();
	if(showUser != userBtn->isVisible())
	    userBtn->setVisible(showUser);
	if(showReset != resetBtn->isVisible())
	    resetBtn->setVisible(showReset);
    }
    /*! \internal
     *  Adjusts visibility for reset button when text changes.
     *	\param text New text content
     */
    void adjustResetButton(const QString &text)
    {
	if(resetMode == QxtLineEdit::ShowResetNotEmpty){
	    if(resetBtn->isVisible() != !text.isEmpty())
		resetBtn->setVisible(!text.isEmpty());
	}
    }
};

/*!
    \class QxtLineEdit
    \inmodule QxtWidgets
    \brief The QxtLineEdit widget is a line edit that is able to show a sample text and embedded buttons.

    QxtLineEdit is a line edit that is able to show a sample text, a user
    configurable button and/or a reset button.
    The sample text is shown when the line edit is empty and has
    no focus.

    \image qxtlineedit.png "Two empty QxtLineEdits: non-focused and focused."
 */

/*!
    Constructs a new QxtLineEdit with \a parent.
 */
QxtLineEdit::QxtLineEdit(QWidget* parent) : QLineEdit(parent)
{
    QXT_INIT_PRIVATE(QxtLineEdit);
    qxt_d().initButtons();
}

/*!
    Constructs a new QxtLineEdit with \a text and \a parent.
 */
QxtLineEdit::QxtLineEdit(const QString& text, QWidget* parent) : QLineEdit(text, parent)
{
    QXT_INIT_PRIVATE(QxtLineEdit);
    qxt_d().initButtons();
}

/*!
    Destructs the line edit.
 */
QxtLineEdit::~QxtLineEdit()
{
}

/*!
    \property QxtLineEdit::sampleText
    \brief The sample text of the line edit

    The sample text is shown when the line edit is empty and has
    no focus.
 */
QString QxtLineEdit::sampleText() const
{
    return qxt_d().sampleText;
}

void QxtLineEdit::setSampleText(const QString& text)
{
    if (qxt_d().sampleText != text)
    {
        qxt_d().sampleText = text;
        if (displayText().isEmpty() && !hasFocus())
            update();
    }
}

/*!
    \property QxtLineEdit::buttonAutoRaise
    \brief The \a autoRaise property of the embedded user button

    This property mirrors the property of the embedded QToolButton and
    causes the button to appear flat until the mouse hovers over it.
 */
bool QxtLineEdit::buttonAutoRaise() const
{
    return qxt_d().userBtn->autoRaise();
}

void QxtLineEdit::setButtonAutoRaise(bool f)
{
    qxt_d().userBtn->setAutoRaise(f);
}

/*!
    \property QxtLineEdit::buttonIcon
    \brief The \a icon property of the embedded user button

    This property mirrors the property of the embedded QToolButton. The
    default icon is a stock search pixmap which will be obtained from
    the system style when available.
 */
QIcon QxtLineEdit::buttonIcon() const
{
    return qxt_d().userBtn->icon();
}

void QxtLineEdit::setButtonIcon(const QIcon &icon)
{
    qxt_d().userBtn->setIcon(icon);
}

/*!
    \property QxtLineEdit::buttonPosition
    \brief The \a position of the embedded user button

    This property controls where the button is positioned within the
    line edit. The default is \a NoButton.

    \sa QxtLineEdit::ButtonPosition
 */
QxtLineEdit::ButtonPosition QxtLineEdit::buttonPosition() const
{
    ButtonPosition result = qxt_d().btnPosn;
    if(result & PositionAuto)
	// Hide real direction when auto
	result = static_cast<ButtonPosition>(result & ~PositionLeft);
    return result;
}

void QxtLineEdit::setButtonPosition(ButtonPosition posn)
{
    if(posn == qxt_d().btnPosn)
	return;
    qxt_d().adjustButtons(posn, qxt_d().resetMode);
    update();
}

/*!
    \property QxtLineEdit::resetButtonMode
    \brief The \a mode of the embedded reset button

    This property controls when the reset button is visible.
    The default is \a HideResetButton.

    \sa QxtLineEdit::ResetButtonMode
 */
QxtLineEdit::ResetButtonMode QxtLineEdit::resetButtonMode() const
{
    return qxt_d().resetMode;
}

void QxtLineEdit::setResetButtonMode(ResetButtonMode mode)
{
    if(mode == qxt_d().resetMode)
	return;
    qxt_d().adjustButtons(qxt_d().btnPosn, mode);
}

/*!
    \brief Provides access to the embedded user button object

    This method returns a pointer to the embedded user button. It
    is never NULL. The QToolButton is owned by the line edit and
    must not be deleted.
 */
QToolButton* QxtLineEdit::button() const
{
    return qxt_d().userBtn;
}

/*!
    \reimp
*/
void QxtLineEdit::paintEvent(QPaintEvent* event)
{
    QLineEdit::paintEvent(event);
    if (displayText().isEmpty() && !hasFocus())
    {
        QStyleOptionFrameV2 option;
        initStyleOption(&option);


        QRect r = style()->subElementRect(QStyle::SE_LineEditContents, &option, this);
#if QT_VERSION >= 0x040500
        // TODO: sort out prior Qt 4.5
        int left, top, right, bottom;
        getTextMargins(&left, &top, &right, &bottom);
        r.adjust(left, top, -right, -bottom);
#endif // QT_VERSION >= 0x040500
        r.adjust(hMargin, vMargin, -hMargin, -vMargin);

        QPainter painter(this);
        QPalette pal = palette();
        pal.setCurrentColorGroup(QPalette::Disabled);
        style()->drawItemText(&painter, r, alignment(), pal, false, qxt_d().sampleText, QPalette::Text);
    }
}

/*!
 * \reimp
 */
void QxtLineEdit::actionEvent(QActionEvent *e)
{
    // Transfer action events to the button since that's where they typically
    // belong. All others are passed on to the base class implementation.
    switch(e->type()){
    case QEvent::ActionAdded:
	qxt_d().userBtn->insertAction(e->before(), e->action());
	break;
    case QEvent::ActionRemoved:
	qxt_d().userBtn->removeAction(e->action());
	break;
    default:
	QLineEdit::actionEvent(e);
    }
}

/*!
 * \reimp
 */
void QxtLineEdit::resizeEvent(QResizeEvent *e)
{
    // Adjust the button's geometry when resizing events occur so it doesn't
    // end up out of place. The base implementation is always used as well.
    qxt_d().updateButtonGeom();
    QLineEdit::resizeEvent(e);
}

/*! \internal
 *  Forwards signal to private class
 */
void QxtLineEdit::_qxt_textChanged(const QString &text)
{
    qxt_d().adjustResetButton(text);
}

/*!
    \enum QxtLineEdit::ButtonPosition
    \brief This enum controls the position of an embedded button.

    \value NoButton The button is not displayed (default)
    \value InnerRight Button is shown inside the frame on the right-hand side
    \value InnerLeft Button is shown inside the frame on the left-hand side
    \value InnerAuto Button is shown inside the frame, side determined by locale
    \value OuterRight Button is shown outside the frame on the right-hand side
    \value OuterLeft Button is shown outside the frame on the left-hand side
    \value OuterAuto Button is shown outside the frame, side determined by locale
*/
/*!
    \enum QxtLineEdit::ResetButtonMode
    \brief This enum controls the display of a reset button.

    \value HideResetButton The button is not displayed (default)
    \value ShowResetNotEmpty The reset button appears only when the line-edit is not empty
    \value ShowResetAlways The reset button is always visible
*/
/*!
    \fn buttonClicked()

    This signal is emitted when the user button is clicked.

    \sa QToolButton::clicked(bool)
*/
