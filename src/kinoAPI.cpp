#include "kinoAPI.h"

kinoAPI::kinoAPI(QObject *parent):
    QObject(parent)
{
    parser_ = new Parser();
    QObject::connect(parser_, SIGNAL(initData(HTTPEngine::EventModelType)),
                     this, SLOT(eventsReady(HTTPEngine::EventModelType)));
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

QVariant kinoAPI::getModel(int type) const
{
    HTTPEngine::EventModelType t = static_cast<HTTPEngine::EventModelType>(type);
    EventsModel* model = parser_->getModel(t);
    return QVariant::fromValue((QObject*) model);
}

QVariant kinoAPI::getEvent(QString id)
{
    return QVariant::fromValue((QObject*) parser_->getEvent(id));
}

void kinoAPI::eventsReady(HTTPEngine::EventModelType type)
{
    parser_->getSchedules();
}

void kinoAPI::schedulesReady() {
    emit loading(false);
    emit schedule();
    emit events();
    emit comingSoonEvents();
}


