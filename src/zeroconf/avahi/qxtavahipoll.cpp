
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

#include "qxtavahipoll.h"
#include "qxtavahipoll_p.h"

#include <QThread>
#include <QDebug>

#include <avahi-common/timeval.h>

// declare the functions for the struct
// comments copied from watch.h so that I know what they do :P

/* Create a new watch for the specified file descriptor and for
 * the specified events. The API will call the callback function
 * whenever any of the events happens. */
AvahiWatch* qxtAvahiWatchNew(const AvahiPoll *api, int fd, AvahiWatchEvent event, AvahiWatchCallback callback, void *userdata);
/* Update the events to wait for. It is safe to call this function from an AvahiWatchCallback */
void qxtAvahiWatchUpdate(AvahiWatch *w, AvahiWatchEvent event);
/* Return the events that happened. It is safe to call this function from an AvahiWatchCallback  */
AvahiWatchEvent qxtAvahiWatchGetEvents(AvahiWatch *w);
/* Free a watch. It is safe to call this function from an AvahiWatchCallback */
void qxtAvahiWatchFree(AvahiWatch *w);
/* Set a wakeup time for the polling loop. The API will call the
callback function when the absolute time *tv is reached. If tv is
NULL, the timeout is disabled. After the timeout expired the
callback function will be called and the timeout is disabled. You
can reenable it by calling timeout_update()  */
AvahiTimeout* qxtAvahiTimeoutNew(const AvahiPoll *api, const struct timeval *tv, AvahiTimeoutCallback callback, void *userdata);
/* Update the absolute expiration time for a timeout, If tv is
 * NULL, the timeout is disabled. It is safe to call this function from an AvahiTimeoutCallback */
void qxtAvahiTimeoutUpdate(AvahiTimeout *, const struct timeval *tv);
/* Free a timeout. It is safe to call this function from an AvahiTimeoutCallback */
void qxtAvahiTimeoutFree(AvahiTimeout *t);

const AvahiPoll* qxtAvahiPoll(void)
{
	static const AvahiPoll avahiPoll =
	{
		NULL,
		qxtAvahiWatchNew,
		qxtAvahiWatchUpdate,
		qxtAvahiWatchGetEvents,
		qxtAvahiWatchFree,
		qxtAvahiTimeoutNew,
		qxtAvahiTimeoutUpdate,
		qxtAvahiTimeoutFree
	};
	return &avahiPoll;
}

AvahiWatch* qxtAvahiWatchNew(const AvahiPoll *api, int fd, AvahiWatchEvent event, AvahiWatchCallback callback, void *userdata)
{
	Q_UNUSED(api);
	return new AvahiWatch(fd, event, callback, userdata);
}

void qxtAvahiWatchUpdate(AvahiWatch *w, AvahiWatchEvent event)
{
	w->setEventType(event);
}

AvahiWatchEvent qxtAvahiWatchGetEvents(AvahiWatch *w)
{
	return w->lastEvent();
}

void qxtAvahiWatchFree(AvahiWatch *w)
{
	if (w->thread() == QThread::currentThread())
		delete w;
	else
		w->deleteLater();
}

AvahiTimeout* qxtAvahiTimeoutNew(const AvahiPoll *api, const struct timeval *tv, AvahiTimeoutCallback callback, void *userdata)
{
	Q_UNUSED(api);
	return new AvahiTimeout(tv, callback, userdata);
}

void qxtAvahiTimeoutUpdate(AvahiTimeout *t, const struct timeval *tv)
{
	t->updateTimeout(tv);
}

void qxtAvahiTimeoutFree(AvahiTimeout *t)
{
	if (t->thread() == QThread::currentThread())
		delete t;
	else
		t->deleteLater();
}

/* WATCH */
AvahiWatch::AvahiWatch(int fd, AvahiWatchEvent event, AvahiWatchCallback callback, void *userdata)
		: _notifier(0),
		_fd(fd),
		_event(event),
		_lastEvent((AvahiWatchEvent)0),
		_callback(callback),
		_userdata(userdata)
{
	setEventType(event);
}

AvahiWatch::~AvahiWatch()
{
}

void AvahiWatch::setEventType(AvahiWatchEvent event)
{
	if (_notifier != 0)
	{
		if (_notifier->thread() == QThread::currentThread())
			delete _notifier;
		else
			_notifier->deleteLater();
		_notifier = 0;
	}
	_event = event;
	switch (_event)
	{
		case AVAHI_WATCH_IN:
		{
			_notifier = new QSocketNotifier(_fd, QSocketNotifier::Read, this);
			connect(_notifier, SIGNAL(activated(int)), this, SLOT(activated(int)));
			break;
		}
		case AVAHI_WATCH_OUT:
		{
			_notifier = new QSocketNotifier(_fd, QSocketNotifier::Write, this);
			connect(_notifier, SIGNAL(activated(int)), this, SLOT(activated(int)));
			break;
		}
		default: //The constructor should only get in or out...
			qWarning("AvahiWatch: Bad event type passed to AvahiWatch constructor");
			break;
	}
}

void AvahiWatch::activated(int fd)
{
	Q_UNUSED(fd);
	_lastEvent = _event;
	_callback(this, _fd, _event, _userdata);
	_lastEvent = (AvahiWatchEvent)0;
}

AvahiWatchEvent AvahiWatch::lastEvent()
{
	return _lastEvent;
}

/* TIMEOUT */
AvahiTimeout::AvahiTimeout(const struct timeval *tv, AvahiTimeoutCallback callback, void* userdata)
		: _callback(callback),
		_userdata(userdata)
{
	connect(&_timer, SIGNAL(timeout()), this, SLOT(timeout()));
	updateTimeout(tv);
}

AvahiTimeout::~AvahiTimeout()
{
}

void AvahiTimeout::updateTimeout(const struct timeval *tv)
{
	if (tv == 0)
	{
		_timer.stop();
		return;
	}
	qint64 msecs = (avahi_age(tv)) / 1000;
	if (msecs > 0)
		msecs = 0;
	else
		msecs = -msecs;
	_timer.setInterval(msecs);
	_timer.start();
}

void AvahiTimeout::timeout()
{
	_timer.stop();
	_callback(this, _userdata);
}


