#ifndef PLATFORMZEROCONFCONTROLLERAVAHI_H
#define PLATFORMZEROCONFCONTROLLERAVAHI_H

#include <QObject>

#include <platform/platformzeroconfcontroller.h>

class ZeroConfServiceBrowserAvahi;
class ZeroConfServicePublisherAvahi;

class PlatformZeroConfPluginControllerAvahi: public PlatformZeroConfController
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.nymea.PlatformZeroConfController")
    Q_INTERFACES(PlatformZeroConfController)
public:
    PlatformZeroConfPluginControllerAvahi(QObject *parent = nullptr);

    ZeroConfServiceBrowser *zeroConfServiceBrowser() const override;
    ZeroConfServicePublisher *zeroConfServicePublisher() const override;

private:
    ZeroConfServiceBrowserAvahi *m_serviceBrowser = nullptr;
    ZeroConfServicePublisherAvahi *m_servicePublisher = nullptr;
};

#endif // PLATFORMZEROCONFCONTROLLERAVAHI_H
