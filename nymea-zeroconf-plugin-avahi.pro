TEMPLATE = lib
TARGET = $$qtLibraryTarget(nymea_zeroconfpluginavahi)

QT -= gui
QT += network dbus

QMAKE_CXXFLAGS *= -Werror

CONFIG += plugin link_pkgconfig c++11
PKGCONFIG += nymea avahi-client

SOURCES += platformzeroconfcontrolleravahi.cpp \
    zeroconfservicebrowseravahi.cpp \
    zeroconfservicebrowseravahi_p.cpp \
    qtavahiclient.cpp \
    qt-watch.cpp \
    qtavahiservice.cpp \
    qtavahiservice_p.cpp \
    zeroconfservicepublisheravahi.cpp


HEADERS += platformzeroconfcontrolleravahi.h \
    zeroconfservicebrowseravahi.h \
    zeroconfservicebrowseravahi_p.h \
    qtavahiclient.h \
    qt-watch.h \
    qtavahiservice.h \
    qtavahiservice_p.h \
    zeroconfservicepublisheravahi.h


target.path = $$[QT_INSTALL_LIBS]/nymea/platform/
INSTALLS += target
