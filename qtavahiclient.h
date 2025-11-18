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

#ifndef AVAHICLIENT_H
#define AVAHICLIENT_H

#include <QObject>

#include <avahi-client/client.h>

class QtAvahiClient : public QObject
{
    Q_OBJECT
public:    
    explicit QtAvahiClient(QObject *parent = nullptr);

private:
    static void clientCallback(AvahiClient *client, AvahiClientState state, void *userdata);
    static void serviceBrowserCallback();

private:
    friend class QtAvahiServiceBrowser;
    friend class QtAvahiServicePublisher;
    AvahiClient *m_client = nullptr;
};

#endif // AVAHICLIENT_H
