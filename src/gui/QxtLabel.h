/****************************************************************************
**
** Copyright (C) Qxt Foundation. Some rights reserved.
**
** This file is part of the QxtCore module of the Qt eXTension library
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or any later version.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** There is aditional information in the LICENSE file of libqxt.
** If you did not receive a copy of the file try to download it or
** contact the libqxt Management
** 
** <http://libqxt.sourceforge.net>  <aep@exys.org>  <coda@bobandgeorge.com>
**
****************************************************************************/


#ifndef QXTLABEL_H
#define QXTLABEL_H

#include <QFrame>
#include <Qxt/qxtglobal.h>
#include <Qxt/qxtnamespace.h>
#include <QxtPimpl.h>

class QxtLabelPrivate;

class QXT_GUI_EXPORT QxtLabel : public QFrame
{
	Q_OBJECT
	QXT_DECLARE_PRIVATE(QxtLabel);
	Q_PROPERTY(QString text READ text WRITE setText)
	Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
	Q_PROPERTY(Qt::TextElideMode elideMode READ elideMode WRITE setElideMode)
	Q_PROPERTY(Qxt::Rotation rotation READ rotation WRITE setRotation)
	
public:
	explicit QxtLabel(QWidget* parent = 0, Qt::WindowFlags flags = 0);
	QxtLabel(const QString& text, QWidget* parent = 0, Qt::WindowFlags flags = 0);
	virtual ~QxtLabel();
	
	QString text() const;
	
	Qt::Alignment alignment() const;
	void setAlignment(Qt::Alignment alignment);
	
	Qt::TextElideMode elideMode() const;
	void setElideMode(Qt::TextElideMode mode);
	
	Qxt::Rotation rotation() const;
	void setRotation(Qxt::Rotation rotation);
	
#ifndef QXT_DOXYGEN_RUN
	virtual QSize sizeHint() const;
	virtual QSize minimumSizeHint() const;
#endif // QXT_DOXYGEN_RUN
	
public slots:
	void setText(const QString& text);
	
signals:
	void clicked();
	void textChanged(const QString& text);
	
#ifndef QXT_DOXYGEN_RUN
protected:
	virtual void changeEvent(QEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void paintEvent(QPaintEvent* event);
#endif // QXT_DOXYGEN_RUN
};

#endif // QXTLABEL_H
