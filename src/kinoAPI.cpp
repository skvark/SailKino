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

    QObject::connect(settings_, SIGNAL(countryChanged()),
                     this, SLOT(getLanguages()));

    QObject::connect(parser_, SIGNAL(languageData(QVariant)),
                     this, SIGNAL(languagesReady(QVariant)));

    if (settings_->getCountryName().length() != 0
            && settings_->loadLanguage().length() != 0) {
        parser_->setLocation(settings_->country());
        parser_->setLanguage(settings_->loadLanguage());
        parser_->getAreas();
    }

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
    parser_->setLocation(settings_->country());
    parser_->setLanguage(settings_->loadLanguage());
    parser_->parseEvents(getArea());
}

QString kinoAPI::getCountryName() {
    return settings_->getCountryName();
}

QString kinoAPI::getLanguageName() {
    return settings_->getLanguageName();
}

void kinoAPI::setFilterState(bool state)
{
    settings_->saveFilter(state);
}

bool kinoAPI::getFilterState()
{
    return settings_->getFilterState();
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
    return settings_->loadArea();
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
    QString areaid = settings_->loadArea();
    return parser_->getAreaName(areaid);
}

void kinoAPI::saveArea(QString area)
{
    settings_->saveArea(parser_->getAreaID(area));
}

QVariant kinoAPI::getAreas()
{
    return QVariant::fromValue(parser_->getAreasList());
}

bool kinoAPI::areaSelectedEarlier()
{
    return areaSelectedEarlier_;
}

QVariant kinoAPI::getLocations()
{
    return QVariant::fromValue(settings_->getCountryList());
}

void kinoAPI::saveLocation(QString loc)
{
    settings_->saveCountry(loc);
}

void kinoAPI::resetLanguage() {
    settings_->saveLanguage("", "");
    parser_->setLanguage("");
}

void kinoAPI::saveLanguage(QString lang)
{
    settings_->saveLanguage("/" + parser_->convertLangToISOCode(lang), lang);
    parser_->getAreas();
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
    qDebug() << "areasparsed";
    emit areas();
}

void kinoAPI::getLanguages()
{
    parser_->setLocation(settings_->country());
    emit languagesLoading();
    parser_->getLanguages();
}


