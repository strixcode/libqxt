#include "qxtdeplex_p.h"
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
\class QxtDeplex

\inmodule QxtCore

\brief The QxtDeplex class puts a standard QIODevice into a QxtPipe chain



implements a normal QxtPipe with the exception that r/w operations are delegated to the device set by setDevice()

\sa QIODevice
\sa QxtPipe
*/

/*!
\fn QxtDeplex::QxtDeplex(QObject * parent)
\brief creates a new QxtDeplex


\fn QxtDeplex::QxtDeplex(QIODevice * device,QObject * parent)
\brief creates a new QxtDeplex and delegates r/w operations to the device


\fn void QxtDeplex::setDevice(QIODevice * device);
\brief delegate r/w operations to the device

\fn     QIODevice *QxtDeplex::device() const;
\brief returns the device currently delegated


\fn virtual void   QxtDeplex::receiveData (QByteArray data, const QxtPipe * sender )

*/





QxtDeplex::QxtDeplex(QObject * parent): QxtPipe(parent)
{
    QXT_INIT_PRIVATE(QxtDeplex);
    qxt_d().delegate = 0;
    setDevice(0);
}
QxtDeplex::QxtDeplex(QIODevice * device, QObject * parent): QxtPipe(parent)
{
    QXT_INIT_PRIVATE(QxtDeplex);
    qxt_d().delegate = 0;
    setDevice(device);
}

void QxtDeplex::setDevice(QIODevice * device)
{
    if (qxt_d().delegate)
    {
        QObject::disconnect(qxt_d().delegate, SIGNAL(readyRead()), &qxt_d(), SLOT(readyRead()));
        QObject::disconnect(qxt_d().delegate, SIGNAL(destroyed(QObject *)), &qxt_d(), SLOT(extDestroyed(QObject *)));
    }
    qxt_d().delegate = device;
    if (qxt_d().delegate)
    {
        QObject::connect(qxt_d().delegate, SIGNAL(readyRead()), &qxt_d(), SLOT(readyRead()));
        QObject::connect(qxt_d().delegate, SIGNAL(destroyed(QObject *)), &qxt_d(), SLOT(extDestroyed(QObject *)));
    }
}
QIODevice *QxtDeplex::device() const
{
    return qxt_d().delegate;
}

void QxtDeplexPrivate::readyRead()
{
    QByteArray d = static_cast<QIODevice*>(sender())->readAll();

    qxt_p().enqueData(d);
    qxt_p().sendData(d);
}

void QxtDeplexPrivate::extDestroyed(QObject *)
{
    delegate = 0;
}


qint64 QxtDeplex::writeData(const char * a, qint64 s)
{
    if (!qxt_d().delegate)
        return 0;
    return qxt_d().delegate->write(a, s);

}
void  QxtDeplex::receiveData(QByteArray data, const QxtPipe *)
{
    if (!qxt_d().delegate)
        return;
    qxt_d().delegate->write(data);
}
