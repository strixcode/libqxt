#ifndef QXTCONFIGWIDGET_H
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

#define QXTCONFIGWIDGET_H

#include <QDialog>
#include "qxtglobal.h"

QT_FORWARD_DECLARE_CLASS(QSplitter)
QT_FORWARD_DECLARE_CLASS(QTableWidget)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
class QxtConfigWidgetPrivate;

class QXT_GUI_EXPORT QxtConfigWidget : public QWidget
{
    Q_OBJECT
    QXT_DECLARE_PRIVATE(QxtConfigWidget)
    Q_PROPERTY(int count READ count)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex)
    Q_PROPERTY(bool hoverEffect READ hasHoverEffect WRITE setHoverEffect)
    Q_PROPERTY(QxtConfigWidget::IconPosition iconPosition READ iconPosition WRITE setIconPosition)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
    Q_ENUMS(IconPosition)

public:
    enum IconPosition { North, West, East };

    explicit QxtConfigWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    explicit QxtConfigWidget(QxtConfigWidget::IconPosition position, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~QxtConfigWidget();

    bool hasHoverEffect() const;
    void setHoverEffect(bool enabled);

    QxtConfigWidget::IconPosition iconPosition() const;
    void setIconPosition(QxtConfigWidget::IconPosition position);

    QSize iconSize() const;
    void setIconSize(const QSize& size);

    int addPage(QWidget* page, const QIcon& icon, const QString& title = QString());
    int insertPage(int index, QWidget* page, const QIcon& icon, const QString& title = QString());
    QWidget* takePage(int index);

    int count() const;
    int currentIndex() const;
    QWidget* currentPage() const;

    int indexOf(QWidget* page) const;
    QWidget* page(int index) const;

    bool isPageEnabled(int index) const;
    void setPageEnabled(int index, bool enabled);

    bool isPageHidden(int index) const;
    void setPageHidden(int index, bool hidden);

    QIcon pageIcon(int index) const;
    void setPageIcon(int index, const QIcon& icon);

    QString pageTitle(int index) const;
    void setPageTitle(int index, const QString& title);

    QString pageToolTip(int index) const;
    void setPageToolTip(int index, const QString& tooltip);

    QString pageWhatsThis(int index) const;
    void setPageWhatsThis(int index, const QString& whatsthis);

public Q_SLOTS:
    void setCurrentIndex(int index);
    void setCurrentPage(QWidget* page);

    virtual void accept();
    virtual void reject();

Q_SIGNALS:
    void currentIndexChanged(int index);

protected:
    QSplitter* splitter() const;
    QTableWidget* tableWidget() const;
    QStackedWidget* stackedWidget() const;

    virtual void cleanupPage(int index);
    virtual void initializePage(int index);
};

#endif // QXTCONFIGWIDGET_H
