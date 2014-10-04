#ifndef FOODPARSER_H
#define FOODPARSER_H

#include <QString>
#include <QList>
#include <httpEngine.h>
#include <QVector>
#include <QTextCodec>
#include <QNetworkReply>
#include <QDebug>
#include <QDate>
#include <event.h>
#include <eventsmodel.h>
#include <QXmlStreamReader>

class Parser : public QObject
{
    Q_OBJECT

public:
    Parser(QObject *parent = 0);
    ~Parser();

    void addNewModel(HTTPEngine::EventModelType, EventsModel *model);
    void parseEvents(QString area);
    void getSchedules(QString area);
    EventsModel *getModel(HTTPEngine::EventModelType);
    void parseEvent(QXmlStreamReader &xml, HTTPEngine::EventModelType);
    void parseArea(QXmlStreamReader &xml);
    QString parseElement(QXmlStreamReader &xml) const;
    Event *getEvent(QString id);
    void parseSoonEvents();
    void getAreas();
    QString getAreaName(QString id);
    QString getAreaID(QString area);
    QStringList getAreasList();
    void clear();

signals:
    void initData();
    void scheduleData();
    void areaData();

public slots:
    void parseInitData(const QByteArray &data, HTTPEngine::EventModelType type);
    void parseSchedules(const QByteArray &data);
    void parseAreas(const QByteArray &data);

private:
    HTTPEngine *httpEngine_;
    QMap<HTTPEngine::EventModelType, EventsModel*> models_;
    QMap<QString, QString> areas_;
    void parseShow(QXmlStreamReader &xml);
};

#endif // FOODPARSER_H
