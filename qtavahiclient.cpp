#include "qtavahiclient.h"

#include "qt-watch.h"

#include "loggingcategories.h"

QtAvahiClient::QtAvahiClient(QObject *parent) : QObject(parent)
{
    int error = 0;
    m_client = avahi_client_new(avahi_qt_poll_get(), (AvahiClientFlags) 0, QtAvahiClient::clientCallback, this, &error);
    if (error != 0) {
        qCWarning(dcPlatformZeroConf()) << "Error creating avahi client:" << error;
    }

}

void QtAvahiClient::clientCallback(AvahiClient *client, AvahiClientState state, void *userdata)
{
    Q_UNUSED(client)
    Q_UNUSED(userdata)
    switch (state) {
    case AVAHI_CLIENT_S_RUNNING:
        qCDebug(dcPlatformZeroConf()) << "Connected to avahi";
        break;
    case AVAHI_CLIENT_FAILURE:
        qCWarning(dcPlatformZeroConf()) << "Failed to connect to avahi";
        break;
    default:
        ;
    }
}
