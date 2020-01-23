#ifndef QAVAHISERVICEPUBLISHER_H
#define QAVAHISERVICEPUBLISHER_H

#include <QObject>
#include <QHostAddress>
#include <QTimer>

#include <avahi-client/publish.h>

class QtAvahiClient;

class QtAvahiServicePublisher : public QObject
{
    Q_OBJECT
public:
    explicit QtAvahiServicePublisher(QObject *parent = nullptr);
    explicit QtAvahiServicePublisher(QtAvahiClient *client, QObject *parent = nullptr);
    ~QtAvahiServicePublisher();

    bool registerService(const QString &name, const QHostAddress &hostAddress, const quint16 &port, const QString &serviceType, const QHash<QString, QString> &txtRecords);
    void unregisterService(const QString &name);

private:
    class ServiceInfo {
    public:
        AvahiEntryGroup *group = nullptr;
        QString name;
        QString effectiveName;
        QHostAddress hostAddress;
        quint16 port;
        QString serviceType;
        QHash<QString, QString> txtRecords;
        AvahiStringList *serviceList = nullptr;
    };
    bool registerServiceInternal(ServiceInfo *info);
    void unregisterServiceInternal(ServiceInfo *info);

    bool handleCollision(ServiceInfo *info);

    static void callback(AvahiEntryGroup *group, AvahiEntryGroupState state, void *userdata);

    static AvahiStringList *createTxtList(const QHash<QString, QString> &txt);

private:
    QtAvahiClient *m_client = nullptr;
    QTimer m_reregisterTimer;

    QHash<QString, ServiceInfo*> m_services;
    QHash<AvahiEntryGroup*, ServiceInfo*> m_servicesByGroup;
};

#endif // QAVAHISERVICEPUBLISHER_H
