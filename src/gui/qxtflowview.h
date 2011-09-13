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
**
** This is a derived work of PictureFlow (http://pictureflow.googlecode.com)
**
** The original code was distributed under the following terms:
**
** Copyright (C) 2008 Ariya Hidayat (ariya@kde.org)
** Copyright (C) 2007 Ariya Hidayat (ariya@kde.org)
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
****************************************************************************/

#ifndef QXT_FLOWVIEW_H
#define QXT_FLOWVIEW_H

#include <QWidget>
#include <QAbstractItemModel>
#include "qxtglobal.h"


class QxtFlowViewPrivate;
class QXT_GUI_EXPORT QxtFlowView : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QSize slideSize READ slideSize WRITE setSlideSize)
    Q_PROPERTY(QModelIndex currentIndex READ currentIndex WRITE setCurrentIndex)
    Q_PROPERTY(int pictureRole READ pictureRole WRITE setPictureRole)
    Q_PROPERTY(int pictureColumn READ pictureColumn WRITE setPictureColumn)
    Q_PROPERTY(QModelIndex rootIndex READ rootIndex WRITE setRootIndex)
    Q_PROPERTY(ReflectionEffect reflectionEffect READ reflectionEffect WRITE setReflectionEffect)
    Q_ENUMS(ReflectionEffect)

#if 0
    Q_PROPERTY(int textRole READ textRole WRITE setTextRole)
    Q_PROPERTY(int textColumn READ textColumn WRITE setTextColumn)
#endif


public:

    enum ReflectionEffect
    {
        NoReflection,
        PlainReflection,
        BlurredReflection
    };

    QxtFlowView(QWidget* parent = 0);
    ~QxtFlowView();

    void setModel(QAbstractItemModel * model);
    QAbstractItemModel * model();

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor& c);

    QSize slideSize() const;
    void setSlideSize(QSize size);

    QModelIndex currentIndex() const;

    QModelIndex rootIndex() const;
    void setRootIndex(QModelIndex index);

    ReflectionEffect reflectionEffect() const;
    void setReflectionEffect(ReflectionEffect effect);

    int pictureRole();
    void setPictureRole(int);

    int pictureColumn();
    void setPictureColumn(int);

#if 0
    int textRole();
    void setTextRole(int);
    int textColumn();
    void setTextColumn(int);
#endif

public Q_SLOTS:
    void setCurrentIndex(QModelIndex index);

    void showPrevious();
    void showNext();

    void showSlide(QModelIndex index);

    void render();
    void triggerRender();

Q_SIGNALS:
    void currentIndexChanged(QModelIndex index);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void wheelEvent(QWheelEvent * event);

private Q_SLOTS:
    void updateAnimation();

private:
    QxtFlowViewPrivate* d;
};

#endif // PICTUREFLOW_H
