#ifndef QXTTOOLTIP_H
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

#define QXTTOOLTIP_H

#include <QRect>
#include <QPointer>
#include <QWidget>
#include "qxtglobal.h"

class QXT_GUI_EXPORT QxtToolTip
{
    QxtToolTip();

public:
    static void show(const QPoint& pos, QWidget* tooltip, QWidget* parent = 0, const QRect& rect = QRect());
    static void hide();

    static QWidget* toolTip(QWidget* parent);
    static void setToolTip(QWidget* parent, QWidget* tooltip, const QRect& rect = QRect());

    static QRect toolTipRect(QWidget* parent);
    static void setToolTipRect(QWidget* parent, const QRect& rect);

    static int margin();
    static void setMargin(int margin);

    static qreal opacity();
    static void setOpacity(qreal level);
};

QT_BEGIN_NAMESPACE
inline uint qHash(const QPointer<QWidget> key)
{
    return reinterpret_cast<quint64>(key ? (&*key) : 0);
}
QT_END_NAMESPACE

#endif // QXTTOOLTIP_H
