#include "qxtserialdevice.h"
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

#include "qxtserialdevice_p.h"

QxtSerialDevicePrivate::QxtSerialDevicePrivate() {
    fd = -1;
    notifier = 0;
}

/**
 * Creates a new QxtSerialDevice with the specified parent and sets the device name to \a device.
 */
QxtSerialDevice::QxtSerialDevice(const QString& device, QObject* parent) : QIODevice(parent) {
    QXT_INIT_PRIVATE(QxtSerialDevice);
    setDeviceName(device);
    setBaud(Baud9600);
}

/**
 * Creates a new QxtSerialDevice with the specified parent.
 */
QxtSerialDevice::QxtSerialDevice(QObject* parent) : QIODevice(parent) {
    QXT_INIT_PRIVATE(QxtSerialDevice);
    setBaud(Baud9600);
}

/**
 * \reimp
 */
bool QxtSerialDevice::atEnd() const {
    return (bytesAvailable() == 0);
}

/**
 * \reimp
 */
bool QxtSerialDevice::canReadLine() const {
    if(QIODevice::canReadLine()) return true;
    if(openMode() & QIODevice::Unbuffered) return false;
    if(qxt_d().constFillBuffer()) return false;
    return qxt_d().buffer.contains('\n');
}

/**
 * \reimp
 */
bool QxtSerialDevice::isSequential() const {
    return true;
}

/**
 * Sets the device name to \a device. This is a device node like "/dev/ttyS0" on UNIX or a device name like "COM1" on Windows.
 * \sa deviceName
 */
void QxtSerialDevice::setDeviceName(const QString& device) {
    qxt_d().device = device;
}

/**
 * Returns the current device name.
 * \sa device
 */
QString QxtSerialDevice::deviceName() const {
    return qxt_d().device;
}

/**
 * Returns the file descriptor for the open device. If the device is not open, this function returns -1.
 */
int QxtSerialDevice::handle() const {
    if(!isOpen()) return -1;
    return qxt_d().fd;
}

bool QxtSerialDevice::open(const QString& device, OpenMode mode) {
    setDeviceName(device);
    return(open(mode));
}

bool QxtSerialDevice::setPortSettings(PortSettings setup) {
    qxt_d().portSettings = setup;
    return qxt_d().setPortSettings(setup);
}

QxtSerialDevice::PortSettings QxtSerialDevice::portSettings() const {
    return qxt_d().portSettings;
}

QxtSerialDevice::PortSetting QxtSerialDevice::dataBits() const {
    return PortSetting(int(qxt_d().portSettings & BitMask));
}

QxtSerialDevice::PortSetting QxtSerialDevice::flowControl() const {
    return PortSetting(int(qxt_d().portSettings & FlowMask));
}

QxtSerialDevice::PortSetting QxtSerialDevice::parity() const {
    return PortSetting(int(qxt_d().portSettings & ParityMask));
}

QxtSerialDevice::PortSetting QxtSerialDevice::stopBits() const {
    return PortSetting(int(qxt_d().portSettings & StopMask));
}

