
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

/*!
    \class QxtJSONRpcCall
    \inmodule QxtNetwork
    \brief The QxtJSONRpcCall class represents a Call to an JSON-RPC Service.

*/

/*!
    \fn QxtJSONRpcCall::downloadProgress ( qint64 bytesReceived, qint64 bytesTotal )

    This signal is emitted to indicate the progress of retreiving the response from the remote service

    \sa QNetworkReply::downloadProgress()
*/

/*!
    \fn QxtJSONRpcCall::error(QNetworkReply::NetworkError code)

    Signals a network error

    \sa QNetworkReply::error()
*/

/*!
    \fn QxtJSONRpcCall::finished()

    emitted once the result is fully available

    \sa QNetworkReply::finished()
*/

/*!
    \fn QxtJSONRpcCall::sslErrors ( const QList<QSslError> & errors );


    \sa QNetworkReply::sslErrors()
*/

/*!
    \fn QxtJSONRpcCall:: uploadProgress()

    This signal is emitted to indicate the progress of sending the request to the remote service

    \sa QNetworkReply::uploadProgress()
*/

#include "qxtjsonrpccall.h"
#include <QXmlStreamReader>
#include <QNetworkReply>
#include <QxtJSON>

class QxtJSONRpcCallPrivate
{
public:
    bool isFault;
    QNetworkReply * reply;
    QVariant result;
    void d_finished();
    QxtJSONRpcCall * pub;
};


/*!
  returns true if the remote service sent a fault message
*/
bool QxtJSONRpcCall::isFault() const
{
    return d->isFault;
}

/*!
  returns the result or fault message or a null QVariant() if the call isnt finished yet

  \sa QxtJSON#type-conversion
*/
QVariant QxtJSONRpcCall::result() const
{
    return d->result;
}

/*!
  returns an associated network error.
*/
QNetworkReply::NetworkError QxtJSONRpcCall::error() const
{
    return d->reply->error();
}

QxtJSONRpcCall::QxtJSONRpcCall(QNetworkReply * reply)
        : d(new QxtJSONRpcCallPrivate())
{
    d->isFault = false;
    d->reply = reply;
    d->pub = this;
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(downloadProgress(qint64, qint64)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(const QList<QSslError> &)), this, SIGNAL(sslErrors(const QList<QSslError> &)));
    connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SIGNAL(uploadProgress(qint64, qint64)));
    connect(reply, SIGNAL(finished()), this, SLOT(d_finished()));
}

void QxtJSONRpcCallPrivate::d_finished()
{
    if (!reply->error())
    {
        QVariant m_=QxtJSON::parse(QString::fromUtf8(reply->readAll()));
        if(m_.isNull()){
            qWarning("QxtJSONRpcCall: invalid JSON received");
        }

        QVariantMap m=m_.toMap();

        if(m["error"]!=QVariant()){
            isFault=true;
            result=m["error"];
        }else{
            result=m["result"];
        }
    }
    emit pub->finished();
}


#include "moc_qxtjsonrpccall.cpp"
