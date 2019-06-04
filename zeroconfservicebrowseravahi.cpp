/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Copyright (C) 2016 Simon Stürz <simon.stuerz@guh.io>                   *
 *                                                                         *
 *  This file is part of nymea.                                            *
 *                                                                         *
 *  This library is free software; you can redistribute it and/or          *
 *  modify it under the terms of the GNU Lesser General Public             *
 *  License as published by the Free Software Foundation; either           *
 *  version 2.1 of the License, or (at your option) any later version.     *
 *                                                                         *
 *  This library is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
 *  Lesser General Public License for more details.                        *
 *                                                                         *
 *  You should have received a copy of the GNU Lesser General Public       *
 *  License along with this library; If not, see                           *
 *  <http://www.gnu.org/licenses/>.                                        *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "zeroconfservicebrowseravahi.h"
#include "zeroconfservicebrowseravahi_p.h"
#include "loggingcategories.h"

#include <avahi-common/error.h>

/*! Constructs a new \l{ZeroConfServiceBrowserAvahi} with the given \a parent. */
ZeroConfServiceBrowserAvahi::ZeroConfServiceBrowserAvahi(const QString &serviceType, QObject *parent) :
    ZeroConfServiceBrowser(serviceType, parent),
    m_serviceType(serviceType),
    d_ptr(new ZeroConfServiceBrowserAvahiPrivate(new QtAvahiClient))
{
    connect(d_ptr->client, &QtAvahiClient::clientStateChanged, this, &ZeroConfServiceBrowserAvahi::onClientStateChanged);
    qCDebug(dcPlatformZeroConf) << "Created Avahi service broweser" << serviceType;

    d_ptr->client->start();
}

/*! Destructs this \l{ZeroConfServiceBrowserAvahi}. */
ZeroConfServiceBrowserAvahi::~ZeroConfServiceBrowserAvahi()
{
    // Delete each service browser
    foreach (const QString &serviceType, d_ptr->serviceBrowserTable.keys()) {
        AvahiServiceBrowser *browser = d_ptr->serviceBrowserTable.take(serviceType);
        if (browser) {
            avahi_service_browser_free(browser);
        }
    }

    // Delete the service type browser
    if (d_ptr->serviceTypeBrowser)
        avahi_service_type_browser_free(d_ptr->serviceTypeBrowser);

    delete d_ptr;
}

/*! Returns the current \l{AvahiServiceEntry} list of this \l{ZeroConfServiceBrowserAvahi}. */
QList<ZeroConfServiceEntry> ZeroConfServiceBrowserAvahi::serviceEntries() const
{
    return m_serviceEntries;
}

void ZeroConfServiceBrowserAvahi::onClientStateChanged(const QtAvahiClient::QtAvahiClientState &state)
{
    if (state == QtAvahiClient::QtAvahiClientStateRunning) {
        qCDebug(dcPlatformZeroConf()) << "Service browser client connected.";
        if (m_serviceType.isEmpty()) {
            // Return if we already have a service type browser
            if (d_ptr->serviceTypeBrowser)
                return;
            d_ptr->serviceTypeBrowser = avahi_service_type_browser_new(d_ptr->client->m_client, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, 0, (AvahiLookupFlags) 0, ZeroConfServiceBrowserAvahiPrivate::callbackServiceTypeBrowser, this);
        } else {
            createServiceBrowser(m_serviceType.toUtf8().data());
        }
    } else if (state == QtAvahiClient::QtAvahiClientStateFailure) {
        qCWarning(dcPlatformZeroConf()) << "client failure:" << d_ptr->client->errorString();
    }
}

void ZeroConfServiceBrowserAvahi::createServiceBrowser(const char *serviceType)
{
    // create a new service browser for the given serviceType
    AvahiServiceBrowser *browser = avahi_service_browser_new(d_ptr->client->m_client, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, serviceType, NULL, (AvahiLookupFlags) 0,  ZeroConfServiceBrowserAvahiPrivate::callbackServiceBrowser, this);
    d_ptr->serviceBrowserTable.insert(serviceType, browser);
}


