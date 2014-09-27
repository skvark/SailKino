#include "kinoAPI.h"

kinoAPI::kinoAPI(QObject *parent):
    QObject(parent)
{
    parser_ = new Parser();
    QObject::connect(parser_, SIGNAL(initData(HTTPEngine::EventModelType)),
                     this, SLOT(eventsReady(HTTPEngine::EventModelType)));
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
    return QVariant::fromValue((QObject*) parser_->getModel(static_cast<HTTPEngine::EventModelType>(type)));
}

QVariant kinoAPI::getEvent(QString id)
{
    return QVariant::fromValue((QObject*) parser_->getEvent(id));
}

void kinoAPI::eventsReady(HTTPEngine::EventModelType type)
{
    emit loading(false);
    if (type == HTTPEngine::EventModelType::InTheatres) {
        emit events();
    }
    else if (type == HTTPEngine::EventModelType::ComingSoon) {
        emit comingSoonEvents();
    }
}


