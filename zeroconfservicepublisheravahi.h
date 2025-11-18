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

#ifndef ZEROCONFSERVICEPUBLISHERAVAHI_H
#define ZEROCONFSERVICEPUBLISHERAVAHI_H

#include <QObject>

#include <network/zeroconf/zeroconfservicepublisher.h>

#include "qtavahiservicepublisher.h"

class ZeroConfServicePublisherAvahi : public ZeroConfServicePublisher
{
    Q_OBJECT
public:
    explicit ZeroConfServicePublisherAvahi(QtAvahiServicePublisher *publisher, QObject *parent = nullptr);

    bool registerService(const QString &name, const QHostAddress &hostAddress, const quint16 &port, const QString &serviceType, const QHash<QString, QString> &txtRecords) override;
    void unregisterService(const QString &name) override;

private:
    QtAvahiServicePublisher *m_publisher = nullptr;
};

#endif // ZEROCONFSERVICEPUBLISHERAVAHI_H
