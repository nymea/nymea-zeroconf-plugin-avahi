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

#include "zeroconfservicebrowseravahi.h"
#include "loggingcategories.h"

ZeroConfServiceBrowserAvahi::ZeroConfServiceBrowserAvahi(QtAvahiServiceBrowser *avahiBrowser, const QString &serviceType, QObject *parent) :
    ZeroConfServiceBrowser(serviceType, parent),
    m_serviceType(serviceType),
    m_avahiBrowser(avahiBrowser)
{
    connect(m_avahiBrowser, &QtAvahiServiceBrowser::serviceAdded, this, [=](const ZeroConfServiceEntry &entry){
        if (entry.serviceType() == m_serviceType) {
            emit serviceEntryAdded(entry);
        }
    });
    connect(m_avahiBrowser, &QtAvahiServiceBrowser::serviceRemoved, this, [=](const ZeroConfServiceEntry &entry){
        if (entry.serviceType() == m_serviceType) {
            emit serviceEntryRemoved(entry);
        }
    });
}

ZeroConfServiceBrowserAvahi::~ZeroConfServiceBrowserAvahi()
{
}

QList<ZeroConfServiceEntry> ZeroConfServiceBrowserAvahi::serviceEntries() const
{
    if (m_serviceType.isEmpty()) {
        return m_avahiBrowser->entries();
    }
    QList<ZeroConfServiceEntry> ret;
    foreach (const ZeroConfServiceEntry &entry, m_avahiBrowser->entries()) {
        if (entry.serviceType() == m_serviceType) {
            ret.append(entry);
        }
    }
    return ret;
}

