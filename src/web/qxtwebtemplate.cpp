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

\brief The QxtWebTemplate class is an template engine that allows embedding JavaScript into any text

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

Everything within <? ?> tags will be interpreted as JavaScript with one
notable exception: if the first line contains <?xml ... ?>, it will be
copied verbatim into the output.

JavaScript short form is also allowed:
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


/*! Constructs a new QxtWebTemplate
 *
 *  A new QScriptEngine will be constructed if one is not supplied in \a engine.
 *
 *  \warning This class will modify the script engine if any of the assign()
 *  methods are used.
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

/*! Constructs a new QxtWebTemplate as RIAA post object.
 *
 *  This method of construction is well suited for templated responses to
 *  web service queries. Upon deconstruction, QxtWebTemplate automatically
 *  answers the passed QxtWebRequestEvent \a ev unless the release() method
 *  is used first. The \a service and \a ev parameters must not be NULL.
 *  Since this is QObject-based, a \a parent may also be specified for
 *  objects created on the heap (via new).
 *
 *  The normal content-type generated is <b>text/html; charset=\"utf-8\"</b>.
 *  This may be overridden by supplying a value for \a contentType. A new
 *  QScriptEngine will be constructed if one is not supplied in \a engine.
 *
 *  Example usage:
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
 *  \warning The content will always be sent UTF-8 encoded, no matter what has
 *  been supplied for \a contentType.
 */
QxtWebTemplate::QxtWebTemplate(QxtAbstractWebService *service,
        QxtWebRequestEvent *ev, QByteArray contentType,
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


/*!
 *  Releases the QxtWebRequestEvent preventing automatic response upon
 *  deconstruction. If the template wasn't constructed with an event,
 *  this method does nothing. Invoking the method more than once does not
 *  cause an error but doesn't accomplish anything either.
 *
 */
void QxtWebTemplate::release()
{
    d->event = 0;
}

/*! Destroys the template and releases all resources. If the template was
 *  constructed with a QxtWebRequestEvent (and release() was not invoked),
 *  the evaluate() method is called internally and the result posted as
 *  a QxtWebPageEvent using the supplied content-type.
 */
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

/*!
 *  Loads the specified \a file as the template source. Any pathname suitable
 *  with the QFile class constructor is allowed, including resources.
 *  Returns true if the file was successfully loaded and processed and false
 *  otherwise (usually indicating the file did not exist).
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

    int line = 0;
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
                                    data.append(d->handleCode(codeType, code, line));
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

	if (*x == '\\'){
	    data.append("\\\\");
	} else if (*x == '\r') {
            data.append("\\r");
        } else if (*x == '\n') {
	    ++line;
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

/*!
 *  Evaluates the template returning the result as UTF-8 encoded text. This
 *  method may be invoked as many times as desired.
 *
 */
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
	qWarning() << "QxtWebTemplate: Exception in evaluate"
	    << d->fileName << ":" << d->engine->uncaughtException().toString();
        d->result = d->fileName
            + ": " + d->engine->uncaughtException().toString();
    }

    return d->result;
}

/*!
 *  Assign a \a value to a template variable named \a property.
 *
 *  \warning This directly updates the QScriptEngine. If you passed your own
 *  engine in the constructor, it will be modified by this method.
 */
void QxtWebTemplate::assign(const QString &property, const QVariant &value)
{
    d->engine->globalObject().setProperty(property,
            qScriptValueFromValue(d->engine,value));
}

/*!
 *  Assign another template \a t to a template variable \a property. The
 *  template so assigned will be rendered whenever this one is rendered.
 *
 *  \warning This directly updates the QScriptEngine. If you passed your own
 *  engine in the constructor, it will be modified by this method.
 */
void QxtWebTemplate::assign(const QString &property, QxtWebTemplate *t)
{
    d->others.insert(property, t);
    t->d->assignedTo.append(this);
}
