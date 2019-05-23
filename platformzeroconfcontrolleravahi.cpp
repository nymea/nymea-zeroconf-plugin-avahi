#include "platformzeroconfcontrolleravahi.h"
#include "zeroconfservicebrowseravahi.h"
#include "zeroconfservicepublisheravahi.h"

PlatformZeroConfPluginControllerAvahi::PlatformZeroConfPluginControllerAvahi(QObject *parent):
    PlatformZeroConfController(parent)
{
    m_serviceBrowser = new ZeroConfServiceBrowserAvahi(this);
    m_servicePublisher = new ZeroConfServicePublisherAvahi(this);
}

ZeroConfServiceBrowser *PlatformZeroConfPluginControllerAvahi::zeroConfServiceBrowser() const
{
    return m_serviceBrowser;
}

ZeroConfServicePublisher *PlatformZeroConfPluginControllerAvahi::zeroConfServicePublisher() const
{
    return  m_servicePublisher;
}
