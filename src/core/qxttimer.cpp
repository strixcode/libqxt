#include "qxttimer.h"
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

#include "qxtmetaobject.h"
#include <QTimerEvent>
#include <QPointer>

/*!
    \class QxtTimer
    \inmodule QxtCore
    \brief The QxtTimer class extends QTimer.

    QxtTimer extends QTimer with capability to call a slot after with given
    parameters after a given time interval.

    Example usage:
    \code
    QxtTimer::singleShot(500, widget, SLOT(setWindowTitle(QString)), QString("Window Title"));
    \endcode
 */

class QxtSingleShotTimer : public QObject
{
public:
    QxtSingleShotTimer(int msec, QObject* receiver, const char* member, const QVariantList& args);

protected:
    void timerEvent(QTimerEvent* event);

private:
    QPointer<QObject> receiver;
    const char* member;
    QVariantList args;
    int timerId;
};

QxtSingleShotTimer::QxtSingleShotTimer(int msec, QObject* receiver, const char* member, const QVariantList& args) :
    receiver(receiver), member(member), args(args), timerId(-1)
{
    timerId = startTimer(msec);
}

void QxtSingleShotTimer::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == timerId)
    {
        QxtMetaObject::invokeMethod(receiver, member, args.at(0), args.at(1),
                                    args.at(2), args.at(3), args.at(4), args.at(5), 
                                    args.at(6), args.at(7), args.at(8), args.at(9));
        deleteLater();
    }
}

/*!
    Constructs a new QxtTimer with \a parent.
 */
QxtTimer::QxtTimer(QObject* parent) : QTimer(parent)
{
}

/*!
    Destructs the timer.
 */
QxtTimer::~QxtTimer()
{
}

/*!
    This static function calls a slot with given parameters after a given time interval.

    It is very convenient to use this function because you do not need to bother with a timerEvent or create a local QTimer object.

    You can pass up to ten arguments (\a arg0, \a arg1, \a arg2, \a arg3, \a arg4, \a arg5, \a arg6, \a arg7, \a arg8 and \a arg9).

    The \a receiver is the receiving object and the \a member is the slot. The time interval is \a msec milliseconds.
 */
void QxtTimer::singleShot(int msec, QObject* receiver, const char* member, const QVariant& arg0,
                          const QVariant& arg1, const QVariant& arg2, const QVariant& arg3,
                          const QVariant& arg4, const QVariant& arg5, const QVariant& arg6,
                          const QVariant& arg7, const QVariant& arg8, const QVariant& arg9)
{
    if (receiver && member)
    {
        QVariantList args;
        args << arg0 << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9;
        (void) new QxtSingleShotTimer(msec, receiver, member, args);
    }
}
