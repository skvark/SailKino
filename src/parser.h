#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QList>
#include <httpEngine.h>
#include <QVector>
#include <QTextCodec>
#include <QNetworkReply>
#include <QDate>
#include <event.h>
#include <eventsmodel.h>
#include <QXmlStreamReader>
#include <QDate>
#include <settings.h>
#include <QDateTime>
#include <QPair>

class Parser : public QObject
{
    Q_OBJECT

public:
    Parser(QObject *parent = 0);
    ~Parser();

    void addNewModel(HTTPEngine::EventModelType, EventsModel *model);
    void parseEvents(QString area);
    void getSchedules(QString area, QDate date);
    EventsModel *getModel(HTTPEngine::EventModelType);
    void parseEvent(QXmlStreamReader &xml, HTTPEngine::EventModelType);
    void parseArea(QXmlStreamReader &xml);
    QString parseElement(QXmlStreamReader &xml) const;
    Event *getEvent(QString id);
    void parseSoonEvents();
    void getAreas();
    void getLanguages();
    QString getAreaName(QString id);
    QString getAreaID(QString area);
    QStringList getAreasList();
    void setLocation(SettingsManager::Country country);
    void clear();
    void setLanguage(QString lang);
    QString convertLangToISOCode(QString lang);

signals:
    void initData();
    void scheduleData();
    void areaData();
    void languageData(QVariant);

public slots:
    void parseInitData(const QByteArray &data, HTTPEngine::EventModelType type);
    void parseSchedules(const QByteArray &data);
    void parseAreas(const QByteArray &data);
    void parseLanguages(const QByteArray &data);
    void trailerUrl(QString url, QString id);

private:
    HTTPEngine *httpEngine_;
    QMap<HTTPEngine::EventModelType, EventsModel*> models_;
    QMap<QString, QString> areas_;
    QMap<QString, QString> languages_;
    QMap<QDateTime, QPair<Event*, Show*> > coverData_;
    void parseShow(QXmlStreamReader &xml);
    void parseLanguage(QXmlStreamReader &xml);
};

#endif // PARSER_H
