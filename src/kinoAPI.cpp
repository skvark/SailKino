#include "kinoAPI.h"

kinoAPI::kinoAPI(QObject *parent):
    QObject(parent)
{
    parser_ = new Parser();
    settings_ = new SettingsManager();
    date_ = QDate::currentDate();
    QObject::connect(parser_, SIGNAL(initData()),
                     this, SLOT(eventsReady()));
    QObject::connect(parser_, SIGNAL(scheduleData()),
                     this, SLOT(schedulesReady()));
    QObject::connect(parser_, SIGNAL(areaData()),
                     this, SLOT(areasParsed()));
    parser_->getAreas();
    if(getArea().isEmpty()) {
        areaSelectedEarlier_ = false;
    } else {
        areaSelectedEarlier_ = true;
    }
}

kinoAPI::~kinoAPI()
{
    delete parser_;
    parser_ = 0;
}

void kinoAPI::init() {
    emit loading(true);
    parser_->parseEvents(getArea());
}

void kinoAPI::setID(QString id)
{
    id_ = id;
}

EventsModel *kinoAPI::inTheatres() const
{
    EventsModel* model = parser_->getModel(HTTPEngine::EventModelType::InTheatres);
    return model;
}

EventsModel *kinoAPI::comingSoon() const
{
    EventsModel* model = parser_->getModel(HTTPEngine::EventModelType::ComingSoon);
    return model;
}

void kinoAPI::eventsReady()
{
    parser_->getSchedules(getArea(), date_);
}

Event* kinoAPI::getEvent() const
{
    Event* event = parser_->getEvent(id_);
    return event;
}

QString kinoAPI::getArea()
{
    return settings_->loadSettings();
}

void kinoAPI::setDate(QDate date)
{
    date_ = date;
    emit schedulesLoading(true);
    eventsReady();
}

QDate kinoAPI::getDate()
{
    return date_;
}

QString kinoAPI::getAreaName()
{
    QString areaid = settings_->loadSettings();
    return parser_->getAreaName(areaid);
}

void kinoAPI::saveArea(QString area)
{
    settings_->saveSettings(parser_->getAreaID(area));
}

QVariant kinoAPI::getAreas()
{
    return QVariant::fromValue(parser_->getAreasList());
}

bool kinoAPI::areaSelectedEarlier()
{
    return areaSelectedEarlier_;
}

void kinoAPI::clearModels() {
    parser_->clear();
}

void kinoAPI::schedulesReady() {
    emit loading(false);
    emit schedulesLoading(false);
    emit ready();
}

void kinoAPI::areasParsed()
{
    emit areas();
}


