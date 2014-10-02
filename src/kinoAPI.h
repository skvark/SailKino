#ifndef GETFOODDATA_H
#define GETFOODDATA_H

#include <parser.h>
#include <settings.h>
#include <eventsmodel.h>
#include <QDebug>
#include <httpEngine.h>


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
    Q_INVOKABLE void setID(QString id);
    EventsModel *inTheatres() const;
    EventsModel *comingSoon() const;
    Event* getEvent() const;

signals:
    void ready();
    void loading(bool yesno);

public slots:
    void eventsReady();
    void schedulesReady();

private:
    Parser* parser_;
    EventsModel* model_;
    QString id_;

};

#endif // GETFOODDATA_H
