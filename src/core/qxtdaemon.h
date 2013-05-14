
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

#ifndef QXTDAEMON_H
#define QXTDAEMON_H

#include "qxtglobal.h"
#include <QObject>
#include <QString>
#include <QCoreApplication>

QT_FORWARD_DECLARE_CLASS(QFile)

class QXT_CORE_EXPORT QxtDaemon : public QObject
{
    Q_OBJECT
public:
    QxtDaemon(QString applicationName = QCoreApplication::applicationName());
    bool daemonize(bool pidfile = true);
    static int pid();
    bool changeUser(QString name);

#ifndef QXT_DOXYGEN_RUN

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
#else
    static void messageHandler(QtMsgType type, const char *msg);
#endif

#ifdef Q_OS_UNIX
    static void signalHandler(int signal);
#endif

#endif

private:
    QString m_name;
    QFile *logfile;
Q_SIGNALS:
    void signal(int);
    void hangup();
    void terminate();
};

#endif // QXT_DAEMON_H
