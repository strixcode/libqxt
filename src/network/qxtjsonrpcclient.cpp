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
    \class QxtJSONRpcClient
    \inmodule QxtNetwork
    \brief The QxtJSONRpcClient class implements a JSON-RPC Client.

    Implements a Client that can communicate with services implementing the JSON-RPC spec
    http://json-rpc.org/wiki/specification

    \sa QxtJSON

*/

#include "qxtjsonrpcclient.h"
#include "qxtjsonrpccall.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QxtJSON>

struct QxtJSONRpcClient::Private
{
    int callid;
    QUrl url;
    QNetworkAccessManager * networkManager;
};

QxtJSONRpcClient::QxtJSONRpcClient(QObject * parent)
        : QObject(parent)
        , d(new Private())
{
    d->callid=0;
    d->networkManager = new QNetworkAccessManager(this);
}

/*!
  returns the url of the remote service
 */
QUrl QxtJSONRpcClient::serviceUrl() const
{
    return d->url;
}

/*!
  sets the url of the remote service to \a url
 */
void QxtJSONRpcClient::setServiceUrl(QUrl url)
{
    d->url = url;
}
/*!
  returns the QNetworkAccessManager used for connecting to the remote service
 */
QNetworkAccessManager * QxtJSONRpcClient::networkManager() const
{
    return d->networkManager;
}
/*!
  sets the QNetworkAccessManager used for connecting to the remote service to \a manager
 */

void QxtJSONRpcClient::setNetworkManager(QNetworkAccessManager * manager)
{
    delete d->networkManager;
    d->networkManager = manager;
}

/*!
  calls the remote \a method with \a arguments and returns a QxtJSONRpcCall wrapping it.
  you can connect to QxtJSONRpcCall's signals to retreive the status of the call.
 */
QxtJSONRpcCall * QxtJSONRpcClient::call(QString method, QVariantList arguments)
{
    QVariantMap m;
    m["id"]=d->callid++;
    m["method"]=method;
    m["params"]=arguments;

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain; charset=utf-8");
    request.setRawHeader("Connection", "close");
    request.setUrl(d->url);

    return new QxtJSONRpcCall(d->networkManager->post(request, QxtJSON::stringify(m).toUtf8()));
}
