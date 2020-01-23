#include "qtavahiservicebrowser.h"

#include <loggingcategories.h>

#include <avahi-common/strlst.h>
#include <avahi-common/error.h>


QtAvahiServiceBrowser::QtAvahiServiceBrowser(QObject *parent): QObject(parent)
{
    m_client = new QtAvahiClient(this);
    m_serviceTypeBrowser = avahi_service_type_browser_new(m_client->m_client, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, 0, (AvahiLookupFlags) 0, QtAvahiServiceBrowser::serviceTypeBrowserCallback, this);
}

QtAvahiServiceBrowser::QtAvahiServiceBrowser(QtAvahiClient *client, QObject *parent):
    QObject(parent),
    m_client(client)
{
    m_serviceTypeBrowser = avahi_service_type_browser_new(m_client->m_client, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, 0, (AvahiLookupFlags) 0, QtAvahiServiceBrowser::serviceTypeBrowserCallback, this);
}

QtAvahiServiceBrowser::~QtAvahiServiceBrowser()
{
    foreach (AvahiServiceResolver *resolver, m_resolvers.toList()) {
        avahi_service_resolver_free(resolver);
    }
    m_resolvers.clear();

    while (!m_serviceBrowsers.isEmpty()) {
        AvahiServiceBrowser *browser = m_serviceBrowsers.keys().first();
        m_serviceBrowsers.take(browser);
        avahi_service_browser_free(browser);
    }

    avahi_service_type_browser_free(m_serviceTypeBrowser);
}

QList<ZeroConfServiceEntry> QtAvahiServiceBrowser::entries()
{
    return m_entries;
}

void QtAvahiServiceBrowser::registerServiceBrowser(const QString &serviceType, const QString &domain, AvahiIfIndex interface, AvahiProtocol protocol)
{
    AvahiServiceBrowser* browser = avahi_service_browser_new(m_client->m_client,
                                                             interface,
                                                             protocol,
                                                             serviceType.toUtf8().data(),
                                                             domain.toUtf8().data(),
                                                             (AvahiLookupFlags) 0,
                                                             QtAvahiServiceBrowser::serviceBrowserCallback,
                                                             this);
    BrowserInfo info;
    info.type = serviceType;
    info.domain = domain;
    info.interface = interface;
    info.protocol = protocol;
    m_serviceBrowsers.insert(browser, info);
}

void QtAvahiServiceBrowser::unregisterServiceBrowser(const QString &serviceType, const QString &domain, AvahiIfIndex interface, AvahiProtocol protocol)
{
    foreach (AvahiServiceBrowser *browser, m_serviceBrowsers.keys()) {
        BrowserInfo info = m_serviceBrowsers.value(browser);
        if (info.type == serviceType && info.domain == domain && info.interface == interface && info.protocol == protocol) {
            m_serviceBrowsers.remove(browser);
            avahi_service_browser_free(browser);
        }
    }
}

void QtAvahiServiceBrowser::registerServiceResolver(const QString &name, const QString &type, const QString &domain, AvahiIfIndex interface, AvahiProtocol protocol)
{
    AvahiServiceResolver *resolver = avahi_service_resolver_new(m_client->m_client,
                                                                interface,
                                                                protocol,
                                                                name.toUtf8().data(),
                                                                type.toUtf8().data(),
                                                                domain.toUtf8().data(),
                                                                AVAHI_PROTO_UNSPEC,
                                                                (AvahiLookupFlags) 0,
                                                                QtAvahiServiceBrowser::serviceResolverCallback,
                                                                this);
    if (!resolver) {
        qCWarning(dcPlatformZeroConf()) << "Failed to resolve service" << QString(name) << ":" << avahi_strerror(avahi_client_errno(m_client->m_client));
        return;
    }

    m_resolvers.insert(resolver);
}


void QtAvahiServiceBrowser::serviceTypeBrowserCallback(AvahiServiceTypeBrowser *browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *type, const char *domain, AvahiLookupResultFlags flags, void *userdata)
{
    Q_UNUSED(browser)
    Q_UNUSED(flags)

    QtAvahiServiceBrowser *instance = static_cast<QtAvahiServiceBrowser*>(userdata);

    switch (event) {
    case AVAHI_BROWSER_NEW:
        instance->registerServiceBrowser(type, domain, interface, protocol);
        break;
    case AVAHI_BROWSER_REMOVE:
        instance->unregisterServiceBrowser(type, domain, interface, protocol);
        break;
    case AVAHI_BROWSER_CACHE_EXHAUSTED:
        break;
    case AVAHI_BROWSER_ALL_FOR_NOW:
        break;
    case AVAHI_BROWSER_FAILURE:
        qCWarning(dcPlatformZeroConf()) << "Service type browser error:" << QString(avahi_strerror(avahi_client_errno(instance->m_client->m_client)));
        break;
    }

}

void QtAvahiServiceBrowser::serviceBrowserCallback(AvahiServiceBrowser *browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, const char *type, const char *domain, AvahiLookupResultFlags flags, void *userdata)
{
    Q_UNUSED(browser)
    Q_UNUSED(flags)

    QtAvahiServiceBrowser *instance = static_cast<QtAvahiServiceBrowser*>(userdata);

    switch (event) {
    case AVAHI_BROWSER_NEW: {
        // Start resolving new service
        instance->registerServiceResolver(name, type, domain, interface, protocol);
        break;
    }
    case AVAHI_BROWSER_REMOVE: {
        QMutableListIterator<ZeroConfServiceEntry> i(instance->m_entries);
        while (i.hasNext()) {
            ZeroConfServiceEntry entry = i.next();
            // FIXME: In theory we'd need to compare interface too, but nymea's zeroconfentry doesn't store that information
            if (entry.name() == name && entry.serviceType() == type && entry.domain() == domain && entry.protocol() == convertProtocol(protocol)) {
                i.remove();
//                qCDebug(dcPlatformZeroConf()) << "Service removed:" << entry;
                emit instance->serviceRemoved(entry);
            }
        }
        break;
    }
    case AVAHI_BROWSER_ALL_FOR_NOW:
        break;
    case AVAHI_BROWSER_CACHE_EXHAUSTED:
        break;
    case AVAHI_BROWSER_FAILURE:
        qCWarning(dcPlatformZeroConf()) << "Service browser error:" << QString(avahi_strerror(avahi_client_errno(instance->m_client->m_client)));
        break;
    }

}

void QtAvahiServiceBrowser::serviceResolverCallback(AvahiServiceResolver *resolver, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const char *name, const char *type, const char *domain, const char *host_name, const AvahiAddress *address, uint16_t port, AvahiStringList *txt, AvahiLookupResultFlags flags, void *userdata)
{
    Q_UNUSED(interface)

    QtAvahiServiceBrowser *instance = static_cast<QtAvahiServiceBrowser*>(userdata);

    switch (event) {
    case AVAHI_RESOLVER_FAILURE:
        break;
    case AVAHI_RESOLVER_FOUND: {
        char a[AVAHI_ADDRESS_STR_MAX];
        avahi_address_snprint(a, sizeof(a), address);
        QHostAddress hostAddress = QHostAddress(QString(a));

        ZeroConfServiceEntry entry(name,
                                   type,
                                   hostAddress,
                                   domain,
                                   host_name,
                                   port,
                                   convertProtocol(protocol),
                                   convertTxtList(txt),
                                   flags & AVAHI_LOOKUP_RESULT_CACHED,
                                   flags & AVAHI_LOOKUP_RESULT_WIDE_AREA,
                                   flags & AVAHI_LOOKUP_RESULT_MULTICAST,
                                   flags & AVAHI_LOOKUP_RESULT_LOCAL,
                                   flags & AVAHI_LOOKUP_RESULT_OUR_OWN);


        if (!instance->m_entries.contains(entry)) {
            instance->m_entries.append(entry);
            qCDebug(dcPlatformZeroConf()) << "Service added:" << entry;
            emit instance->serviceAdded(entry);
        }
        break;
    }
    }

    instance->m_resolvers.remove(resolver);
    avahi_service_resolver_free(resolver);
}


QStringList QtAvahiServiceBrowser::convertTxtList(AvahiStringList *txt)
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

QAbstractSocket::NetworkLayerProtocol QtAvahiServiceBrowser::convertProtocol(const AvahiProtocol &protocol)
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
