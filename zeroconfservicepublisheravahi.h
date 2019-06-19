#ifndef ZEROCONFSERVICEPUBLISHERAVAHI_H
#define ZEROCONFSERVICEPUBLISHERAVAHI_H

#include <QObject>

#include <network/zeroconf/zeroconfservicepublisher.h>

class QtAvahiService;

class ZeroConfServicePublisherAvahi : public ZeroConfServicePublisher
{
    Q_OBJECT
public:
    explicit ZeroConfServicePublisherAvahi(QObject *parent = nullptr);

    bool registerService(const QString &name, const QHostAddress &hostAddress, const quint16 &port, const QString &serviceType, const QHash<QString, QString> &txtRecords) override;
    void unregisterService(const QString &name) override;

private:
    QHash<QString, QtAvahiService*> m_services;
};

#endif // ZEROCONFSERVICEPUBLISHERAVAHI_H
