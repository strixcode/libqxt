
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
    \class QxtXmlRpcCall
    \inmodule QxtNetwork
    \brief The QxtXmlRpcCall class represents a Call to an XMLRPC Service.

*/

/*!
    \fn QxtXmlRpcCall::downloadProgress ( qint64 bytesReceived, qint64 bytesTotal )

    This signal is emitted to indicate the progress of retreiving the response from the remote service

    \sa QNetworkReply::downloadProgress()
*/

/*!
    \fn QxtXmlRpcCall::error(QNetworkReply::NetworkError code)

    Signals a network error

    \sa QNetworkReply::error()
*/

/*!
    \fn QxtXmlRpcCall::finished()

    emitted once the result is fully available

    \sa QNetworkReply::finished()
*/

/*!
    \fn QxtXmlRpcCall::sslErrors ( const QList<QSslError> & errors );


    \sa QNetworkReply::sslErrors()
*/

/*!
    \fn QxtXmlRpcCall:: uploadProgress()

    This signal is emitted to indicate the progress of sending the request to the remote service

    \sa QNetworkReply::uploadProgress()
*/

#include "qxtxmlrpccall.h"
#include "qxtxmlrpc_p.h"
#include <QXmlStreamReader>
#include <QNetworkReply>

class QxtXmlRpcCallPrivate
{
public:
    bool isFault;
    QNetworkReply * reply;
    QVariant result;
    void d_finished();
    QxtXmlRpcCall * pub;
};


/*!
  returns true if the remote service sent a fault message
*/
bool QxtXmlRpcCall::isFault() const
{
    return d->isFault;
}

/*!
  returns the result or fault message or a null QVariant() if the call isnt finished yet

  \sa QxtXmlRpcClient#type-conversion
*/
QVariant QxtXmlRpcCall::result() const
{
    return d->result;
}

/*!
  returns an associated network error.
*/
QNetworkReply::NetworkError QxtXmlRpcCall::error() const
{
    return d->reply->error();
}

QxtXmlRpcCall::QxtXmlRpcCall(QNetworkReply * reply)
        : d(new QxtXmlRpcCallPrivate())
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

void QxtXmlRpcCallPrivate::d_finished()
{

    if (!reply->error())
    {
        int s = 0;

        QXmlStreamReader xml(reply->readAll());
        while (!xml.atEnd())
        {
            xml.readNext();
            if (xml.isStartElement())
            {
                if (s == 0)
                {
                    if (xml.name().toString() == "methodResponse")
                    {
                        s = 1;
                    }
                    else
                    {
                        xml.raiseError("expected <methodResponse>,  got:<" + xml.name().toString() + ">");
                    }
                }
                else if (s == 1)
                {
                    if (xml.name().toString() == "params")
                    {
                        s = 2;
                    }
                    else if (xml.name().toString() == "fault")
                    {
                        isFault = true;
                        s = 3;
                    }
                    else
                    {
                        xml.raiseError("expected <params> or <fault>,  got:<" + xml.name().toString() + ">");
                    }
                }
                else if (s == 2)
                {
                    if (xml.name().toString() == "param")
                    {
                        s = 3;
                    }
                    else
                    {
                        xml.raiseError("expected <param>,  got:<" + xml.name().toString() + ">");
                    }
                }
                else if (s == 3)
                {
                    if (xml.name().toString() == "value")
                    {
                        result = QxtXmlRpc::deserialize(xml);
                        s = 4;
                    }
                    else
                    {
                        xml.raiseError("expected <value>,  got:<" + xml.name().toString() + ">");
                    }
                }

            }
        }
        if (xml.hasError())
        {
            qWarning("QxtXmlRpcCall: %s at line %lld column %lld", xml.errorString().toLocal8Bit().data(),
                     xml.lineNumber(),
                     xml.columnNumber());
        }
    }
    emit pub->finished();
}


#include "moc_qxtxmlrpccall.cpp"
