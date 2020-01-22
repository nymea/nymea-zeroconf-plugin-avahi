#include "zeroconfservicepublisheravahi.h"

#include <loggingcategories.h>


ZeroConfServicePublisherAvahi::ZeroConfServicePublisherAvahi(QtAvahiServicePublisher *publisher, QObject *parent):
    ZeroConfServicePublisher(parent),
    m_publisher(publisher)
{

}

bool ZeroConfServicePublisherAvahi::registerService(const QString &name, const QHostAddress &hostAddress, const quint16 &port, const QString &serviceType, const QHash<QString, QString> &txtRecords)
{
    return m_publisher->registerService(name, hostAddress, port, serviceType, txtRecords);
}

void ZeroConfServicePublisherAvahi::unregisterService(const QString &name)
{
    m_publisher->unregisterService(name);
}
