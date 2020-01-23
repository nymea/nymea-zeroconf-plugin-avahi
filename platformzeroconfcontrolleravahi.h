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

    ZeroConfServiceBrowser *createServiceBrowser(const QString &serviceType = QString()) override;
    ZeroConfServicePublisher *servicePublisher() const override;

private:
    QtAvahiClient *m_avahiClient = nullptr;
    QtAvahiServiceBrowser *m_avahiServiceBrowser = nullptr;
    QtAvahiServicePublisher *m_avahiServicePublisher = nullptr;

    ZeroConfServicePublisherAvahi *m_servicePublisher = nullptr;
};

#endif // PLATFORMZEROCONFCONTROLLERAVAHI_H
