/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Copyright (C) 2016 Simon Stürz <simon.stuerz@guh.io>                   *
 *                                                                         *
 *  This file is part of nymea.                                            *
 *                                                                         *
 *  This library is free software; you can redistribute it and/or          *
 *  modify it under the terms of the GNU Lesser General Public             *
 *  License as published by the Free Software Foundation; either           *
 *  version 2.1 of the License, or (at your option) any later version.     *
 *                                                                         *
 *  This library is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
 *  Lesser General Public License for more details.                        *
 *                                                                         *
 *  You should have received a copy of the GNU Lesser General Public       *
 *  License along with this library; If not, see                           *
 *  <http://www.gnu.org/licenses/>.                                        *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ZEROCONFSERVICEBROWSERAVAHI_H
#define ZEROCONFSERVICEBROWSERAVAHI_H

#include <QObject>
#include <avahi-client/lookup.h>

#include "qtavahiclient.h"

#include "network/zeroconf/zeroconfserviceentry.h"
#include "network/zeroconf/zeroconfservicebrowser.h"


class ZeroConfServiceBrowserAvahiPrivate;

class ZeroConfServiceBrowserAvahi : public ZeroConfServiceBrowser
{
    Q_OBJECT

public:
    explicit ZeroConfServiceBrowserAvahi(const QString &serviceType = QString(), QObject *parent = nullptr);
    ~ZeroConfServiceBrowserAvahi() override;

    QList<ZeroConfServiceEntry> serviceEntries() const override;

private slots:
    void onClientStateChanged(const QtAvahiClient::QtAvahiClientState &state);

private:
    QString m_serviceType;

    ZeroConfServiceBrowserAvahiPrivate *d_ptr;

    QList<ZeroConfServiceEntry> m_serviceEntries;

    void createServiceBrowser(const char* serviceType);

    Q_DECLARE_PRIVATE(ZeroConfServiceBrowserAvahi)
};

#endif // ZEROCONFSERVICEBROWSERAVAHI_H
