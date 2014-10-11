#include "parser.h"

Parser::Parser(QObject *parent):
    QObject(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    httpEngine_ = new HTTPEngine();
    QObject::connect(httpEngine_, SIGNAL(eventsReady(const QByteArray&, HTTPEngine::EventModelType)),
                     this, SLOT(parseInitData(const QByteArray&, HTTPEngine::EventModelType)));
    QObject::connect(httpEngine_, SIGNAL(scheludesReady(const QByteArray&)),
                     this, SLOT(parseSchedules(const QByteArray&)));
    QObject::connect(httpEngine_, SIGNAL(areasReady(const QByteArray&)),
                     this, SLOT(parseAreas(const QByteArray&)));
}

void Parser::addNewModel(HTTPEngine::EventModelType type, EventsModel* model)
{
    models_.insert(type, model);
}

Parser::~Parser()
{
}

void Parser::getAreas()
{
    httpEngine_->getAreas();
}

QStringList Parser::getAreasList()
{
    QStringList areas;
    foreach(QString area, areas_) {
        if(area != "Valitse alue/teatteri")
            areas.append(area);
    }
    return areas;
}

void Parser::clear()
{
    foreach(EventsModel* model, models_) {
        model->clear();
    }
}

QString Parser::getAreaName(QString id)
{
    return areas_.value(id);
}

QString Parser::getAreaID(QString area)
{
    return areas_.key(area);
}

void Parser::parseEvents(QString area)
{
    QList<QPair<QString, QString> > list;
    list.append(qMakePair(QString("listType"), QString("NowInTheatres")));
    list.append(qMakePair(QString("area"), area));
    addNewModel(HTTPEngine::EventModelType::InTheatres, new EventsModel());
    httpEngine_->getEvents(list, HTTPEngine::EventModelType::InTheatres);
}

// area has no effect on this
void Parser::parseSoonEvents() {
    QList<QPair<QString, QString> > list;
    list.append(qMakePair(QString("listType"), QString("ComingSoon")));
    addNewModel(HTTPEngine::EventModelType::ComingSoon, new EventsModel());
    httpEngine_->getEvents(list, HTTPEngine::EventModelType::ComingSoon);
}

void Parser::parseAreas(const QByteArray &data)
{
    QXmlStreamReader xml;
    xml.addData(data);

    while(!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if(token == QXmlStreamReader::StartElement) {

            if(xml.name() == "TheatreAreas") {
                continue;
            }

            if(xml.name() == "TheatreArea") {
                parseArea(xml);
            }
        }
    }

    if(xml.hasError()) {
        qDebug() << "asdasdasd";
    }
    xml.clear();
    emit areaData();
}

void Parser::getSchedules(QString area, QDate date) {

    foreach(EventsModel* model, models_) {
        model->clearSchedules();
    }

    QList<QPair<QString, QString> > list;
    list.append(qMakePair(QString("area"), area));
    list.append(qMakePair(QString("dt"), date.toString("dd.MM.yyyy")));
    httpEngine_->getSchedule(list);
}

EventsModel *Parser::getModel(HTTPEngine::EventModelType type)
{
    return models_.value(type);
}

void Parser::parseEvent(QXmlStreamReader& xml, HTTPEngine::EventModelType type) {

    QMap<QString, QString> event;
    bool titleAdded = false;

    xml.readNext();

    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Event")) {

        if(xml.tokenType() == QXmlStreamReader::StartElement) {

            if(xml.name() == "ID") {
                event.insert("ID", parseElement(xml));
            }
            if(xml.name() == "Title" && !titleAdded) {
                event.insert("Title", parseElement(xml));
                titleAdded = true;
            }
            if(xml.name() == "ShortSynopsis") {
                event.insert("ShortSynopsis", parseElement(xml));
            }
            if(xml.name() == "OriginalTitle") {
                event.insert("OriginalTitle", parseElement(xml));
            }
            if(xml.name() == "Genres") {
                event.insert("Genres", parseElement(xml));
            }
            if(xml.name() == "Synopsis") {
                event.insert("Synopsis", parseElement(xml));
            }
            if(xml.name() == "EventSmallImagePortrait") {
                event.insert("SmallImagePortrait", parseElement(xml));
            }
            if(xml.name() == "EventLargeImageLandscape") {
                event.insert("LargeImageLandscape", parseElement(xml));
            }
            if(xml.name() == "Location") {
                event.insert("Trailer", parseElement(xml));
            }
            if(xml.name() == "Rating") {
                event.insert("Rating", parseElement(xml));
            }
            if(xml.name() == "ProductionYear") {
                event.insert("ProductionYear", parseElement(xml));
            }
            if(xml.name() == "LengthInMinutes") {
                event.insert("LengthInMinutes", parseElement(xml));
            }
        }
        xml.readNext();
    }

    Event* _event = new Event(event.value("ID"),
                              event.value("Title"),
                              event.value("Synopsis"),
                              event.value("OriginalTitle"),
                              event.value("ShortSynopsis"),
                              event.value("Genres"),
                              event.value("SmallImagePortrait"),
                              event.value("LargeImageLandscape"),
                              event.value("Trailer"),
                              event.value("Rating"),
                              event.value("ProductionYear"),
                              event.value("LengthInMinutes"));

    models_.value(type)->addEvent(_event);
}

void Parser::parseArea(QXmlStreamReader &xml)
{
    QString id;
    QString name;
    xml.readNext();

    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "TheatreArea")) {

        if(xml.tokenType() == QXmlStreamReader::StartElement) {

            if(xml.name() == "ID") {
                id = parseElement(xml);
            }
            if(xml.name() == "Name") {
                name = parseElement(xml);
            }
        }
        xml.readNext();
    }

    areas_.insert(id, name);
}

QString Parser::parseElement(QXmlStreamReader& xml) const {

    if(xml.tokenType() != QXmlStreamReader::StartElement) {
        return QString("");
    }

    xml.readNext();

    if(xml.tokenType() != QXmlStreamReader::Characters) {
        return QString("");
    }

    return xml.text().toString();
}

Event *Parser::getEvent(QString id)
{
    Event* event = models_.value(HTTPEngine::EventModelType::InTheatres)->getEvent(id);
    if (event) {
        return event;
    } else {
        return models_.value(HTTPEngine::EventModelType::ComingSoon)->getEvent(id);
    }
}

void Parser::parseInitData(const QByteArray &data, HTTPEngine::EventModelType type)
{
    QXmlStreamReader xml;
    xml.addData(data);

    while(!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if(token == QXmlStreamReader::StartElement) {

            if(xml.name() == "Events") {
                continue;
            }

            if(xml.name() == "Event") {
                parseEvent(xml, type);
            }
        }
    }

    if(xml.hasError()) {
        qDebug() << "asdasdasd";
    }
    xml.clear();
    if(type == HTTPEngine::EventModelType::ComingSoon) {
        emit initData();
    } else {
        parseSoonEvents();
    }
}

void Parser::parseSchedules(const QByteArray &data)
{
    QXmlStreamReader xml;
    xml.addData(data);

    while(!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if(token == QXmlStreamReader::StartElement) {

            if(xml.name() == "Schedule") {
                continue;
            }
            if(xml.name() == "PubDate") {
                continue;
            }
            if(xml.name() == "Shows") {
                continue;
            }
            if(xml.name() == "Show") {
                parseShow(xml);
            }
        }
    }

    if(xml.hasError()) {
        qDebug() << "kissat koiria";
    }
    xml.clear();
    emit scheduleData();
}

void Parser::parseShow(QXmlStreamReader& xml) {

    QMap<QString, QString> show;
    xml.readNext();
    EventsModel* model = getModel(HTTPEngine::EventModelType::InTheatres);

    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Show")) {

        if(xml.tokenType() == QXmlStreamReader::StartElement) {

            if(xml.name() == "ID") {
                show.insert("ID", parseElement(xml));
            }
            if(xml.name() == "dttmShowStart") {
                show.insert("dttmShowStart", parseElement(xml));
            }
            if(xml.name() == "dttmShowEnd") {
                show.insert("dttmShowEnd", parseElement(xml));
            }
            if(xml.name() == "EventID") {
                show.insert("EventID", parseElement(xml));
            }
            if(xml.name() == "Theatre") {
                show.insert("Theatre", parseElement(xml));
            }
            if(xml.name() == "TheatreAuditorium") {
                show.insert("TheatreAuditorium", parseElement(xml));
            }
            if(xml.name() == "ShowURL") {
                show.insert("ShowURL", parseElement(xml));
            }
            if(xml.name() == "EventURL") {
                show.insert("EventURL", parseElement(xml));
            }
        }
        xml.readNext();
    }
    model->getEvent(show.value("EventID"))->addSchedule(show);
}





