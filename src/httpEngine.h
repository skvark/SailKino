#ifndef HTTPENGINE_H
#define HTTPENGINE_H

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QHash>
#include <QMap>
#include <QPair>
#include <settings.h>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSet>

namespace {
    const QString baseUrlFI = "http://www.finnkino.fi";
    const QString baseUrlEE = "http://www.forumcinemas.ee";
    const QString baseUrlLV = "http://www.forumcinemas.lv";
    const QString baseUrlLT = "http://www.forumcinemas.lt";
}

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
    void getLanguages();
    QString getCurrentLang();
    void getAreas();
    void setLocation(SettingsManager::Country country);
    void setLanguage(QString language);

private:
    void eventsRequest(QNetworkReply *finished, EventModelType type);
    void areasRequest(QNetworkReply *finished);
    void LanguagesRequest(QNetworkReply *finished);
    void scheduleDatesRequest(QNetworkReply *finished);
    void scheduleRequest(QNetworkReply *finished);
    void youtubeRequest(QNetworkReply *finished, QString eventID);
    QString generateUrl(queryItemList &queryItems);
    void GET(QUrl &url, GetMethod method, EventModelType type);
    bool checkError(QNetworkReply *finished);

signals:
    void eventsReady(const QByteArray &data, HTTPEngine::EventModelType type);
    void scheludesReady(const QByteArray &data);
    void scheludeDatesReady(const QByteArray &data);
    void languagesReady(const QByteArray &data);
    void areasReady(const QByteArray &data);
    void youtubeReady(QString url, QString id);
    void networkError(QNetworkReply::NetworkError error);

public slots:
    void getYoutubeVideoInfo(QString video_id, QString event_id);
    void finished(QNetworkReply *reply);

private:
    QNetworkAccessManager nam_;
    QHash<QNetworkReply*, QPair<GetMethod, EventModelType> > hash_;
    QMap<QNetworkReply*, QString> youtubeReplies_;
    QString baseUrl_;
    QString lang_;
};

#endif // HTTPENGINE_H
