#include "zeroconfservicepublisheravahi.h"
#include "qtavahiservice.h"

#include <loggingcategories.h>


ZeroConfServicePublisherAvahi::ZeroConfServicePublisherAvahi(QObject *parent) : ZeroConfServicePublisher(parent)
{

}

bool ZeroConfServicePublisherAvahi::registerService(const QString &id, const QString &name, const QHostAddress &hostAddress, const quint16 &port, const QString &serviceType, const QHash<QString, QString> &txtRecords)
{
    if (m_services.contains(id)) {
        qCWarning(dcPlatformZeroConf) << "Cannot register service. Service with id" << id << "already existing";
        return false;
    }
    QtAvahiService *service = new QtAvahiService(this);
//    connect(m_avahiService, &QtAvahiService::serviceStateChanged, this, &TcpServer::onAvahiServiceStateChanged);
    // Note: reversed order

    if (!service->registerService(name, hostAddress, port, serviceType, txtRecords)) {
        qCWarning(dcPlatformZeroConf) << "Failed to register service" << id << "at avahi";
        delete service;
        return false;
    }

    m_services.insert(id, service);
    return true;
}

void ZeroConfServicePublisherAvahi::unregisterService(const QString &id)
{
    if (!m_services.contains(id)) {
        qCWarning(dcPlatformZeroConf) << "Cannot unregister service. Service with id" << id << "not existinng";
        return;
    }
    QtAvahiService *service = m_services.take(id);
    service->resetService();
    service->deleteLater();
}
