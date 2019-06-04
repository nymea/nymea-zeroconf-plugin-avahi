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

    bool available() const override;

    ZeroConfServiceBrowser *createServiceBrowser(const QString &serviceType = QString()) override;
    ZeroConfServicePublisher *servicePublisher() const override;

private:
    ZeroConfServicePublisherAvahi *m_servicePublisher = nullptr;
};

#endif // PLATFORMZEROCONFCONTROLLERAVAHI_H
