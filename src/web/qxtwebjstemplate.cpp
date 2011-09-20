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
\class QxtWebJsonRPCService

\inmodule QxtWeb

\brief The QxtWebJsonRPCService class provides a Slot based webservice that responds to JSONRPC

See http://json-rpc.org for details on the protocol

To create a Webservice, simply subclass QxtWebJsonRPCService. All slots are exposed as jsonrpc method

\code
class MyService : public QxtWebJsonRPCService
{
Q_OBJECT
public slots:
    int add(int a, int b
    {
        return a +b;
    }
}
\endcode

\code
curl -d '{"method":"add", "id":1, "params": [9,2] }' localhost:1339
{"error":null,"id":1,"result":11}
\endcode

\sa QxtAbstractWebService
*/

#include "qxtwebjstemplate_p.h"

#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptProgram>
#include <QtScript/QScriptValue>

QxtWebJsTemplate::QxtWebJsTemplate(QScriptEngine *engine, QObject *parent)
   : QObject(parent)
   , d(new QxtWebJsTemplatePrivate)

{
    d->setParent(this);
    d->engine = engine;
}

bool QxtWebJsTemplate::load(const QString &file)
{
    d->fileName = file;
    QFile f(file);
    if (!f.open(QFile::ReadOnly)) {
        return false;
    }

    uchar *x = f.map (0, f.size());
    uchar *end = x + f.size();
    QByteArray data;
    data.reserve(f.size());
    data.append("print ('");

    QByteArray codeType;
    QByteArray code;

    while (x < end) {
        if (*x == '<') {
            if (x + 1 < end) {
                ++x;
                if (*x == '?') {
                    ++x;
                    codeType = QByteArray();
                    while (x < end) {
                        if (*x == '=') {
                            codeType="=";
                            ++x;
                            break;
                        } else if( *x == ' ' || *x == '\t' ) {
                            break;
                        }
                        codeType.append(*x);
                        ++x;
                    }
                    code = QByteArray();
                    while (x < end) {
                        if (*x == '?') {
                            if (x + 1 < end) {
                                ++x;
                                if (*x == '>') {
                                    data.append(d->handleCode(codeType, code));
                                    ++x;
                                    break;
                                }
                            }
                        }
                        code.append(*x);
                        ++x;
                    }
                    continue;
                } else {
                    data.append('<');
                }
            }
        }

        if (*x == '\r') {
            data.append("\\r");
        } else if (*x == '\n') {
            data.append("\\n");
        } else if (*x == '\'') {
            data.append("\\'");
        } else {
            data.append(*x);
        }
        ++x;
    }
    data.append("')");
    d->program = QScriptProgram(QString::fromUtf8(data), file);
    return true;
}

QString QxtWebJsTemplate::evaluate()
{
    d->result.clear();
    QScriptValue sThis = d->engine->newQObject (d, QScriptEngine::QtOwnership);

    QScriptValue oldPrint = d->engine->globalObject().property("print");
    d->engine->globalObject().setProperty("print",sThis.property("print"));

    QScriptValue r = d->engine->evaluate(d->program);
    d->engine->globalObject().setProperty("print", oldPrint);

    if (d->engine->hasUncaughtException()) {
        d->result = d->fileName
            + ": " + d->engine->uncaughtException().toString();
    }

    return d->result;
}


