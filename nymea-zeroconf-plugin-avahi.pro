TEMPLATE = lib
TARGET = $$qtLibraryTarget(nymea_zeroconfpluginavahi)

QT -= gui
QT += network

greaterThan(QT_MAJOR_VERSION, 5) {
    message("Building using Qt6 support")
    CONFIG *= c++17
    QMAKE_LFLAGS *= -std=c++17
    QMAKE_CXXFLAGS *= -std=c++17
} else {
    message("Building using Qt5 support")
    CONFIG *= c++11
    QMAKE_LFLAGS *= -std=c++11
    QMAKE_CXXFLAGS *= -std=c++11
    DEFINES += QT_DISABLE_DEPRECATED_UP_TO=0x050F00
}

QMAKE_CXXFLAGS *= -Wno-deprecated-declaration

CONFIG += plugin link_pkgconfig
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
