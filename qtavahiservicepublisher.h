// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of nymea-zeroconf-plugin-avahi.
*
* nymea-zeroconf-plugin-avahi is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-zeroconf-plugin-avahi is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-zeroconf-plugin-avahi. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef QAVAHISERVICEPUBLISHER_H
#define QAVAHISERVICEPUBLISHER_H

#include <QObject>
#include <QHostAddress>
#include <QTimer>

#include <avahi-client/publish.h>

class QtAvahiClient;

class QtAvahiServicePublisher : public QObject
{
    Q_OBJECT
public:
    explicit QtAvahiServicePublisher(QObject *parent = nullptr);
    explicit QtAvahiServicePublisher(QtAvahiClient *client, QObject *parent = nullptr);
    ~QtAvahiServicePublisher();

    bool registerService(const QString &name, const QHostAddress &hostAddress, const quint16 &port, const QString &serviceType, const QHash<QString, QString> &txtRecords);
    void unregisterService(const QString &name);

private:
    class ServiceInfo {
    public:
        AvahiEntryGroup *group = nullptr;
        QString name;
        QString effectiveName;
        QHostAddress hostAddress;
        quint16 port;
        QString serviceType;
        QHash<QString, QString> txtRecords;
        AvahiStringList *serviceList = nullptr;
    };
    bool registerServiceInternal(ServiceInfo *info);
    void unregisterServiceInternal(ServiceInfo *info);

    bool handleCollision(ServiceInfo *info);

    static void callback(AvahiEntryGroup *group, AvahiEntryGroupState state, void *userdata);

    static AvahiStringList *createTxtList(const QHash<QString, QString> &txt);

private:
    QtAvahiClient *m_client = nullptr;
    QTimer m_reregisterTimer;

    QHash<QString, ServiceInfo*> m_services;
    QHash<AvahiEntryGroup*, ServiceInfo*> m_servicesByGroup;
};

#endif // QAVAHISERVICEPUBLISHER_H
