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
    QObject::connect(httpEngine_, SIGNAL(languagesReady(const QByteArray&)),
                     this, SLOT(parseLanguages(const QByteArray&)));
    QObject::connect(httpEngine_, SIGNAL(youtubeReady(QString, QString)),
                     this, SLOT(trailerUrl(QString, QString)));
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
    areas_.clear();
    httpEngine_->getAreas();
}

void Parser::getLanguages()
{
    httpEngine_->getLanguages();
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

void Parser::setLocation(SettingsManager::Country country)
{
    httpEngine_->setLocation(country);
}

void Parser::setLanguage(QString lang)
{
    httpEngine_->setLanguage(lang);
}

QString Parser::convertLangToISOCode(QString lang)
{
    return languages_.key(lang);
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
    }

    xml.clear();
    emit areaData();
}

void Parser::parseLanguages(const QByteArray &data)
{
    QXmlStreamReader xml;
    xml.addData(data);
    languages_.clear();

    while(!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if(token == QXmlStreamReader::StartElement) {

            if(xml.name() == "Languages") {
                continue;
            }

            if(xml.name() == "Language") {
                parseLanguage(xml);
            }
        }
    }

    if(xml.hasError()) {
    }

    xml.clear();

    QStringList langs;
    foreach(QString value, languages_.values()) {
        langs.append(value);
    }
    emit languageData(QVariant::fromValue(langs));
}

void Parser::trailerUrl(QString url, QString id)
{
    Event* event = getEvent(id);
    if(event) {
        event->setTrailerUrl(url);
    }
}

void Parser::parseLanguage(QXmlStreamReader &xml)
{
    QString isocode;
    QString isocodetwo;
    QString name;
    QString localName;
    xml.readNext();

    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Language")) {

        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            if(xml.name() == "ISOCode") {
                isocode = parseElement(xml);
            }
            if(xml.name() == "ISOTwoLetterCode") {
                isocodetwo = parseElement(xml);
            }
            if(xml.name() == "Name") {
                name = parseElement(xml);
            }
            if(xml.name() == "LocalName") {
                localName = parseElement(xml);
            }
        }
        xml.readNext();
    }

    if (isocodetwo == "FI") {
        languages_.insert(isocodetwo, QString("%1 (%2)").arg(localName).arg(name));
    } else {
        languages_.insert(isocode, QString("%1 (%2)").arg(localName).arg(name));
    }
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
            if(xml.name() == "EventMediumImagePortrait") {
                event.insert("MediumImagePortrait", parseElement(xml));
            }
            if(xml.name() == "EventLargeImageLandscape") {
                event.insert("LargeImageLandscape", parseElement(xml));
            }
            if(xml.name() == "Location") {
                event.insert("Trailer", QString("https://m.youtube.com/watch?v=%1").arg(parseElement(xml)));
            }
            if(xml.name() == "MediaResourceFormat") {
                event.insert("TrailerType", parseElement(xml));
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
                              event.value("MediumImagePortrait"),
                              event.value("LargeImageLandscape"),
                              event.value("Trailer"),
                              event.value("TrailerType"),
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
    }

    xml.clear();

    foreach(Event* event, getModel(HTTPEngine::EventModelType::InTheatres)->all()) {
        event->filter();
    }

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
                QString url = parseElement(xml);
                if (httpEngine_->getCurrentLang() == "/fin") {
                    url = url.replace(QLatin1String("http://www"), QString("http://m"));
                }
                show.insert("ShowURL", url);
            }
            if(xml.name() == "EventURL") {
                QString url = parseElement(xml);
                if (httpEngine_->getCurrentLang() == "/fin") {
                    url = url.replace(QLatin1String("http://www"), QString("http://m"));
                }
                show.insert("EventURL", url);
            }
        }
        xml.readNext();
    }

    Event* _event = model->getEvent(show.value("EventID"));
    _event->addSchedule(show);

}





