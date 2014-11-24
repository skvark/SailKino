#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QQmlContext>
#include <QQuickView>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QMetaType>
#include <event.h>
#include <QtQml>
#include <eventsmodel.h>
#include <showtimemodel.h>
#include <kinoAPI.h>

int main(int argc, char *argv[])
{
   qmlRegisterType<Event>("harbour.sailkino.events", 1, 0, "Event");
   qmlRegisterType<EventsModel>("harbour.sailkino.eventsmodel", 1, 0, "EventsModel");
   qmlRegisterType<ShowTimeModel>("harbour.sailkino.showtimemodel", 1, 0, "ShowTimeModel");
   QCoreApplication::setApplicationName("harbour-sailkino");
   QCoreApplication::setOrganizationName("harbour-sailkino");
   kinoAPI api;
   QGuiApplication *app = SailfishApp::application(argc, argv);
   QQuickView *view = SailfishApp::createView();
   view->rootContext()->setContextProperty("kinoAPI", &api);
   view->rootContext()->setContextProperty("APP_VERSION", APP_VERSION);
   view->setSource(SailfishApp::pathTo("qml/harbour-sailkino.qml"));
   view->showFullScreen();
   app->exec();
}

