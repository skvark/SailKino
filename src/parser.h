#ifndef FOODPARSER_H
#define FOODPARSER_H

#include <QString>
#include <QList>
#include <httpEngine.h>
#include <kitchen.h>
#include <helperFunctions.h>
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
    void parseEvents();
    EventsModel *getModel(HTTPEngine::EventModelType);
    void parseEvent(QXmlStreamReader &xml, HTTPEngine::EventModelType);
    QString parseElement(QXmlStreamReader &xml) const;
    Event *getEvent(QString id);

signals:
    void initData(HTTPEngine::EventModelType type);

public slots:
    void parseInitData(const QByteArray &data, HTTPEngine::EventModelType type);

private:
    HTTPEngine *httpEngine_;
    QMap<HTTPEngine::EventModelType, EventsModel*> models_;
};

#endif // FOODPARSER_H
