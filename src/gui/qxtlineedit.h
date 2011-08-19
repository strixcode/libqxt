/****************************************************************************
 **
 ** Copyright (C) Qxt Foundation. Some rights reserved.
 **
 ** This file is part of the QxtGui module of the Qxt library.
 **
 ** This library is free software; you can redistribute it and/or modify it
 ** under the terms of the Common Public License, version 1.0, as published
 ** by IBM, and/or under the terms of the GNU Lesser General Public License,
 ** version 2.1, as published by the Free Software Foundation.
 **
 ** This file is provided "AS IS", without WARRANTIES OR CONDITIONS OF ANY
 ** KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
 ** WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY OR
 ** FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** You should have received a copy of the CPL and the LGPL along with this
 ** file. See the LICENSE file and the cpl1.0.txt/lgpl-2.1.txt files
 ** included with the source distribution for more information.
 ** If you did not receive a copy of the licenses, contact the Qxt Foundation.
 **
 ** <http://libqxt.org>  <foundation@libqxt.org>
 **
 ****************************************************************************/
#ifndef QXTLINEEDIT_H
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
