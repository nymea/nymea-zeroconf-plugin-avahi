#ifndef AVAHICLIENT_H
#define AVAHICLIENT_H

#include <QObject>

#include <avahi-client/client.h>

class QtAvahiClient : public QObject
{
    Q_OBJECT
public:    
    explicit QtAvahiClient(QObject *parent = nullptr);

private:
    static void clientCallback(AvahiClient *client, AvahiClientState state, void *userdata);
    static void serviceBrowserCallback();

private:
    friend class QtAvahiServiceBrowser;
    friend class QtAvahiServicePublisher;
    AvahiClient *m_client = nullptr;
};

#endif // AVAHICLIENT_H
