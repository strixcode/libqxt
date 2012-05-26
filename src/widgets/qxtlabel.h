#ifndef QXTLABEL_H
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

#define QXTLABEL_H

#include <QFrame>
#include "qxtnamespace.h"
#include "qxtglobal.h"

class QxtLabelPrivate;

class QXT_GUI_EXPORT QxtLabel : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(Qt::TextElideMode elideMode READ elideMode WRITE setElideMode)
    Q_PROPERTY(Qxt::Rotation rotation READ rotation WRITE setRotation)

public:
    explicit QxtLabel(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    explicit QxtLabel(const QString& text, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~QxtLabel();

    QString text() const;

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment alignment);

    Qt::TextElideMode elideMode() const;
    void setElideMode(Qt::TextElideMode mode);

    Qxt::Rotation rotation() const;
    void setRotation(Qxt::Rotation rotation);

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

public Q_SLOTS:
    void setText(const QString& text);

Q_SIGNALS:
    void clicked();
    void textChanged(const QString& text);

protected:
    virtual void changeEvent(QEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void paintEvent(QPaintEvent* event);

private:
    QXT_DECLARE_PRIVATE(QxtLabel)
};

#endif // QXTLABEL_H
