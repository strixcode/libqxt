#ifndef QXTSERIALDEVICE_P_H
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

#define QXTSERIALDEVICE_P_H

#include "qxtserialdevice.h"
#include <QByteArray>

#ifdef Q_OS_UNIX
#include <termios.h>
#endif

QT_FORWARD_DECLARE_CLASS(QSocketNotifier)

class QxtSerialDevicePrivate : public QObject, public QxtPrivate<QxtSerialDevice> {
Q_OBJECT
public:
    QxtSerialDevicePrivate();
    
    int fd;
    bool errorState;
    QSocketNotifier* notifier;
    mutable QByteArray buffer;
    QString device;
    int baud, flow, format;
    QxtSerialDevice::PortSettings portSettings; // duplicated for convenience
#ifdef Q_OS_UNIX
    termios reset, settings;
#endif

    qint64 deviceBuffer() const;
    bool setPortSettings(QxtSerialDevice::PortSettings settings);
    bool updateSettings();
    
public slots:
    int fillBuffer();
    int constFillBuffer() const;
};

#endif // QXTSERIALDEVICE_P_H
