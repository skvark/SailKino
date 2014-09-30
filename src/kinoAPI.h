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

public:
    kinoAPI(QObject *parent = 0);
    ~kinoAPI();

    Q_INVOKABLE void init();
    Q_INVOKABLE QVariant getModel(int type) const;
    Q_INVOKABLE QVariant getEvent(QString id);

signals:
    void events();
    void comingSoonEvents();
    void loading(bool yesno);
    void schedule();

public slots:
    void eventsReady(HTTPEngine::EventModelType type);
    void schedulesReady();

private:
    Parser* parser_;
    EventsModel* model_;

};

#endif // GETFOODDATA_H
