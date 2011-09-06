#ifndef QXTSERIALDEVICE_H
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

#define QXTSERIALDEVICE_H

#include "qxtglobal.h"
#include <QIODevice>

class QxtSerialDevicePrivate;
class QXT_CORE_EXPORT QxtSerialDevice : public QIODevice {
Q_OBJECT
Q_ENUMS(BaudRate)
Q_FLAGS(PortSetting PortSettings)
public:
    enum BaudRate {
        Baud110, Baud300, Baud600, Baud1200, Baud2400, Baud4800, Baud9600, Baud19200, Baud38400, Baud57600, Baud115200
    };

    enum PortSetting {
        Bit8 = 0,
        Bit7 = 1,
        Bit6 = 2,
        Bit5 = 3,
        BitMask = 3,
        FlowOff = 0,
        FlowRtsCts = 4,
        FlowXonXoff = 8,
        FlowMask = 12,
        ParityNone = 0,
        ParityOdd = 16,
        ParityEven = 32,
        ParityMark = 48,
        ParitySpace = 64,
        ParityMask = 112,
        Stop1 = 0,
        Stop2 = 128,
        StopMask = 128
    };
    Q_DECLARE_FLAGS(PortSettings, PortSetting);

    explicit QxtSerialDevice(const QString& device, QObject* parent = 0);
    QxtSerialDevice(QObject* parent = 0);

    virtual bool atEnd() const;
    virtual qint64 bytesAvailable() const;
    virtual bool canReadLine() const;
    virtual void close();
    virtual bool isSequential() const;
    bool open(const QString& device, OpenMode mode);
    virtual bool open(OpenMode mode);
    int handle() const;

    void setDeviceName(const QString& device);
    QString deviceName() const;

    bool setBaud(BaudRate rate);
    BaudRate baud() const;

    bool setPortSettings(PortSettings setup);
    PortSettings portSettings() const;
    PortSetting dataBits() const;
    PortSetting flowControl() const;
    PortSetting parity() const;
    PortSetting stopBits() const;

protected:
    virtual qint64 readData(char* data, qint64 maxSize);
    virtual qint64 writeData(const char* data, qint64 maxSize);

private:
    QXT_DECLARE_PRIVATE(QxtSerialDevice)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QxtSerialDevice::PortSettings);

#endif
