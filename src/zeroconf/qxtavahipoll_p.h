
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

#ifndef QXTAVAHIPOLL_P_H
#define QXTAVAHIPOLL_P_H

#include <QObject>
#include <QTimer>
#include <QSocketNotifier>
#include <avahi-common/watch.h>

// Private QObject that does all the work

/**
	@author Chris Vickery <chrisinajar@gmail.com>
*/
class AvahiWatch : public QObject
{
	Q_OBJECT
public:
	AvahiWatch(int fd, AvahiWatchEvent event, AvahiWatchCallback callback, void* userdata);
	~AvahiWatch();
	void setEventType(AvahiWatchEvent event);
	AvahiWatchEvent lastEvent();

private slots:
	void activated(int);

private:
	QSocketNotifier* _notifier;
	int _fd;
	AvahiWatchEvent _event;
	AvahiWatchEvent _lastEvent;
	AvahiWatchCallback _callback;
	void* _userdata;
};

class AvahiTimeout : public QObject
{
	Q_OBJECT
public:
	AvahiTimeout(const struct timeval *tv, AvahiTimeoutCallback callback, void* userdata);
	~AvahiTimeout();
	void updateTimeout(const struct timeval *tv);

private slots:
	void timeout();
private:
	QTimer _timer;
	AvahiTimeoutCallback _callback;
	void* _userdata;
};

#endif
