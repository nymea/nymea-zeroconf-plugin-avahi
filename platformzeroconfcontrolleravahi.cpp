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
