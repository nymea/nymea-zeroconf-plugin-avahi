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
