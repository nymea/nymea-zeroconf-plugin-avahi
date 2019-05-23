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

#include "zeroconfservicebrowseravahi_p.h"
#include "zeroconfservicebrowseravahi.h"
#include "network/zeroconf/zeroconfserviceentry.h"
#include "loggingcategories.h"

#include <avahi-common/strlst.h>
#include <avahi-common/error.h>

#include <QPointer>

ZeroConfServiceBrowserAvahiPrivate::ZeroConfServiceBrowserAvahiPrivate(QtAvahiClient *client) :
    client(client),
    serviceTypeBrowser(nullptr)
{

}

ZeroConfServiceBrowserAvahiPrivate::~ZeroConfServiceBrowserAvahiPrivate()
{
    foreach (AvahiServiceResolver *resolver, m_serviceResolvers) {
        avahi_service_resolver_free(resolver);
    }
}

void ZeroConfServiceBrowserAvahiPrivate::callbackServiceTypeBrowser(AvahiServiceTypeBrowser *browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *type, const char *domain, AvahiLookupResultFlags flags, void *userdata)
{
    Q_UNUSED(browser)
    Q_UNUSED(interface)
    Q_UNUSED(protocol)
    Q_UNUSED(domain)
    Q_UNUSED(flags)

    ZeroConfServiceBrowserAvahi *serviceBrowser = static_cast<ZeroConfServiceBrowserAvahi *>(userdata);
    if (!serviceBrowser)
        return;

    switch (event) {
    case AVAHI_BROWSER_NEW:
        if (!serviceBrowser->m_serviceTypes.contains(type)) {
            serviceBrowser->m_serviceTypes.append(type);
            qCDebug(dcPlatformZeroConf()) << "[+] Service browser" << type;
            serviceBrowser->createServiceBrowser(type);
        }
        break;
    case AVAHI_BROWSER_REMOVE:
        // Note: the browser for this serviceType will be deleted once all
        // services from this type are removed
        break;
    case AVAHI_BROWSER_CACHE_EXHAUSTED:
        break;
    case AVAHI_BROWSER_ALL_FOR_NOW:
        break;
    case AVAHI_BROWSER_FAILURE:
        qCWarning(dcPlatformZeroConf()) << "Service type browser error:" << QString(avahi_strerror(avahi_client_errno(serviceBrowser->d_ptr->client->m_client)));
        break;
    }
}

void ZeroConfServiceBrowserAvahiPrivate::callbackServiceBrowser(AvahiServiceBrowser *browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, const char *type, const char *domain, AvahiLookupResultFlags flags, void *userdata)
{
    Q_UNUSED(browser);
    Q_UNUSED(flags);

    ZeroConfServiceBrowserAvahi *serviceBrowser = static_cast<ZeroConfServiceBrowserAvahi *>(userdata);
    if (!serviceBrowser)
        return;

    switch (event) {
    case AVAHI_BROWSER_NEW: {
        // Start resolving new service
        AvahiServiceResolver *resolver = avahi_service_resolver_new(serviceBrowser->d_ptr->client->m_client,
                                                                    interface,
                                                                    protocol,
                                                                    name,
                                                                    type,
                                                                    domain,
                                                                    AVAHI_PROTO_UNSPEC,
                                                                    (AvahiLookupFlags) 0,
                                                                    ZeroConfServiceBrowserAvahiPrivate::callbackServiceResolver,
                                                                    serviceBrowser);
        if (resolver) {
            serviceBrowser->d_ptr->m_serviceResolvers.append(resolver);
        } else {
            qCWarning(dcPlatformZeroConf()) << "Failed to resolve service" << QString(name) << ":" << avahi_strerror(avahi_client_errno(serviceBrowser->d_ptr->client->m_client));
        }
        break;
    }
    case AVAHI_BROWSER_REMOVE: {
        // Remove the service
        foreach (const ZeroConfServiceEntry &entry, serviceBrowser->m_serviceEntries) {
            // Check not only the name, but also the protocol
            if (entry.name() == name && entry.protocol() == ZeroConfServiceBrowserAvahiPrivate::convertProtocol(protocol)) {
                serviceBrowser->m_serviceEntries.removeAll(entry);
                emit serviceBrowser->serviceEntryRemoved(entry);
            }
        }

        // Check if this was the last service for this serviceType
        foreach (const ZeroConfServiceEntry &entry, serviceBrowser->m_serviceEntries) {
            if (entry.serviceType() == type)
                return;
        }

        // This was the last service for this serviceType, lets delete the corresponding browser
        AvahiServiceBrowser *browser = serviceBrowser->d_ptr->serviceBrowserTable.take(type);
        if (browser)
            avahi_service_browser_free(browser);

        serviceBrowser->m_serviceTypes.removeAll(type);
        break;
    }
    case AVAHI_BROWSER_ALL_FOR_NOW:
        break;
    case AVAHI_BROWSER_CACHE_EXHAUSTED:
        break;
    case AVAHI_BROWSER_FAILURE:
        qCWarning(dcPlatformZeroConf()) << "Service browser error:" << QString(avahi_strerror(avahi_client_errno(serviceBrowser->d_ptr->client->m_client)));
        break;
    }

}

void ZeroConfServiceBrowserAvahiPrivate::callbackServiceResolver(AvahiServiceResolver *resolver, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const char *name, const char *type, const char *domain, const char *host_name, const AvahiAddress *address, uint16_t port, AvahiStringList *txt, AvahiLookupResultFlags flags, void *userdata)
{
    Q_UNUSED(interface);
    Q_UNUSED(type);
    Q_UNUSED(txt);

    QPointer<ZeroConfServiceBrowserAvahi> serviceBrowser = static_cast<ZeroConfServiceBrowserAvahi *>(userdata);
    if (serviceBrowser.isNull())
        return;

    switch (event) {
    case AVAHI_RESOLVER_FAILURE:
        break;
    case AVAHI_RESOLVER_FOUND: {
        char a[AVAHI_ADDRESS_STR_MAX];
        avahi_address_snprint(a, sizeof(a), address);

        // convert protocol
        QAbstractSocket::NetworkLayerProtocol networkProtocol = ZeroConfServiceBrowserAvahiPrivate::convertProtocol(protocol);
        QStringList txtList = ZeroConfServiceBrowserAvahiPrivate::convertTxtList(txt);

        // create the new resolved service entry
        ZeroConfServiceEntry entry = ZeroConfServiceEntry(name,
                                                    type,
                                                    QHostAddress(QString(a)),
                                                    QString(domain),
                                                    QString(host_name),
                                                    (quint16)port,
                                                    networkProtocol,
                                                    txtList,
                                                    flags & AVAHI_LOOKUP_RESULT_CACHED,
                                                    flags & AVAHI_LOOKUP_RESULT_WIDE_AREA,
                                                    flags & AVAHI_LOOKUP_RESULT_MULTICAST,
                                                    flags & AVAHI_LOOKUP_RESULT_LOCAL,
                                                    flags & AVAHI_LOOKUP_RESULT_OUR_OWN);

        serviceBrowser->m_serviceEntries.append(entry);
        emit serviceBrowser->serviceEntryAdded(entry);
    }
    }
    serviceBrowser->d_ptr->m_serviceResolvers.removeAll(resolver);
    avahi_service_resolver_free(resolver);

}

QStringList ZeroConfServiceBrowserAvahiPrivate::convertTxtList(AvahiStringList *txt)
{
    if (!txt)
        return QStringList();

    QStringList txtList;
    txtList.append(QString(reinterpret_cast<char *>(txt->text)));

    while (txt->next) {
        AvahiStringList *next = txt->next;
        txtList.append(QString(reinterpret_cast<char *>(next->text)));
        txt = next;
    }

    return txtList;
}

QAbstractSocket::NetworkLayerProtocol ZeroConfServiceBrowserAvahiPrivate::convertProtocol(const AvahiProtocol &protocol)
{
    QAbstractSocket::NetworkLayerProtocol networkProtocol = QAbstractSocket::UnknownNetworkLayerProtocol;

    switch (protocol) {
    case AVAHI_PROTO_INET:
        networkProtocol = QAbstractSocket::IPv4Protocol;
        break;
    case AVAHI_PROTO_INET6:
        networkProtocol = QAbstractSocket::IPv6Protocol;
        break;
    case AVAHI_PROTO_UNSPEC:
        networkProtocol = QAbstractSocket::UnknownNetworkLayerProtocol;
        break;
    }
    return networkProtocol;
}
