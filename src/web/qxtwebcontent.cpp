/****************************************************************************
 **
 ** Copyright (C) Qxt Foundation. Some rights reserved.
 **
 ** This file is part of the QxtWeb module of the Qxt library.
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

/*!
\class QxtWebContent

\inmodule QxtWeb

\brief The QxtWebContent class provides and I/O device for data sent by the web browser

QxtWebContent is a read-only QIODevice subclass that encapsulates data sent
from the web browser, for instance in a POST or PUT request.

In order to avoid delays while reading content sent from the client, and to
insulate multiple pipelined requests on the same connection from each other,
QxtWeb uses QxtWebContent as an abstraction for streaming data.

\sa QxtAbstractWebService
*/

#include "qxtwebcontent.h"
#include <string.h>
#include <QUrl>
#include <QCoreApplication>
#include <QThread>

#ifndef QXT_DOXYGEN_RUN
class QxtWebContentPrivate : public QxtPrivate<QxtWebContent>
{
public:
    QxtWebContentPrivate() : bytesNeeded(0), ignoreRemaining(false) {}
    QXT_DECLARE_PUBLIC(QxtWebContent)

    void init(int contentLength, QIODevice* device)
    {
        if (contentLength < 0)
            bytesNeeded = -1;
        else{
            bytesNeeded = contentLength - qxt_p().bytesAvailable();
	    Q_ASSERT(bytesNeeded >= 0);
	}
	if(device){
	    // Connect a disconnected signal if it has one
	    if(device->metaObject()->indexOfSignal(QMetaObject::normalizedSignature(SIGNAL(disconnected()))) >= 0){
		QObject::connect(device, SIGNAL(disconnected()), &qxt_p(), SLOT(ignoreRemainingContent()));
	    }
	    // Likewise, connect an error signal if it has one
	    if(device->metaObject()->indexOfSignal(QMetaObject::normalizedSignature(SIGNAL(error(QAbstractSocket::SocketError)))) >= 0){
		QObject::connect(device, SIGNAL(error(QAbstractSocket::SocketError)), &qxt_p(), SLOT(errorReceived(QAbstractSocket::SocketError)));
	    }
	}
    }

    qint64 bytesNeeded;
    bool ignoreRemaining;
};
#endif

/*!
 * Constructs a QxtWebContent object.
 *
 * The content provided by this constructor is the data contained in \a prime,
 * followed by whatever data is subsequently written to this object from the
 * source device up to the specified \a contentLength. Note that the provided
 * \a sourceDevice is used solely to detect socket errors and does not specify
 * parentage.
 *
 */
QxtWebContent::QxtWebContent(int contentLength, const QByteArray& prime,
	QObject *parent, QIODevice* sourceDevice) : QxtFifo(prime, parent)
{
    QXT_INIT_PRIVATE(QxtWebContent);
    qxt_d().init(contentLength, sourceDevice);
}

/*!
 * Constructs a QxtWebContent object with the specified \a parent.
 *
 * The content provided by this constructor is exactly the data contained in
 * \a content.
 */
QxtWebContent::QxtWebContent(const QByteArray& content, QObject* parent)
: QxtFifo(content, parent)
{
    QXT_INIT_PRIVATE(QxtWebContent);
    qxt_d().init(content.size(), 0);
    setOpenMode(ReadOnly);
}

/*!
 * \reimp
 */
qint64 QxtWebContent::readData(char* data, qint64 maxSize)
{
    int result = QxtFifo::readData(data, maxSize);
    if(bytesAvailable() == 0 && bytesNeeded() == 0)
        QMetaObject::invokeMethod(this, "aboutToClose", Qt::QueuedConnection);
    return result;
}

/*!
 * Returns true if the total content size is unknown and false otherwise.
 */
bool QxtWebContent::wantAll() const
{
    return (qxt_d().bytesNeeded == -1);
}

/*!
 * Returns the total number of bytes of content expected. This will be -1
 * if the total content size is unknown.
 */
qint64 QxtWebContent::unreadBytes() const
{
    if(wantAll())
	return -1;
    return bytesAvailable() + bytesNeeded();
}

/*!
 * Returns the number of bytes of content that have not yet been written
 * from the source device. This will be -1 if the total content size is
 * unknown.
 */
qint64 QxtWebContent::bytesNeeded() const
{
    return qxt_d().bytesNeeded;
}

/*!
 * \reimp
 */
qint64 QxtWebContent::writeData(const char *data, qint64 maxSize)
{
    if(!(openMode() & WriteOnly))
	return -1; // Not accepting writes
    if(maxSize > 0) {
	// This must match the QxtFifo implementation for consistency
        if(maxSize > INT_MAX) maxSize = INT_MAX; // qint64 could easily exceed QAtomicInt, so let's play it safe
	if(qxt_d().bytesNeeded >= 0){
	    qxt_d().bytesNeeded -= maxSize;
	    Q_ASSERT(qxt_d().bytesNeeded >= 0);
	}
	if(qxt_d().ignoreRemaining)
	    return maxSize;
	return QxtFifo::writeData(data, maxSize);
    }
    // Error
    return -1;
}

/*!
 * \internal
 */
void QxtWebContent::errorReceived(QAbstractSocket::SocketError)
{
    QIODevice *device = qobject_cast<QIODevice*>(sender());
    if(device)
	setErrorString(device->errorString());
}

/*!
 * Blocks until all of the streaming data has been read from the browser.
 *
 * Note that this function will block events for the thread on which it is called.
 * If the main thread is blocked, QxtWeb will be unable to process additional
 * requests until the content has been received.
 */
void QxtWebContent::waitForAllContent()
{
    while(qxt_d().bytesNeeded != 0 && !qxt_d().ignoreRemaining){
	// Still need data ... yield processing
	if(QCoreApplication::hasPendingEvents())
	    QCoreApplication::processEvents();
	if(this->thread() != QThread::currentThread())
	    QThread::yieldCurrentThread();
    }
}

/*!
 * Discards any data not yet read.
 *
 * After invoking this function, any further data received from the browser
 * is silently discarded.
 */
void QxtWebContent::ignoreRemainingContent()
{
    if (qxt_d().bytesNeeded <= 0) return;
    if(!qxt_d().ignoreRemaining){
	qxt_d().ignoreRemaining = true;
	qxt_d().bytesNeeded = 0;
    }
}

#ifndef QXT_DOXYGEN_RUN
typedef QPair<QString, QString> QxtQueryItem;
#endif

/*!
 * Extracts the key/value pairs from application/x-www-form-urlencoded \a data,
 * such as the query string from the URL or the form data from a POST request.
 */
QHash<QString, QString> QxtWebContent::parseUrlEncodedQuery(const QString& data)
{
    QUrl post("/?" + data);
    QHash<QString, QString> rv;
    foreach(const QxtQueryItem& item, post.queryItems())
    {
        rv.insertMulti(item.first, item.second);
    }
    return rv;
}
