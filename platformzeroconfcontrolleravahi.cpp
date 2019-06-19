#include "platformzeroconfcontrolleravahi.h"
#include "zeroconfservicebrowseravahi.h"
#include "zeroconfservicepublisheravahi.h"

PlatformZeroConfPluginControllerAvahi::PlatformZeroConfPluginControllerAvahi(QObject *parent):
    PlatformZeroConfController(parent)
{
    m_servicePublisher = new ZeroConfServicePublisherAvahi(this);
}

bool PlatformZeroConfPluginControllerAvahi::available() const
{
    return true;
}

ZeroConfServiceBrowser *PlatformZeroConfPluginControllerAvahi::createServiceBrowser(const QString &serviceType)
{
    return new ZeroConfServiceBrowserAvahi(serviceType, this);
}

ZeroConfServicePublisher *PlatformZeroConfPluginControllerAvahi::servicePublisher() const
{
    return m_servicePublisher;
}
