# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed


TARGET = harbour-sailkino
CONFIG += sailfishapp sailfishapp_i18n
CONFIG += c++11

QT += network dbus

INCLUDEPATH += src/

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += src/SailKino.cpp \
    src/httpEngine.cpp \
    src/settings.cpp \
    src/eventsmodel.cpp \
    src/kinoAPI.cpp \
    src/parser.cpp \
    src/event.cpp \
    src/show.cpp \
    src/showtimemodel.cpp \
    src/schedulefiltermodel.cpp


HEADERS += \
    src/httpEngine.h \
    src/settings.h \
    src/eventsmodel.h \
    src/kinoAPI.h \
    src/parser.h \
    src/event.h \
    src/show.h \
    src/showtimemodel.h \
    src/schedulefiltermodel.h

DISTFILES += \
    rpm/harbour-sailkino.spec \
    translations/*.ts \
    qml/cover/CoverPage.qml \
    harbour-sailkino.desktop \
    qml/harbour-sailkino.qml \
    qml/pages/Events.qml \
    qml/pages/EventsPage.qml \
    qml/pages/SingleEvent.qml \
    qml/pages/SearchPage.qml \
    qml/pages/SelectLocationLanguage.qml \
    qml/pages/Settings.qml \
    qml/pages/About.qml

TRANSLATIONS += \
    translations/harbour-sailkino-fi.ts
