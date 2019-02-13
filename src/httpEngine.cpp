#include "httpEngine.h"
#include <QDebug>

HTTPEngine::HTTPEngine(QObject *parent) :
    QObject(parent)
{
    QObject::connect(&nam_, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(finished(QNetworkReply*)));

}

void HTTPEngine::getEvents(HTTPEngine::queryItemList &queryItems,
                           EventModelType type)
{
    QUrl api_url(baseUrl_ + lang_ + "/xml/Events/?" + generateUrl(queryItems));
    GET(api_url, events, type);
}

void HTTPEngine::getSchedule(HTTPEngine::queryItemList &queryItems)
{
    QUrl api_url(baseUrl_ + lang_ + "/xml/Schedule/?" + generateUrl(queryItems));
    GET(api_url, schedule, InTheatres);
}

void HTTPEngine::getScheduleDates(HTTPEngine::queryItemList &queryItems)
{
    QUrl api_url(baseUrl_ + lang_ + "/xml/ScheduleDates/?" + generateUrl(queryItems));
    GET(api_url, scheduledates, InTheatres);
}

void HTTPEngine::getLanguages()
{
    QUrl api_url(baseUrl_ + lang_ + "/xml/Languages/");
    GET(api_url, languages, InTheatres);
}

QString HTTPEngine::getCurrentLang()
{
    return lang_;
}

void HTTPEngine::getAreas()
{
    QUrl api_url(baseUrl_ + lang_ + "/xml/TheatreAreas/");
    GET(api_url, areas, InTheatres);
}

void HTTPEngine::setLanguage(QString language)
{
    lang_ = language;
}

void HTTPEngine::setLocation(SettingsManager::Country code)
{
    switch(code)
    {
    case SettingsManager::Country::FI:
        baseUrl_ = baseUrlFI;
        break;
    case SettingsManager::Country::EE:
        baseUrl_ = baseUrlEE;
        break;
    case SettingsManager::Country::LV:
        baseUrl_ = baseUrlLV;
        break;
    case SettingsManager::Country::LT:
        baseUrl_ = baseUrlLT;
        break;
    default:
        baseUrl_ = baseUrlFI;
        break;
    }
}

QString HTTPEngine::generateUrl(HTTPEngine::queryItemList &queryItems)
{
    QUrlQuery url;
    url.setQueryItems(queryItems);
    return url.toString();
}

void HTTPEngine::GET(QUrl &url,
                     HTTPEngine::GetMethod method,
                     EventModelType type)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = nam_.get(request);
    hash_[reply] = QPair<GetMethod, EventModelType>(method, type);
}

void HTTPEngine::finished(QNetworkReply *reply)
{
    if (youtubeReplies_.contains(reply)) {
        youtubeRequest(reply, youtubeReplies_[reply]);
        youtubeReplies_.remove(reply);
    }

    if(hash_.contains(reply)) {

        switch(hash_[reply].first)
        {
        case events:
            eventsRequest(reply, hash_[reply].second);
            break;
        case schedule:
            scheduleRequest(reply);
            break;
        case scheduledates:
            scheduleDatesRequest(reply);
            break;
        case languages:
            LanguagesRequest(reply);
            break;
        case areas:
            areasRequest(reply);
            break;
        }
        hash_.remove(reply);

    }
}

bool HTTPEngine::checkError(QNetworkReply *finished)
{
    if (finished->error() != QNetworkReply::NoError)
    {
        emit networkError(finished->error());
        return true;
    }
    return false;
}

void HTTPEngine::eventsRequest(QNetworkReply *finished, HTTPEngine::EventModelType type)
{
    if (checkError(finished))
        return;
    QByteArray data = finished->readAll();
    emit eventsReady(data, type);
    finished->deleteLater();
}

void HTTPEngine::scheduleRequest(QNetworkReply *finished)
{
    if (checkError(finished))
        return;
    QByteArray data = finished->readAll();
    emit scheludesReady(data);
    finished->deleteLater();
}

void HTTPEngine::scheduleDatesRequest(QNetworkReply *finished)
{
    if (checkError(finished))
        return;
    QByteArray data = finished->readAll();
    emit scheludeDatesReady(data);
    finished->deleteLater();
}

void HTTPEngine::LanguagesRequest(QNetworkReply *finished)
{
    if (checkError(finished))
        return;
    QByteArray data = finished->readAll();
    emit languagesReady(data);
    finished->deleteLater();
}

void HTTPEngine::areasRequest(QNetworkReply *finished)
{
    if (checkError(finished))
        return;
    QByteArray data = finished->readAll();
    emit areasReady(data);
    finished->deleteLater();
}

void HTTPEngine::youtubeRequest(QNetworkReply *finished, QString eventID) {

    if (checkError(finished))
        return;

    QByteArray data = finished->readAll();
    QJsonParseError err;

    // Get the url_encoded_fmt_stream_map from the json monster
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    QJsonArray all_data = doc.array();
    QJsonObject data_object = all_data.at(2).toObject();
    QJsonObject data2 = data_object[QString("data")].toObject();
    QJsonObject swfcfg = data2[QString("swfcfg")].toObject();
    QJsonObject args = swfcfg[QString("args")].toObject();
    QString url_encoded_fmt_stream_map = args[QString("url_encoded_fmt_stream_map")].toString();

    // decode the string and split
    QUrl url;
    QList<QString> dirty_urls = url.fromPercentEncoding(url_encoded_fmt_stream_map.toUtf8()).split("url=");

    // strip off some video "urls" from the query url
    // the components are not never in the same order which makes this a bit complicated
    // also I'm expecting that the high quality video is first in the array
    QString real_url;

    if (dirty_urls.length() > 0) {
        if (dirty_urls[0].contains("http")) {
            real_url = QString("url=") + dirty_urls[0];
        } else {
            if(dirty_urls.length() > 1) {
                real_url = QString("url=") + dirty_urls[1];
            }
        }
    }

    // decode the url couple if times (won't work without this step in qml
    // but the earlier url works in desktop browsers)
    QString decoded = url.fromPercentEncoding(real_url.toUtf8());
    QString decoded2 = url.fromPercentEncoding(decoded.toUtf8());

    // This is the complicated part and it's based on my experiments.
    // First we split the string into parts and check some special cases
    // in which the normal splitting did not do the job correctly
    QStringList components = decoded2.split("&");
    QStringList components2;

    // special cases
    foreach (auto component, components) {

        if(component.count(",") < 3) {

            QStringList parts = component.split(",");
            if (parts.length() == 2) {
                components2.append(parts[0]);
                components2.append(parts[1]);
            } else {
                components2.append(parts[0]);
            }

        } else if(component.contains("url=")) {

            QStringList parts = component.split("?");
            components2.append(parts[0]);
            components2.append(parts[1]);

        } else if (component.contains(",type")) {

            QStringList parts = component.split(",type");
            components2.append(parts[0]);
            components2.append(parts[1]);

        } else {
            components2.append(component);
        }
    }

    int itag_count = 0;

    // remove fallback url, video type and strings containing "+" signs from the query
    for (int i = components2.length(); i > 0; --i) {
        if(components2[i-1].contains("fallback") || components2[i-1].contains("type") || components2[i-1].contains("+")) {
            components2.removeAt(i-1);
        }
    }

    // there must be only one itag in the query
    // -> count itags
    foreach(auto component, components2) {
        if(component.contains("itag") && !component.contains(",itag,")) {
            ++itag_count;
        }
    }

    // -> loop backwards and remove all but one itag
    int i = components2.length() - 1;

    while(i > 0 && itag_count > 1) {
        if(components2[i].contains("itag") && !components2[i].contains(",itag,")) {
            components2.removeAt(i);
            --itag_count;
        } else {
            --i;
        }
    }

    int url_index = 0;

    // find the url component
    foreach(auto component, components2) {
        if(component.contains("url=")) {
            url_index = components2.indexOf(component);
            break;
        }
    }

    // rebuild the cleaned video stream url
    // it should look something like this (the url components are in random order always):
    //
    // http://r14---sn-5hn7ym7e.googlevideo.com/videoplayback?ratebypass=yes?source=youtube&pl=26
    // &initcwndbps=1067500&signature=3FED2EBD774AF7E52A59CFDE07F5165AE9E22ACC.A7DA9E81FC29B6E691488DAB30DFA91BBBEE1E26
    // &nh=EAI&ipbits=0&mm=31&expire=1425931180&sver=3&ms=au&mt=1425909493&mv=m&dur=133.607&ip=88.193.236.147
    // &key=yt5&id=o-ANgQh9fV9of_teTu48eFfO2y-W5RGPYRS3HcAcIx_4es&itag=22
    // &sparams=dur,id,initcwndbps,ip,ipbits,itag,mime,mm,ms,mv,nh,pl,ratebypass,source,upn,expire&upn=FO42xulENzM&mime=video/mp4
    // &fexp=900222,900225,907263,927622,930827,9405714,9406732,942807,943917,946008,948124,951703,952302,952612,952901,955301,957201,959701&quality=hd720

    QString url_base = components2[url_index].replace("url=", "") + QString("?");
    components2.removeAt(url_index);
    QString url_cleaned = url_base + components2.join("&");

    emit youtubeReady(url_cleaned, eventID);

    finished->deleteLater();
}

