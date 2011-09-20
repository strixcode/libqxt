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
\class QxtWebTemplate

\inmodule QxtWeb

\brief The QxtWebTemplate is an template engine that allows embedding javascript into any text

Example usage:
\code
QScriptEngine engine;
engine.globalObject().setProperty("hello", 5);
QxtWebJsTemplate t(&engine);
t.load("index.html");
qDebug() << t.evaluate();
\endcode

index.html could look like the following:
\code

<html><body>
<?js print(hello + 3); ?>
</body></html>

\endcode

everything within &lt;? ?&gt; will be interpreted as javascript.
short form is also allowed:
\code

<html><body>
<?=hello+3?>
</body></html>

\endcode

*/

#include "qxtwebtemplate_p.h"
#include "qxtabstractwebservice.h"

#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptProgram>
#include <QtScript/QScriptValue>
#include "qxtwebevent.h"


/*! constructs a new QxtWebTemplate
 *
 * optionaly, pass the \p engine to use.
 */
QxtWebTemplate::QxtWebTemplate(QScriptEngine *engine, QObject *parent)
   : QObject(parent)
   , d(new QxtWebTemplatePrivate)

{
    d->setParent(this);

    if (engine == 0)
        engine = new QScriptEngine(this);

    d->engine = engine;
    d->service = 0;
    d->event = 0;
}

/*! constructs a new QxtWebTemplate as RIAA post object
 *
 *  on deconstruction, QxtWebTemplate automaticly answers the passed QxtWebRequestEvent \p ev
 *  unless release() is called before that.
 *
 *  example usage:
 *
 *  \code
 * 
 *  class Home : public QxtAbstractWebService
 *  {
 *     virtual void pageRequestedEvent(QxtWebRequestEvent* event)
 *     {
 *          QxtWebTemplate view(this, event);
 *          view.load("home.html");
 *          // deconstruction will automatically send home.html as reponse
 *     }
 *  };
 * 
 *  \endcode
 *
 *  note that the content will always be sent UTF-8 encoded, no matter what you set as \p contentType
 *
 * optionaly, pass the \p engine to use.
 */
QxtWebTemplate::QxtWebTemplate(QxtAbstractWebService *service,
        QxtWebRequestEvent* ev, QByteArray contentType,
        QScriptEngine *engine,QObject *parent)
   : QObject(parent)
   , d(new QxtWebTemplatePrivate)

{
    d->setParent(this);

    if (engine == 0)
        engine = new QScriptEngine(this);

    d->engine = engine;
    d->service = service;
    d->event = ev;
    d->contentType = contentType;
}


/*
 * prevent sending a response on deconstruction
 *
 */
void QxtWebTemplate::release()
{
    d->event = 0;
}

QxtWebTemplate::~QxtWebTemplate()
{
    QString res;
    if ((d->service && d->event) || d->assignedTo.count()) {
        res = evaluate();
    }

    if (d->service && d->event)
    {
        QxtWebPageEvent *answer = new QxtWebPageEvent(
                d->event->sessionID, d->event->requestID,
                res.toUtf8());
        answer->contentType = d->contentType;
        d->service->postEvent(answer);
    }
    if (d->assignedTo.count()) {
        foreach(QxtWebTemplate *p , d->assignedTo) {
            foreach (const QString &k, p->d->others.keys(this)) {
                p->assign(k, res);
                p->d->others.remove(k);
            }
        }
    }

    foreach(QxtWebTemplate *t, d->others.values()) {
        t->d->assignedTo.removeAll(this);
    }
}

/*
 * load the specified \p file as template
 *
 */

bool QxtWebTemplate::load(const QString &file)
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
                        } else if(
                                *x == ' ' ||
                                *x == '\t' ||
                                *x == '\n' ||
                                *x == '\r' ||
                                *x == '\v'
                                ) {
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

/*
 * return the content as utf8 encoded string
 *
 * */
QString QxtWebTemplate::evaluate()
{
    d->result.clear();
    QScriptValue sThis = d->engine->newQObject (d, QScriptEngine::QtOwnership);

    QScriptValue go = d->engine->globalObject();

    QScriptValue oldPrint = go.property("print");
    go.setProperty("print",sThis.property("print"));


    QMapIterator<QString, QxtWebTemplate *> i(d->others);
    while (i.hasNext()) {
        i.next();
        go.setProperty(i.key(), i.value()->evaluate());
    }

    QScriptValue r = d->engine->evaluate(d->program);
    go.setProperty("print", oldPrint);

    if (d->engine->hasUncaughtException()) {
        d->result = d->fileName
            + ": " + d->engine->uncaughtException().toString();
    }

    return d->result;
}

/*
 * assign a value to a template variable
 *
 *
 * note: this changes the QScriptEngine. If you passed an engine in the constructor, this will be modified
 * */

void QxtWebTemplate::assign(const QString &property, const QVariant &value)
{
    d->engine->globalObject().setProperty(property,
            d->engine->newVariant (value));
}

/*
 * assign another template to a template variable
 *
 * the assigned template will rendered when this one is rendered
 *
 * note: this changes the QScriptEngine. If you passed an engine in the constructor, this will be modified
 * */

void QxtWebTemplate::assign(const QString &property, QxtWebTemplate *t)
{
    d->others.insert(property, t);
    t->d->assignedTo.append(this);
}

