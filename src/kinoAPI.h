#ifndef KINOAPI_H
#define KINOAPI_H

#include <parser.h>
#include <settings.h>
#include <eventsmodel.h>
#include <QDebug>
#include <httpEngine.h>
#include <settings.h>
#include <QString>
#include <QStringList>

class kinoAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(EventsModel* inTheatres READ inTheatres CONSTANT)
    Q_PROPERTY(EventsModel* comingSoon READ comingSoon CONSTANT)
    Q_PROPERTY(Event* getEvent READ getEvent CONSTANT)

public:
    kinoAPI(QObject *parent = 0);
    ~kinoAPI();

    Q_INVOKABLE void init();

    Event* getEvent() const;
    Q_INVOKABLE void setID(QString id);

    EventsModel *inTheatres() const;
    EventsModel *comingSoon() const;

    Q_INVOKABLE void setDate(QDate date);
    Q_INVOKABLE QDate getDate();

    QString getArea();
    Q_INVOKABLE QString getAreaName();
    Q_INVOKABLE void saveArea(QString area);
    Q_INVOKABLE QVariant getAreas();
    Q_INVOKABLE bool areaSelectedEarlier();

    Q_INVOKABLE QVariant getLocations();
    Q_INVOKABLE void saveLocation(QString loc);
    Q_INVOKABLE void saveLanguage(QString lang);

    Q_INVOKABLE void clearModels();
    Q_INVOKABLE void resetLanguage();

    Q_INVOKABLE QString getCountryName();
    Q_INVOKABLE QString getLanguageName();

    Q_INVOKABLE void setFilterState(bool state);
    Q_INVOKABLE bool getFilterState();

    Q_INVOKABLE void reFilter();

signals:
    void ready();
    void loading(bool yesno);
    void schedulesLoading(bool yesno);
    void areas();
    void clear();
    void placeholder();
    void languagesLoading();
    void languagesReady(QVariant langs);
    void dateChanged();

public slots:
    void eventsReady();
    void schedulesReady();
    void areasParsed();
    void getLanguages();

private:
    Parser* parser_;
    EventsModel* model_;
    QString id_;
    SettingsManager* settings_;
    QDate date_;
    bool areaSelectedEarlier_;
};

#endif // KINOAPI_H
