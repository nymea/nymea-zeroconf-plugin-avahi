#ifndef AVAHISERVICEBROWSER_H
#define AVAHISERVICEBROWSER_H

#include <QObject>
#include <QHash>
#include <QHostAddress>

#include <network/zeroconf/zeroconfserviceentry.h>

#include <avahi-client/lookup.h>

#include "qtavahiclient.h"


class QtAvahiServiceBrowser: public QObject
{
    Q_OBJECT
public:
    QtAvahiServiceBrowser(QObject *parent = nullptr);
    QtAvahiServiceBrowser(QtAvahiClient* client, QObject *parent = nullptr);
    ~QtAvahiServiceBrowser();

    QList<ZeroConfServiceEntry> entries();

signals:
    void serviceAdded(const ZeroConfServiceEntry &entry);
    void serviceRemoved(const ZeroConfServiceEntry &entry);

private:
    void registerServiceBrowser(const QString &serviceType, const QString &domain, AvahiIfIndex interface, AvahiProtocol protocol);
    void unregisterServiceBrowser(const QString &serviceType, const QString &domain, AvahiIfIndex interface, AvahiProtocol protocol);

    void registerServiceResolver(const QString &name, const QString &type, const QString &domain, AvahiIfIndex interface, AvahiProtocol protocol);

    static void serviceTypeBrowserCallback(AvahiServiceTypeBrowser *browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *type, const char *domain, AvahiLookupResultFlags flags, void *userdata);
    static void serviceBrowserCallback(AvahiServiceBrowser *browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, const char *type, const char *domain, AvahiLookupResultFlags flags, void *userdata);
    static void serviceResolverCallback(AvahiServiceResolver *resolver, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const char *name, const char *type, const char *domain, const char *host_name, const AvahiAddress *address, uint16_t port, AvahiStringList *txt, AvahiLookupResultFlags flags, void *userdata);

    static QStringList convertTxtList(AvahiStringList *txt);
    static QAbstractSocket::NetworkLayerProtocol convertProtocol(const AvahiProtocol &protocol);

private:
    QtAvahiClient *m_client = nullptr;

    AvahiServiceTypeBrowser *m_serviceTypeBrowser = nullptr;

    struct BrowserInfo {
        QString type;
        QString domain;
        AvahiIfIndex interface;
        AvahiProtocol protocol;
    };
    QHash<AvahiServiceBrowser*, BrowserInfo> m_serviceBrowsers;

    QSet<AvahiServiceResolver*> m_resolvers;

    QList<ZeroConfServiceEntry> m_entries;
};

#endif // AVAHISERVICEBROWSER_H
