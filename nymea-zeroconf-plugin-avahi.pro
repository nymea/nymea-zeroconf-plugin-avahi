TEMPLATE = lib
TARGET = $$qtLibraryTarget(nymea_zeroconfpluginavahi)

QT -= gui
QT += network

QMAKE_CXXFLAGS *= -Wno-deprecated-declaration

CONFIG += plugin link_pkgconfig c++11
PKGCONFIG += nymea avahi-client

SOURCES += platformzeroconfcontrolleravahi.cpp \
    qtavahiclient.cpp \
    qtavahiservicebrowser.cpp \
    qtavahiservicepublisher.cpp \
    zeroconfservicepublisheravahi.cpp \
    zeroconfservicebrowseravahi.cpp \
    qt-watch.cpp \

HEADERS += platformzeroconfcontrolleravahi.h \
    qtavahiclient.h \
    qtavahiservicebrowser.h \
    qtavahiservicepublisher.h \
    zeroconfservicepublisheravahi.h \
    zeroconfservicebrowseravahi.h \
    qt-watch.h \


target.path = $$[QT_INSTALL_LIBS]/nymea/platform/
INSTALLS += target
