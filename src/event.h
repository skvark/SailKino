#ifndef EVENT_H
#define EVENT_H

#include <QString>
#include <QObject>
#include <QMap>
#include <QList>
#include <show.h>
#include <showtimemodel.h>
#include "schedulefiltermodel.h"

class Event : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ScheduleFilterModel* getFilteredModel READ getFilteredModel CONSTANT)
    Q_PROPERTY(ShowTimeModel* getModel READ getModel CONSTANT)

public:
    Event(QObject *parent = 0);
    Event(QString id,
          QString title,
          QString synopsis,
          QString originalTitle,
          QString shortSynopsis,
          QString genres,
          QString smallImagePortrait,
          QString mediumImagePortrait,
          QString largeImageLandscape,
          QString trailer,
          QString trailerType,
          QString rating,
          QString productionYear,
          QString lengthInMinutes);

    Q_INVOKABLE QString getTitle();
    QString getID();
    Q_INVOKABLE QString getSynopsis();
    Q_INVOKABLE QString getGenres();
    QString getShortSynopsis();
    Q_INVOKABLE QString originalTitle();
    QString smallImagePortrait();
    Q_INVOKABLE QString mediumImagePortrait();
    Q_INVOKABLE QString getLargeImageLandscape();
    Q_INVOKABLE QString getTrailer();
    Q_INVOKABLE QString getRating();
    Q_INVOKABLE QString getProductionYear();
    Q_INVOKABLE QString getLengthInMinutes();
    Q_INVOKABLE bool hasShows();
    Show *addSchedule(QMap<QString, QString> data);
    ShowTimeModel *getModel() const;

    void verifyTrailerUrl();
    void setTrailerUrl(QString url);

    ScheduleFilterModel *getFilteredModel() const;
    void filter();
    Q_INVOKABLE bool filteredHasShows();
    Q_INVOKABLE void reFilter();

signals:
    void parseYoutube(QString video_id, QString id);

private:
    QString id_;
    QString title_;
    QString synopsis_;
    QString originalTitle_;
    QString shortSynopsis_;
    QString genres_;
    QString smallImagePortrait_;
    QString mediumImagePortrait_;
    QString largeImageLandscape_;
    QString trailer_;
    QString trailerType_;
    QString rating_;
    QString productionYear_;
    QString lengthInMinutes_;
    ShowTimeModel* schedule_;
    ScheduleFilterModel* filteredSchedule_;
};

#endif // EVENT_H
