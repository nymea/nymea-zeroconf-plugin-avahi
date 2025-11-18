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

#include "platformzeroconfcontrolleravahi.h"
#include "zeroconfservicebrowseravahi.h"
#include "zeroconfservicepublisheravahi.h"

PlatformZeroConfPluginControllerAvahi::PlatformZeroConfPluginControllerAvahi(QObject *parent):
    PlatformZeroConfController(parent)
{
    m_avahiClient = new QtAvahiClient(this);
    m_avahiServiceBrowser = new QtAvahiServiceBrowser(m_avahiClient, this);
    m_avahiServicePublisher = new QtAvahiServicePublisher(m_avahiClient, this);

    m_servicePublisher = new ZeroConfServicePublisherAvahi(m_avahiServicePublisher, this);
}

bool PlatformZeroConfPluginControllerAvahi::available() const
{
    return true;
}

bool PlatformZeroConfPluginControllerAvahi::enabled() const
{
    return true;
}

ZeroConfServiceBrowser *PlatformZeroConfPluginControllerAvahi::createServiceBrowser(const QString &serviceType)
{
    return new ZeroConfServiceBrowserAvahi(m_avahiServiceBrowser, serviceType, this);
}

ZeroConfServicePublisher *PlatformZeroConfPluginControllerAvahi::servicePublisher() const
{
    return m_servicePublisher;
}
