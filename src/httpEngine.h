#ifndef HTTPENGINE_H
#define HTTPENGINE_H

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QHash>
#include <QPair>

class HTTPEngine : public QObject
{
    Q_OBJECT
public:
    HTTPEngine(QObject *parent = 0);

    enum GetMethod
    {
        events,
        schedule,
        scheduledates,
        languages,
        areas
    };

    GetMethod methods;

    enum EventModelType {
        InTheatres,
        ComingSoon
    };

    typedef QList<QPair<QString, QString> > queryItemList;

    void getEvents(queryItemList &queryItems, EventModelType type);
    void getSchedule(queryItemList &queryItems);
    void getScheduleDates(queryItemList &queryItems);
    void getLanguages(queryItemList &queryItems);
    void getAreas(queryItemList &queryItems);

private:
    void eventsRequest(QNetworkReply *finished, EventModelType type);
    void areasRequest(QNetworkReply *finished);
    void LanguagesRequest(QNetworkReply *finished);
    void scheduleDatesRequest(QNetworkReply *finished);
    void scheduleRequest(QNetworkReply *finished);
    QUrlQuery generateUrl(queryItemList &queryItems);
    void GET(QUrl &url, GetMethod method, EventModelType type);
    bool checkError(QNetworkReply *finished);

signals:
    void eventsReady(const QByteArray &data, HTTPEngine::EventModelType type);
    void scheludesReady(const QByteArray &data);
    void scheludeDatesReady(const QByteArray &data);
    void languagesReady(const QByteArray &data);
    void areasReady(const QByteArray &data);
    void networkError(QNetworkReply::NetworkError error);

public slots:
    void finished(QNetworkReply *reply);

private:
    QNetworkAccessManager nam_;
    QHash<QNetworkReply*, QPair<GetMethod, EventModelType> > hash_;
};

#endif // HTTPENGINE_H
