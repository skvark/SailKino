#ifndef EVENT_H
#define EVENT_H

#include <QString>
#include <QObject>

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
          QString trailer);

    Q_INVOKABLE QString getTitle();
    QString getID();
    Q_INVOKABLE QString getSynopsis();
    Q_INVOKABLE QString getGenres();
    QString getShortSynopsis();
    Q_INVOKABLE QString originalTitle();
    QString smallImagePortrait();
    Q_INVOKABLE QString getLargeImageLandscape();
    Q_INVOKABLE QString getTrailer();

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
};

#endif // EVENT_H
