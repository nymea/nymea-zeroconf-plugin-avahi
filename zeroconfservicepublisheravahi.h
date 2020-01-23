#ifndef ZEROCONFSERVICEPUBLISHERAVAHI_H
#define ZEROCONFSERVICEPUBLISHERAVAHI_H

#include <QObject>

#include <network/zeroconf/zeroconfservicepublisher.h>

#include "qtavahiservicepublisher.h"

class ZeroConfServicePublisherAvahi : public ZeroConfServicePublisher
{
    Q_OBJECT
public:
    explicit ZeroConfServicePublisherAvahi(QtAvahiServicePublisher *publisher, QObject *parent = nullptr);

    bool registerService(const QString &name, const QHostAddress &hostAddress, const quint16 &port, const QString &serviceType, const QHash<QString, QString> &txtRecords) override;
    void unregisterService(const QString &name) override;

private:
    QtAvahiServicePublisher *m_publisher = nullptr;
};

#endif // ZEROCONFSERVICEPUBLISHERAVAHI_H
