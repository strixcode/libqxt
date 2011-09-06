#ifndef QXTSLOTJOB_H
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

#define QXTSLOTJOB_H

#include <QVariant>

#include <qxtglobal.h>
#include <qxtjob.h>


class QxtSignalWaiter;
class QxtSlotJob;
class QXT_CORE_EXPORT QxtFuture: public QObject
{
    Q_OBJECT
public:
    QVariant delayedResult(int msec = -1);
    QVariant joinedResult();

    QxtFuture(const QxtFuture& other);
    ~QxtFuture();
private:
    friend class QxtSlotJob;
    QxtFuture(QxtSlotJob* j);
    QxtSlotJob * job;
    QxtSignalWaiter * waiter;
Q_SIGNALS:
    void done();
    void done(QVariant);
};


class QxtSlotJobPrivate;
QT_FORWARD_DECLARE_CLASS(QThread)
class QXT_CORE_EXPORT QxtSlotJob : public QxtJob
{
    Q_OBJECT
public:
    static QxtFuture detach(QThread * o, QObject* recv, const char* slot,
                            QGenericArgument p1 = QGenericArgument(),
                            QGenericArgument p2 = QGenericArgument(),
                            QGenericArgument p3 = QGenericArgument(),
                            QGenericArgument p4 = QGenericArgument(),
                            QGenericArgument p5 = QGenericArgument(),
                            QGenericArgument p6 = QGenericArgument(),
                            QGenericArgument p7 = QGenericArgument(),
                            QGenericArgument p8 = QGenericArgument(),
                            QGenericArgument p9 = QGenericArgument(),
                            QGenericArgument p10 = QGenericArgument());

    QxtSlotJob(QObject* recv, const char* slot,
               QGenericArgument p1 = QGenericArgument(),
               QGenericArgument p2 = QGenericArgument(),
               QGenericArgument p3 = QGenericArgument(),
               QGenericArgument p4 = QGenericArgument(),
               QGenericArgument p5 = QGenericArgument(),
               QGenericArgument p6 = QGenericArgument(),
               QGenericArgument p7 = QGenericArgument(),
               QGenericArgument p8 = QGenericArgument(),
               QGenericArgument p9 = QGenericArgument(),
               QGenericArgument p10 = QGenericArgument());

    QVariant result();
    QxtFuture exec(QThread *o);

protected:
    virtual void run();
Q_SIGNALS:
    void done(QVariant);
private:
    QXT_DECLARE_PRIVATE(QxtSlotJob)
///must not be in pimpl. that's heavy doom when they are both Qobject and one moves to another thread
private Q_SLOTS:
    void pdone();

};

#endif // QXTSLOTJOB_H
