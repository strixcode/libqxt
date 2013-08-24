
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
\class QxtDaemon
\inmodule QxtCore
\brief The QxtDaemon class is a Qt-based Implementation of a Unix daemon

QxtDaemon will provide you with a standard UNIX daemon implementation.
after sucessful forking it will install a messageHandler which logs all qDebug/qWarning/etc... output to /var/log/mydaemon.log


example usage:
\code
int main(int argc, char ** argv)
    {
    QxtDaemon d("exampled");
    qDebug("about to fork");
    d.daemonize();
    qDebug("forked");
    d.changeUser("nobody");
    QCoreApplication app(argc,argv);
    return app.exec();
    }
\endcode
*/

#include "qxtdaemon.h"
#include <cassert>
#include <cstdlib>
#include <QFile>
#include <QCoreApplication>
#include <QDateTime>


#ifdef Q_OS_UNIX
#include <signal.h>
#include <fcntl.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif


static QxtDaemon * qxt_daemon_singleton = 0;

#ifdef Q_OS_UNIX
void QxtDaemon::signalHandler(int sig)
{
    emit(qxt_daemon_singleton->signal(sig));
    if (sig == SIGHUP)
    {
        qDebug("hangup signal caught");
        emit(qxt_daemon_singleton->hangup());
    }
    else if (sig == SIGTERM)
    {
        qDebug("terminate signal caught");
        emit(qxt_daemon_singleton->terminate());
        QCoreApplication::exit(0);
    }
}
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#define qxtInstallMessageHandler qInstallMessageHandler
void QxtDaemon::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    const char* msgstr = msg.toLatin1();
    Q_UNUSED(context);
#else
#define qxtInstallMessageHandler qInstallMsgHandler
void QxtDaemon::messageHandler(QtMsgType type, const char *msg)
{
    const char* msgstr = msg;
#endif

    QFile * f = qxt_daemon_singleton->logfile;
    f->write("[");
    f->write(QDateTime::currentDateTime().toString(Qt::ISODate).toLocal8Bit());
    f->write("] ");
    if (type == QtDebugMsg)
        f->write("[qDebug] ");
    else if (type == QtWarningMsg)
        f->write("[qWarning] ");
    else if (type == QtCriticalMsg)
        f->write("[qCritical] ");
    else if (type == QtFatalMsg)
    {
        f->write("[qFatal] ");
        f->write(msgstr);
        f->write("\n");
        f->write("aborting \n");
        f->flush();
        abort();
    }

    f->write(msgstr);
    f->write("\n");
    f->flush();
}

/*!
constructs a new QxtDaemon
the applicationName is used as a base for log and pid files
*/
QxtDaemon::QxtDaemon(QString name)
{
#ifdef Q_OS_UNIX
    if (name.isEmpty())
    {
        qFatal("you need to set an applicationName (e.g. using  QCoreApplication::setApplicationName() )");
    }

    if (qxt_daemon_singleton)
    {
        qFatal("unable to construct more then one QxtDaemon instance");
    }
    else
    {
        qxt_daemon_singleton = this;
        m_name = name;
        logfile = new QFile("/var/log/" + m_name + ".log");
    }
#else
    qFatal("currently QxtDaemon is only implemented on unix");
#endif
}

/*based on work of Levent Karakas <levent at mektup dot at> May 2001*/
/*!
forks the current Process
you can specify weather it will write a pidfile to /var/run/mydaemon.pid or not
if you specify true (the default) QxtDaemon will also try to lock the pidfile. If it can't get a lock it will assume another daemon of the same name is already running and exit
be aware that after calling this function all file descriptors are invalid. QFile will not detect the change, you have to explicitly close all files before forking.
return true on success
*/
bool QxtDaemon::daemonize(bool pidfile)
{
#if defined(Q_OS_UNIX) && !defined(Q_OS_SYMBIAN)
    if (!logfile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        qFatal("cannot open logfile %s", qPrintable(logfile->fileName()));
    logfile->close();



    if (pidfile)
    {
        QFile f("/var/run/" + m_name + ".pid");
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
            qFatal("cannot open pidfile \"/var/run/%s.pid\"", qPrintable(m_name));
        if (lockf(f.handle(), F_TEST, 0) < 0)
            qFatal("can't get a lock on \"/var/run/%s.pid\". another instance is propably already running.", qPrintable(m_name));
        f.close();
    }

    if (!logfile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        qFatal("cannot open logfile %s", qPrintable(logfile->fileName()));
    logfile->close();

    int i;
    if (getppid() == 1) return true; /* already a daemon */
    i = fork();


    /* double fork.*/
    if (i < 0) return false;  /*fork error    */
    if (i > 0) exit(0);     /*parent exits  */
    if (i < 0) return false;  /*fork error    */
    if (i > 0) exit(0);     /*parent exits  */

    /* child (daemon) continues */
    setsid(); /* obtain a new process group */

    for (i = getdtablesize();i >= 0;--i) close(i); /* close all descriptors */


#ifdef Q_OS_LINUX
    ::umask(027); /* some programmers don't understand security, so we set a sane default here */
#endif
    ::signal(SIGCHLD, SIG_IGN); /* ignore child */
    ::signal(SIGTSTP, SIG_IGN); /* ignore tty signals */
    ::signal(SIGTTOU, SIG_IGN);
    ::signal(SIGTTIN, SIG_IGN);
    ::signal(SIGHUP,  QxtDaemon::signalHandler); /* catch hangup signal */
    ::signal(SIGTERM, QxtDaemon::signalHandler); /* catch kill signal */

    if (pidfile)
    {
        int lfp =::open(qPrintable("/var/run/" + m_name + ".pid"), O_RDWR | O_CREAT, 0640);
        if (lfp < 0)
            qFatal("cannot open pidfile \"/var/run/%s.pid\"", qPrintable(m_name));
        if (lockf(lfp, F_TLOCK, 0) < 0)
            qFatal("can't get a lock on \"/var/run/%s.pid\". another instance is propably already running.", qPrintable(m_name));

        QByteArray d = QByteArray::number(pid());

        int writtenSize = 0;
        ssize_t retval = -1;
        while( ((retval = ::write(lfp, d.constData() + writtenSize, d.size() - writtenSize)) != -1) && (writtenSize+=retval) && (writtenSize < d.size()) );
        if(retval == -1)
        {
            qFatal("Can't write to \"/var/run/%s.pid\". Exiting.", qPrintable(m_name));
        }
    }


    assert(logfile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append));
    qxtInstallMessageHandler(QxtDaemon::messageHandler);

    return true;
#else
    return false;
#endif
}

/*!
returns the current processId
*/
int QxtDaemon::pid()
{
#ifdef Q_OS_UNIX
    return getpid();
#else
    return -1;
#endif
}

/*!
changes the current user of this process.
do this after forking to drop root rights.
returns true on success
*/
bool QxtDaemon::changeUser(QString name)
{
#ifdef Q_OS_UNIX
    ///according to the posix spec, i'm not suposed to delete it. *shrug*  weird
    passwd *p =::getpwnam(qPrintable(name));
    if (!p)
        return false;
    return setuid(p->pw_uid) == 0;
#else
    return false;
#endif
}

