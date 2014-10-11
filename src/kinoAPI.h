#ifndef GETFOODDATA_H
#define GETFOODDATA_H

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

    Q_INVOKABLE void clearModels();

signals:
    void ready();
    void loading(bool yesno);
    void schedulesLoading(bool yesno);
    void areas();
    void placeholder();

public slots:
    void eventsReady();
    void schedulesReady();
    void areasParsed();

private:
    Parser* parser_;
    EventsModel* model_;
    QString id_;
    SettingsManager* settings_;
    QDate date_;
    bool areaSelectedEarlier_;
};

#endif // GETFOODDATA_H
