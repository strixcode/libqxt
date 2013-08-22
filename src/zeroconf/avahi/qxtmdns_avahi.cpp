
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

#include "qxtmdns_avahi.h"
#include "qxtmdns_avahi_p.h"
#include <qxtmetaobject.h>
#include <QDebug>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QtEndian>
#include <avahi-common/error.h>

QxtMDNS::QxtMDNS(int id, QObject* parent)
		: QObject(parent)
{
	QXT_INIT_PRIVATE(QxtMDNS);
	qxt_d().info = QHostInfo(id);
	qxt_d().client = NULL;
	qxt_d().recordbrowser = NULL;
	qxt_d().sent = false;
}

void QxtMDNS::doLookup(QString n, QObject * r, const char * m)
{
	qxt_d().name = n;
	qxt_d().receiver = r;
	qxt_d().member = QxtMetaObject::methodName(m);
	int error;
	qxt_d().client = avahi_client_new(
	                     qxtAvahiPoll(), // use the qt event loop
	                     (AvahiClientFlags)0,
	                     QxtMDNSPrivate::avahiClientCallback,
	                     &(qxt_d()),
	                     &error
	                 );
}

void QxtMDNSPrivate::avahiClientCallback(AvahiClient *s, AvahiClientState state, void *userdata)
{
	QxtMDNSPrivate* self = static_cast<QxtMDNSPrivate*>(userdata);
	self->client = s;
	if (state == AVAHI_CLIENT_S_RUNNING)
	{
		self->recordbrowser = avahi_record_browser_new(
		                          s,
		                          AVAHI_IF_UNSPEC,
		                          AVAHI_PROTO_INET,
		                          self->name.toLocal8Bit().constData(),
		                          AVAHI_DNS_CLASS_IN,
		                          AVAHI_DNS_TYPE_A,
		                          (AvahiLookupFlags)0,
					   QxtMDNSPrivate::avahiRecordBrowserCallback,
		                          self);
		if (self->recordbrowser == NULL)
		{
			self->info.setError(QHostInfo::UnknownError);
			self->info.setErrorString(avahi_strerror(avahi_client_errno(self->client)));
			QMetaObject::invokeMethod(self->receiver, self->member, Q_ARG(QHostInfo, self->info));
			self->sent = true;
			self->qxt_p().cancelLookup();
		}
	}
}

void QxtMDNSPrivate::avahiRecordBrowserCallback(AvahiRecordBrowser *b, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, uint16_t clazz, uint16_t type, const void *rdata, size_t size, AvahiLookupResultFlags flags, void *userdata)
{
	///@TODO Support IPv6
	Q_UNUSED(interface);
	Q_UNUSED(protocol);
	Q_UNUSED(name);
	Q_UNUSED(clazz);
	Q_UNUSED(type);
	Q_UNUSED(size);
	Q_UNUSED(flags);
	QxtMDNSPrivate* self = static_cast<QxtMDNSPrivate*>(userdata);
	self->recordbrowser = b;
// 	qDebug() << "Return thing" << md->name << name << size;
	switch (event)
	{
		case AVAHI_BROWSER_NEW:
		{
			//Found an entry!
			uint32_t ip = qFromBigEndian(*static_cast<const uint32_t*>(rdata));
			if (self->sent)
			{
				QHostInfo info(self->info.lookupId());
				info.setAddresses(QList<QHostAddress>() << QHostAddress(ip));
				QMetaObject::invokeMethod(self->receiver, self->member, Q_ARG(QHostInfo, info));
			}
			else
			{
				self->addresses << QHostAddress(ip);
			}
			break;
		}
		case AVAHI_BROWSER_REMOVE:
		{
			uint32_t ip = qFromBigEndian(*static_cast<const uint32_t*>(rdata));
			self->addresses.removeAll(QHostAddress(ip));
			break;
		}
		case AVAHI_BROWSER_CACHE_EXHAUSTED:
			break;
		case AVAHI_BROWSER_ALL_FOR_NOW:
			if (self->addresses.count() == 0)
			{
				self->info.setError(QHostInfo::HostNotFound);
				self->info.setErrorString("The host was not found.");
			}
			else
			{
				self->info.setAddresses(self->addresses);
				self->addresses.clear();
			}
			QMetaObject::invokeMethod(self->receiver, self->member, Q_ARG(QHostInfo, self->info));
			self->sent = true;
			break;
		case AVAHI_BROWSER_FAILURE:

			if (self->sent)
			{
				QHostInfo info(self->info.lookupId());
				info.setError(QHostInfo::UnknownError);
				info.setErrorString(avahi_strerror(avahi_client_errno(self->client)));
				info.setAddresses(self->addresses);
				QMetaObject::invokeMethod(self->receiver, self->member, Q_ARG(QHostInfo, info));
			}
			else
			{
				self->info.setError(QHostInfo::UnknownError);
				self->info.setErrorString(avahi_strerror(avahi_client_errno(self->client)));
				self->info.setAddresses(self->addresses);
				self->addresses.clear();
				QMetaObject::invokeMethod(self->receiver, self->member, Q_ARG(QHostInfo, self->info));
				self->sent = true;
			}
			break;
	}
}

void QxtMDNS::cancelLookup()
{
	if (qxt_d().recordbrowser != NULL)
		avahi_record_browser_free(qxt_d().recordbrowser);

	if (qxt_d().client != NULL)
		avahi_client_free(qxt_d().client);

	deleteLater();
}

