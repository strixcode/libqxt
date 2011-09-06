#include "qxtstdstreambufdevice.h"
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
\class QxtStdStreambufDevice

\inmodule QxtCore

\brief The QxtStdStreambufDevice class provides QIODevice support for std::streambuf

\warning does NOT emit the readyRead() signal

\sa QIODevice

*/

/*!
\brief creates a QxtStdStreambufDevice using a single stream buffer as in and output
*/
QxtStdStreambufDevice::QxtStdStreambufDevice(std::streambuf * b, QObject * parent): QIODevice(parent), buff(b)
{
    setOpenMode(QIODevice::ReadWrite);  //we don't know the real state
    buff_w = 0;
}

/*!
\brief creates a QxtStdStreambufDevice using \a r to read and \a w to write
*/
QxtStdStreambufDevice::QxtStdStreambufDevice(std::streambuf * r, std::streambuf * w, QObject * parent): QIODevice(parent), buff(r), buff_w(w)
{
    setOpenMode(QIODevice::ReadWrite);
}

/*!
\reimp
*/
bool QxtStdStreambufDevice::isSequential() const
{
    return true;//for now
}

/*!
\reimp
*/
qint64 QxtStdStreambufDevice::bytesAvailable() const
{
    return buff->in_avail();
}

/*!
\reimp
*/
qint64 QxtStdStreambufDevice::readData(char * data, qint64 maxSize)
{
    return buff->sgetn(data, maxSize);
}

/*!
\reimp
*/
qint64 QxtStdStreambufDevice::writeData(const char * data, qint64 maxSize)
{
    if (buff_w)
        return buff_w->sputn(data, maxSize);
    return buff->sputn(data, maxSize);
}
