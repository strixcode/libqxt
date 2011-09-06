#ifndef QXTLINEEDIT_H
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

#define QXTLINEEDIT_H

#include <QLineEdit>
#include <QIcon>
#include <qxtglobal.h>

class QxtLineEditPrivate;
class QActionEvent;
class QResizeEvent;
class QToolButton;

class QXT_GUI_EXPORT QxtLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_ENUMS(ButtonPosition)
    Q_ENUMS(ResetButtonMode)
    Q_PROPERTY(bool buttonAutoRaise READ buttonAutoRaise WRITE setButtonAutoRaise)
    Q_PROPERTY(QIcon buttonIcon READ buttonIcon WRITE setButtonIcon)
    Q_PROPERTY(ButtonPosition buttonPosition READ buttonPosition WRITE setButtonPosition)
    Q_PROPERTY(ResetButtonMode resetButtonMode READ resetButtonMode WRITE setResetButtonMode)
    Q_PROPERTY(QString sampleText READ sampleText WRITE setSampleText)

public:
    enum ButtonPosition
    {
	NoButton,
	PositionLeft = 0x1,
	PositionAuto = 0x2,
	PositionOuter = 0x4,
	InnerRight = 0x10,
	InnerLeft = 0x11,
	InnerAuto = 0x12,
	OuterRight = 0x14,
	OuterLeft = 0x15,
	OuterAuto = 0x16
    };
    enum ResetButtonMode
    {
	HideResetButton,
	ShowResetNotEmpty,
	ShowResetAlways,
    };
    explicit QxtLineEdit(QWidget* parent = 0);
    QxtLineEdit(const QString& text, QWidget* parent = 0);
    virtual ~QxtLineEdit();

    QString sampleText() const;
    bool buttonAutoRaise() const;
    void setButtonAutoRaise(bool);
    QIcon buttonIcon() const;
    void setButtonIcon(const QIcon &);
    ButtonPosition buttonPosition() const;
    void setButtonPosition(ButtonPosition);
    ResetButtonMode resetButtonMode() const;
    void setResetButtonMode(ResetButtonMode);
    QToolButton *button() const;

public Q_SLOTS:
    void setSampleText(const QString& text);

Q_SIGNALS:
    void buttonClicked();

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void actionEvent(QActionEvent *);
    virtual void resizeEvent(QResizeEvent *);

private Q_SLOTS:
    void _qxt_textChanged(const QString &text);

private:
    QXT_DECLARE_PRIVATE(QxtLineEdit)
};

#endif // QXTLINEEDIT_H
