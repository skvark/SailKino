#include "kinoAPI.h"
#include <QDBusConnection>
#include <QDBusInterface>

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

    // if country and lang are set, we can fetch areas at startup
    if (settings_->getCountryName().length() != 0
            && settings_->loadLanguage().length() != 0) {
        parser_->setLocation(settings_->country());
        parser_->setLanguage(settings_->loadLanguage());
        parser_->getAreas();
    }

    // if area is not set, raise flag
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

/*
    Inits the interface. Called via QML side when all data needs to be updated.
    Country, language and area must be have been set beforehand.
*/
void kinoAPI::init() {
    emit loading(true);
    parser_->setLocation(settings_->country());
    parser_->setLanguage(settings_->loadLanguage());
    parser_->parseEvents(getArea());
}

/*
    Gets country name from settings.
*/
QString kinoAPI::getCountryName() {
    return settings_->getCountryName();
}

/*
    Gets language name from settings.
*/
QString kinoAPI::getLanguageName() {
    return settings_->getLanguageName();
}

/*
    Saves filter setting state.
*/
void kinoAPI::setFilterState(bool state)
{
    settings_->saveFilter(state);
}

/*
    Gets filter setting state.
*/
bool kinoAPI::getFilterState()
{
    return settings_->getFilterState();
}

/*
    This is called peridiodically at QML side to filter events.
*/
void kinoAPI::reFilter() {
    parser_->getModel(HTTPEngine::EventModelType::InTheatres)->filterShows();
}

/*
    Sets an temporary event ID value, which is then used in kinoAPI::getEvent
    to get the specified event object pointer.
*/
void kinoAPI::setID(QString id)
{
    id_ = id;
}

/*
    Returns pointer to EventsModel object which holds events which are
    currently playing in theatres.
*/
EventsModel *kinoAPI::inTheatres() const
{
    EventsModel* model = parser_->getModel(HTTPEngine::EventModelType::InTheatres);
    return model;
}

/*
    Returns pointer to EventsModel object which holds coming soon events.
*/
EventsModel *kinoAPI::comingSoon() const
{
    EventsModel* model = parser_->getModel(HTTPEngine::EventModelType::ComingSoon);
    return model;
}

/*
    This slot is called when parser has finished parsing the events.
    This means that schedules can be fetched and parsed for the events.
*/
void kinoAPI::eventsReady()
{
    parser_->getSchedules(getArea(), date_);
}

/*
    Gets the event which is specified beforehand with kinoAPI::setID() method.
*/
Event* kinoAPI::getEvent() const
{
    Event* event = parser_->getEvent(id_);
    return event;
}

/*
    Gets the area ID which has been selected in settings.
*/
QString kinoAPI::getArea()
{
    return settings_->loadArea();
}

/*
    Sets a new date. After that emits loading signal to UI and calls eventsReady() to
    get the new schedules for given date.
*/
void kinoAPI::setDate(QDate date)
{
    if(date < QDate::currentDate()) {
        date_ = QDate::currentDate();
    } else {
        date_ = date;
    }
    emit dateChanged();
    emit loading(true);
    emit schedulesLoading(true);
    eventsReady();
}

/*
    Gets the currently set date.
*/
QDate kinoAPI::getDate()
{
    return date_;
}

/*
    Gets name of the area which has been selected in settings.
*/
QString kinoAPI::getAreaName()
{
    QString areaid = settings_->loadArea();
    return parser_->getAreaName(areaid);
}

/*
    Saves area ID to the settings.
*/
void kinoAPI::saveArea(QString area)
{
    settings_->saveArea(parser_->getAreaID(area));
}

/*
    Gets list of the available areas for currently selected country.
    Returns empty list, if no country and language has been set.
*/
QVariant kinoAPI::getAreas()
{
    return QVariant::fromValue(parser_->getAreasList());
}

/*
    Used for checking if user has saved and area before.
*/
bool kinoAPI::areaSelectedEarlier()
{
    return areaSelectedEarlier_;
}

/*
    Gets list of available countries (hardcoded).
*/
QVariant kinoAPI::getLocations()
{
    return QVariant::fromValue(settings_->getCountryList());
}

/*
    Saves given country to settings.
*/
void kinoAPI::saveLocation(QString loc)
{
    settings_->saveCountry(loc);
}

/*
    Resets language to empty string.
*/
void kinoAPI::resetLanguage() {
    settings_->saveLanguage("", "");
    parser_->setLanguage("");
}

/*
    Saves language to settings and calls parser's getAreas() immediately to
    decrease waiting time of the user.
*/
void kinoAPI::saveLanguage(QString lang)
{
    settings_->saveLanguage("/" + parser_->convertLangToISOCode(lang), lang);
    parser_->getAreas();
}

/*
    Clears all the models.
*/
void kinoAPI::clearModels() {
    emit clear();
    parser_->clear();
}

/*
    Signal to notify UI that everything is ready and UI can be populated.
*/
void kinoAPI::schedulesReady() {
    emit loading(false);
    emit schedulesLoading(false);
    emit ready();
}

/*
    Signal to notify UI that areas are ready.
*/
void kinoAPI::areasParsed()
{
    emit areas();
}

/*
    Gets languages when user selects country in the settings view.
*/
void kinoAPI::getLanguages()
{
    parser_->setLocation(settings_->country());
    emit languagesLoading();
    parser_->getLanguages();
}

/* Prevents screen going dark during video playback.
   true = no blanking
   false = blanks normally
*/
void kinoAPI::setBlankingMode(bool state)
{

    QDBusConnection system = QDBusConnection::connectToBus(QDBusConnection::SystemBus,
                                                           "system");

    QDBusInterface interface("com.nokia.mce",
                             "/com/nokia/mce/request",
                             "com.nokia.mce.request",
                             system);

    if (state) {
        interface.call(QLatin1String("req_display_blanking_pause"));
    } else {
        interface.call(QLatin1String("req_display_cancel_blanking_pause"));
    }

}

