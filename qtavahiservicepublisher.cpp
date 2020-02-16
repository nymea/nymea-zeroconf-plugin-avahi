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

#include "qtavahiservicepublisher.h"
#include "qtavahiclient.h"

#include <QNetworkInterface>

#include <loggingcategories.h>

#include <avahi-common/error.h>
#include <avahi-common/alternative.h>

QtAvahiServicePublisher::QtAvahiServicePublisher(QObject *parent):
    QtAvahiServicePublisher(new QtAvahiClient, parent)
{
}

QtAvahiServicePublisher::QtAvahiServicePublisher(QtAvahiClient *client, QObject *parent):
    QObject(parent),
    m_client(client)
{
    // Reregister every minute in order to work around low quality network hardware which
    // doesn't properly keep multicast sessions alive.
    // https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=736641
    m_reregisterTimer.setInterval(60000);
    m_reregisterTimer.setSingleShot(false);
    connect(&m_reregisterTimer, &QTimer::timeout, this, [this](){
        foreach (ServiceInfo *info, m_services) {
            qCDebug(dcPlatformZeroConf()) << "Re-registering avahi service" << info->name;
            unregisterServiceInternal(info);
            registerServiceInternal(info);
        }
    });
}

QtAvahiServicePublisher::~QtAvahiServicePublisher()
{
}

bool QtAvahiServicePublisher::registerService(const QString &name, const QHostAddress &hostAddress, const quint16 &port, const QString &serviceType, const QHash<QString, QString> &txtRecords)
{
    if (m_services.contains(name)) {
        qCWarning(dcPlatformZeroConf()) << "Service" << name << "already registered";
        return false;
    }
    // Cache all values locally at first
    ServiceInfo *info = new ServiceInfo();
    info->name = name;
    info->effectiveName = name;
    info->hostAddress = hostAddress;
    info->port = port;
    info->serviceType = serviceType;
    info->txtRecords = txtRecords;
    info->group = avahi_entry_group_new(m_client->m_client, QtAvahiServicePublisher::callback, this);

    m_services.insert(name, info);
    m_servicesByGroup.insert(info->group, info);

    registerServiceInternal(info);

    m_reregisterTimer.start();

    return true;
}

void QtAvahiServicePublisher::unregisterService(const QString &name)
{
    if (!m_services.contains(name)) {
        qCWarning(dcPlatformZeroConf()) << "Cannot unregister service. Service not registered" << name;
        return;
    }
    qCDebug(dcPlatformZeroConf()) << "Unregistering service" << name;

    ServiceInfo *info = m_services.take(name);
    m_servicesByGroup.remove(info->group);
    unregisterServiceInternal(info);
    avahi_entry_group_free(info->group);
    delete info;

    if (m_services.isEmpty()) {
        m_reregisterTimer.stop();
    }
}

bool QtAvahiServicePublisher::registerServiceInternal(ServiceInfo *info)
{
    // Check if the client is running
    if (!m_client->m_client || AVAHI_CLIENT_S_RUNNING != avahi_client_get_state(m_client->m_client)) {
        qCWarning(dcPlatformZeroConf()) << "Could not register service" << info->name << info->port << info->serviceType << ". The client is not available.";
        return false;
    }

    // Add the service
    AvahiIfIndex ifIndex = AVAHI_IF_UNSPEC;
    if (info->hostAddress != QHostAddress("0.0.0.0")) {
        foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces()) {
            foreach (const QNetworkAddressEntry &addressEntry, interface.addressEntries()) {
                QPair<QHostAddress, int> subnet = QHostAddress::parseSubnet(addressEntry.ip().toString() + "/" + addressEntry.netmask().toString());
                if (info->hostAddress.isInSubnet(subnet.first, subnet.second)) {
                    ifIndex = interface.index();
                    break;
                }
            }
        }
    }

    info->serviceList = createTxtList(info->txtRecords);

    int error = avahi_entry_group_add_service_strlst(info->group,
                                                     ifIndex,
                                                     info->hostAddress.protocol() == QAbstractSocket::IPv6Protocol ? AVAHI_PROTO_INET6 : AVAHI_PROTO_INET,
                                                     (AvahiPublishFlags) 0,
                                                     info->effectiveName.toUtf8().data(),
                                                     info->serviceType.toUtf8().data(),
                                                     0,
                                                     0,
                                                     info->port,
                                                     info->serviceList);

    if (error) {
        if (error == AVAHI_ERR_COLLISION) {
            if (!handleCollision(info)) {
                qCWarning(dcPlatformZeroConf()) << this << "error:" << avahi_strerror(error);
                return false;
            }
        } else {
            qCWarning(dcPlatformZeroConf()) << this << "error:" << avahi_strerror(error);
            return false;
        }
    }

    error = avahi_entry_group_commit(info->group);
    if (error) {
        qCWarning(dcPlatformZeroConf()) << this << "error:" << avahi_strerror(error);
        return false;
    }

    return true;

}

void QtAvahiServicePublisher::unregisterServiceInternal(ServiceInfo *info)
{
    if (info->serviceList) {
        avahi_string_list_free(info->serviceList);
        info->serviceList = nullptr;
    }
    avahi_entry_group_reset(info->group);
}

bool QtAvahiServicePublisher::handleCollision(ServiceInfo *info)
{
    qCDebug(dcPlatformZeroConf()) << "Handling collision for service" << info->name;

   char* alt = avahi_alternative_service_name(info->effectiveName.toUtf8().data());
   info->effectiveName = QLatin1String(alt);
   free(alt);

   qCDebug(dcPlatformZeroConf()) << "Service name collision. Picking alternative service name" << info->effectiveName;

   unregisterServiceInternal(info);
   return registerServiceInternal(info);
}

void QtAvahiServicePublisher::callback(AvahiEntryGroup *group, AvahiEntryGroupState state, void *userdata)
{
    QtAvahiServicePublisher *instance = static_cast<QtAvahiServicePublisher*>(userdata);
    ServiceInfo *info = instance->m_servicesByGroup.value(group);

    switch (state) {
    case AVAHI_ENTRY_GROUP_UNCOMMITED:
        break;
    case AVAHI_ENTRY_GROUP_REGISTERING:
        break;
    case AVAHI_ENTRY_GROUP_ESTABLISHED:
        if (info->name != info->effectiveName) {
            qCDebug(dcPlatformZeroConf()) << "Service registered:" << info->name << "as" << info->effectiveName;
        } else {
            qCDebug(dcPlatformZeroConf()) << "Service registered:" << info->name;
        }
        break;
    case AVAHI_ENTRY_GROUP_COLLISION:
        instance->handleCollision(instance->m_servicesByGroup.value(group));
        break;
    case AVAHI_ENTRY_GROUP_FAILURE:
        qCWarning(dcPlatformZeroConf()) << "Failed to register ZeroConf service" << instance->m_servicesByGroup.value(group)->name << "at avahi";
        break;
    }
}

AvahiStringList *QtAvahiServicePublisher::createTxtList(const QHash<QString, QString> &txt)
{
    AvahiStringList *list = nullptr;
    if (txt.isEmpty())
        return list;

    const QStringList keys = txt.keys();
    list = avahi_string_list_new((keys.first() + '=' + txt[keys.first()]).toLatin1().constData(), nullptr);
    for (const QString &key : keys.mid(1)) {
        list = avahi_string_list_add_pair(list, key.toLatin1().constData(), txt[key].toLatin1().constData());
    }

    return list;
}

