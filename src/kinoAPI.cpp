#include "kinoAPI.h"
#include <QQmlEngine>

kinoAPI::kinoAPI(QObject *parent):
    QObject(parent)
{
    parser_ = new Parser();
    QObject::connect(parser_, SIGNAL(initData()),
                     this, SLOT(eventsReady()));
    QObject::connect(parser_, SIGNAL(scheduleData()),
                     this, SLOT(schedulesReady()));
    init();
}

kinoAPI::~kinoAPI()
{
    delete parser_;
    parser_ = 0;
}

void kinoAPI::init() {
    emit loading(true);
    parser_->parseEvents();
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
    parser_->getSchedules();
}

Event* kinoAPI::getEvent() const
{
    Event* event = parser_->getEvent(id_);
    return event;
}

void kinoAPI::schedulesReady() {
    emit loading(false);
    emit ready();
}


