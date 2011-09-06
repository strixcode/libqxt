
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

/*
\class QxtJob

\inmodule QxtCore

\brief The QxtJob class executes a job on a QThread. Once or multiple times.

QxtJob allows easily starting jobs on different threads.
exec() will ask for the QThread to run the job on.
The Qthread needs an event loop. since version 4.4, QThread has
a non pure run function with a default event loop, allowing easy deployment of jobs.

\code
QThread thread;
thread.start();
LockJob().exec(&thread);
\endcode
*/


/*
\fn  void QxtJob::run()
This function is called by QxtJob.
reimplemented this function to do useful work.
Returning from this method will end the execution of the job.
*/

/*
\fn  void QxtJob::done()
This signal is emitted, when the run() function returns.
*/

#include "qxtjob_p.h"
#include <cassert>
#include <QThread>

class Thread : public QThread
{
public:
    static void usleep(unsigned long usecs)
    {
        QThread::usleep(usecs);
    }
};
/*!
default constructor
*/
QxtJob::QxtJob()
{
    QXT_INIT_PRIVATE(QxtJob);
    qxt_d().running.set(false);
    connect(&qxt_d(), SIGNAL(done()), this, SIGNAL(done()));
}
/*!
execute the Job on \a onthread
*/
void QxtJob::exec(QThread * onthread)
{
    qxt_d().moveToThread(onthread);
    connect(this, SIGNAL(subseed()), &qxt_d(), SLOT(inwrap_d()), Qt::QueuedConnection);

    qxt_d().running.set(true);
    emit(subseed());
}
/*!
\warning The destructor joins. Means it blocks until the job is finished
*/
QxtJob::~QxtJob()
{
    join();
}
/*!
block until the Job finished
Note that the current thread will be blocked.
If you use this, you better be damn sure you actually want a thread.
Maybe you actualy want to use QxtSignalWaiter.
*/
void QxtJob::join()
{
    while (qxt_d().running.get() == true)
    {
        /*!
        oh yeah that sucks ass,
        but since any kind of waitcondition will just fail due to undeterminnism,
        we have no chance then polling.
        And no, a mutex won't work either.
        using join for anything else then testcases sounds kindof retarded anyway.
        */
        Thread::usleep(1000);
    }

}
void QxtJobPrivate::inwrap_d()
{
    synca.wakeAll();
    qxt_p().run();
    running.set(false);
    emit(done());
}









