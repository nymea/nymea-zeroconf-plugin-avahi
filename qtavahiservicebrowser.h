/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by
* copyright law, and remains the property of nymea GmbH. All rights, including
* reproduction, publication, editing and translation, are reserved. The use of
* this project is subject to the terms of a license agreement to be concluded
* with nymea GmbH in accordance with the terms of use of nymea GmbH, available
* under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; version 3. This project is distributed in the hope that
* it will be useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this project. If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under
* contact@nymea.io or see our FAQ/Licensing Information on
* https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
