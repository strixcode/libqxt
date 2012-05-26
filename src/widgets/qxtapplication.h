#ifndef QXTAPPLICATION_H
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

#define QXTAPPLICATION_H

#include <QApplication>
#include "qxtglobal.h"

class QxtApplicationPrivate;
class QxtNativeEventFilter;

#define qxtApp (QxtApplication::instance())

class QXT_GUI_EXPORT QxtApplication : public QApplication
{
    Q_OBJECT
    QXT_DECLARE_PRIVATE(QxtApplication)

public:
    QxtApplication(int& argc, char** argv);
    QxtApplication(int& argc, char** argv, bool GUIenabled);
    QxtApplication(int& argc, char** argv, Type type);
#if defined(Q_WS_X11)
    explicit QxtApplication(Display* display, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0);
    QxtApplication(Display* display, int& argc, char** argv, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0);
#endif // Q_WS_X11
    virtual ~QxtApplication();

    void installNativeEventFilter(QxtNativeEventFilter* filter);
    void removeNativeEventFilter(QxtNativeEventFilter* filter);

#if defined(Q_WS_X11)
    virtual bool x11EventFilter(XEvent* event);
#elif defined(Q_WS_WIN)
    virtual bool winEventFilter(MSG* msg, long* result);
#elif defined(Q_WS_MAC)
    virtual bool macEventFilter(EventHandlerCallRef caller, EventRef event);
#endif // Q_WS_*

    inline static QxtApplication* instance()
    {
        return qobject_cast<QxtApplication*>(QApplication::instance());
    }
};

#endif // QXTAPPLICATION_H
