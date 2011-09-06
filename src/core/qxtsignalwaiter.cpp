
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
\class QxtSignalWaiter

\inmodule QxtCore

\brief The QxtSignalWaiter class blocks and processes events until a signal is emitted

In many cases, writing code that assumes certain actions are synchronous is considerably simpler than breaking your function into
multiple blocks and using signals and slots to connect them all. Using this class, QSignalWaiter::wait will block until a certain
signal is emitted and then return. The return value is true if the signal was caught, or false if a user-specified timeout elapses
before catching the signal.

\code
void MyObject::myFunction() {
    QxtSignalWaiter waiter(myOtherObject, SIGNAL(longProcessFinished()));
    myOtherObject->longProcess();
    if(waiter.wait(5000)) {
        doSomething(myOtherObject->information());
    } else {
        QMessageBox::information(0, "MyObject", "Timed out while waiting on longProcessFinished()", QMessageBox::Ok);
    }
}
\endcode

\bold {Note:}
QxtSignalWaiter is not reentrant. In particular, only one QxtSignalWaiter object per thread can be safely waiting at a
time. If a second QxtSignalWaiter is used while the first is waiting, the first will not return until the second has
timed out or successfully caught its signal.
*/

#include <qxtsignalwaiter.h>
#include <QCoreApplication>
#include <QTimerEvent>

class QxtSignalWaiterPrivate : public QxtPrivate<QxtSignalWaiter>
{
public:
    QXT_DECLARE_PUBLIC(QxtSignalWaiter)

    QxtSignalWaiterPrivate()
    {
        ready = false;
        emitted = false;
        timeout = false;
        waiting = false;
    }

    bool ready, timeout, emitted, waiting;
    int timerID;

    void stopTimer()
    {
        if (timerID)
            qxt_p().killTimer(timerID);
        timerID = 0;
        waiting = false;
    }
};

/*!
 * Constructs a QxtSignalWaiter that will wait for \a signal from \a sender ie. sender::signal()
 * to be emitted. QxtSignalWaiter objects are intended to be created on the stack, therefore no
 * parent parameter is accepted.
 */
QxtSignalWaiter::QxtSignalWaiter(const QObject* sender, const char* signal) : QObject(0)
{
    Q_ASSERT(sender && signal);
    QXT_INIT_PRIVATE(QxtSignalWaiter);
    connect(sender, signal, this, SLOT(signalCaught()));
}

/*!
 * This is an overloaded function provided for convenience. This version can be invoked without first instantiating
 * a QxtSignalWaiter object. Waits for \a signal from \a sender to be emitted within \a msec while processing events
 * according to \a flags. Returns \c true if the signal was caught, or \c false if the timeout elapsed.
 */
bool QxtSignalWaiter::wait(const QObject* sender, const char* signal, int msec, QEventLoop::ProcessEventsFlags flags)
{
    QxtSignalWaiter w(sender, signal);
    return w.wait(msec, flags);
}

/*!
 * Blocks the current function until sender::signal() is emitted. If msec is not -1, wait() will return before the
 * signal is emitted if the specified number of milliseconds have elapsed.
 * Returns \c true if the signal was caught, or \c false if the timeout elapsed.
 * Note that wait() may continue to block after the signal is emitted or the timeout elapses; the function only
 * guarantees that it will not return BEFORE one of these conditions has occurred. This function is not reentrant.
 */
bool QxtSignalWaiter::wait(int msec, QEventLoop::ProcessEventsFlags flags)
{
    QxtSignalWaiterPrivate& d = qxt_d();

    // Clear the emission status
    d.ready = false;
    d.emitted = false;

    // Check input parameters
    if (msec < -1 || msec == 0)
        return false;

    // activate the timeout
    if (msec != -1)
        d.timerID = startTimer(msec);
    else
        d.timerID = 0;

    // Make sure to wait for events
    flags |= QEventLoop::WaitForMoreEvents;

    // Begin waiting
    d.waiting = true;
    while (!d.ready && !d.timeout)
        QCoreApplication::processEvents(flags);

    // Clean up and return status
    qxt_d().stopTimer();
    d.emitted = d.ready;
    d.waiting = false;
    return d.ready;
}

/*!
 * Returns \c true if the desired signal was emitted during the last wait() call.
 */
bool QxtSignalWaiter::hasCapturedSignal() const
{
    return qxt_d().emitted;
}

/*!
 * Signals a waiting object to stop blocking because the desired signal was emitted.
 * QxtSignalWaiter::hasCapturedSignal() will return true after this slot is invoked.
 * Use this slot to allow QxtSignalWaiter to wait for the first of multiple signals.
 */
void QxtSignalWaiter::signalCaught()
{
    if (!qxt_d().waiting) return;
    qxt_d().ready = true;
    qxt_d().stopTimer();
}

/*!
 * \reimp
 */
void QxtSignalWaiter::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);
    cancelWait();
}

/*!
 * Signals a waiting object to stop blocking because the timeout has elapsed.
 * QxtSignalWaiter::hasCapturedSignal() will return false after this slot is invoked.
 * Use this slot to allow QxtSignalWaiter to be interrupted for reasons other than
 * a timeout.
 */
void QxtSignalWaiter::cancelWait()
{
    if (!qxt_d().waiting) return;
    qxt_d().timeout = true;
    qxt_d().stopTimer();
}
