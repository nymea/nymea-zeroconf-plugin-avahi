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

ZeroConfServiceBrowser *PlatformZeroConfPluginControllerAvahi::createServiceBrowser(const QString &serviceType)
{
    return new ZeroConfServiceBrowserAvahi(m_avahiServiceBrowser, serviceType, this);
}

ZeroConfServicePublisher *PlatformZeroConfPluginControllerAvahi::servicePublisher() const
{
    return m_servicePublisher;
}
