#include "parser.h"

Parser::Parser(QObject *parent):
    QObject(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    httpEngine_ = new HTTPEngine();
    QObject::connect(httpEngine_, SIGNAL(eventsReady(const QByteArray&, HTTPEngine::EventModelType)),
                     this, SLOT(parseInitData(const QByteArray&, HTTPEngine::EventModelType)));
}

void Parser::addNewModel(HTTPEngine::EventModelType type, EventsModel* model)
{
    models_.insert(type, model);
}

Parser::~Parser()
{
}

void Parser::parseEvents()
{
    QList<QPair<QString, QString> > list;
    list.append(qMakePair(QString("listType"), QString("NowInTheatres")));
    addNewModel(HTTPEngine::EventModelType::InTheatres, new EventsModel());
    httpEngine_->getEvents(list, HTTPEngine::EventModelType::InTheatres);

    list.clear();
    list.append(qMakePair(QString("listType"), QString("ComingSoon")));
    addNewModel(HTTPEngine::EventModelType::ComingSoon, new EventsModel());
    httpEngine_->getEvents(list, HTTPEngine::EventModelType::ComingSoon);
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
    emit initData(type);
}







