#ifndef EVENT_H
#define EVENT_H

#include <QString>
#include <QObject>
#include <QMap>
#include <QList>
#include <show.h>
#include <showtimemodel.h>

class Event : public QObject
{
    Q_OBJECT

public:
    Event(QObject *parent = 0);
    Event(QString id,
          QString title,
          QString synopsis,
          QString originalTitle,
          QString shortSynopsis,
          QString genres,
          QString smallImagePortrait,
          QString largeImageLandscape,
          QString trailer,
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
    Q_INVOKABLE QString getLargeImageLandscape();
    Q_INVOKABLE QString getTrailer();
    Q_INVOKABLE QString getRating();
    Q_INVOKABLE QString getProductionYear();
    Q_INVOKABLE QString getLengthInMinutes();
    void addSchedule(QMap<QString, QString> data);
    Q_INVOKABLE QVariant getModel();

private:
    QString id_;
    QString title_;
    QString synopsis_;
    QString originalTitle_;
    QString shortSynopsis_;
    QString genres_;
    QString smallImagePortrait_;
    QString largeImageLandscape_;
    QString trailer_;
    QString rating_;
    QString productionYear_;
    QString lengthInMinutes_;
    ShowTimeModel* schedule_;
};

#endif // EVENT_H
