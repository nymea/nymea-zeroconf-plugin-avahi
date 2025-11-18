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

#ifndef PLATFORMZEROCONFCONTROLLERAVAHI_H
#define PLATFORMZEROCONFCONTROLLERAVAHI_H

#include <QObject>

#include <platform/platformzeroconfcontroller.h>

class ZeroConfServiceBrowserAvahi;
class ZeroConfServicePublisherAvahi;

class QtAvahiClient;
class QtAvahiServiceBrowser;
class QtAvahiServicePublisher;

class PlatformZeroConfPluginControllerAvahi: public PlatformZeroConfController
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.nymea.PlatformZeroConfController")
    Q_INTERFACES(PlatformZeroConfController)
public:
    PlatformZeroConfPluginControllerAvahi(QObject *parent = nullptr);

    bool available() const override;
    bool enabled() const override;

    ZeroConfServiceBrowser *createServiceBrowser(const QString &serviceType = QString()) override;
    ZeroConfServicePublisher *servicePublisher() const override;

private:
    QtAvahiClient *m_avahiClient = nullptr;
    QtAvahiServiceBrowser *m_avahiServiceBrowser = nullptr;
    QtAvahiServicePublisher *m_avahiServicePublisher = nullptr;

    ZeroConfServicePublisherAvahi *m_servicePublisher = nullptr;
};

#endif // PLATFORMZEROCONFCONTROLLERAVAHI_H
