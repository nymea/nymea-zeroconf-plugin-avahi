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

#ifndef ZEROCONFSERVICEBROWSERAVAHI_H
#define ZEROCONFSERVICEBROWSERAVAHI_H

#include <QObject>

#include "qtavahiservicebrowser.h"

#include "network/zeroconf/zeroconfserviceentry.h"
#include "network/zeroconf/zeroconfservicebrowser.h"

class ZeroConfServiceBrowserAvahiPrivate;

class ZeroConfServiceBrowserAvahi : public ZeroConfServiceBrowser
{
    Q_OBJECT

public:
    explicit ZeroConfServiceBrowserAvahi(QtAvahiServiceBrowser *avahiBrowser, const QString &serviceType = QString(), QObject *parent = nullptr);
    ~ZeroConfServiceBrowserAvahi() override;

    QList<ZeroConfServiceEntry> serviceEntries() const override;

private:
    QString m_serviceType;

    QtAvahiServiceBrowser *m_avahiBrowser = nullptr;

};

#endif // ZEROCONFSERVICEBROWSERAVAHI_H
