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

#include "qtavahiclient.h"

#include "qt-watch.h"

#include "loggingcategories.h"

QtAvahiClient::QtAvahiClient(QObject *parent) : QObject(parent)
{
    int error = 0;
    m_client = avahi_client_new(avahi_qt_poll_get(), (AvahiClientFlags) 0, QtAvahiClient::clientCallback, this, &error);
    if (error != 0) {
        qCWarning(dcPlatformZeroConf()) << "Error creating avahi client:" << error;
    }

}

void QtAvahiClient::clientCallback(AvahiClient *client, AvahiClientState state, void *userdata)
{
    Q_UNUSED(client)
    Q_UNUSED(userdata)
    switch (state) {
    case AVAHI_CLIENT_S_RUNNING:
        qCDebug(dcPlatformZeroConf()) << "Connected to avahi";
        break;
    case AVAHI_CLIENT_FAILURE:
        qCWarning(dcPlatformZeroConf()) << "Failed to connect to avahi";
        break;
    default:
        ;
    }
}
