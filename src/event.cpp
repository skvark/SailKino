#include "event.h"

Event::Event(QObject *parent)
{

}

Event::Event(QString id,
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
             QString lengthInMinutes):
    id_(id),
    title_(title),
    synopsis_(synopsis),
    originalTitle_(originalTitle),
    shortSynopsis_(shortSynopsis),
    genres_(genres),
    smallImagePortrait_(smallImagePortrait),
    largeImageLandscape_(largeImageLandscape),
    trailer_(trailer),
    rating_(rating),
    productionYear_(productionYear),
    lengthInMinutes_(lengthInMinutes)
{
    schedule_ = new ShowTimeModel();
}

QString Event::getTitle()
{
    return title_;
}

QString Event::getID()
{
    return id_;
}

QString Event::getSynopsis()
{
    return synopsis_;
}

QString Event::getGenres()
{
    return genres_;
}

QString Event::getShortSynopsis()
{
    return shortSynopsis_;
}

QString Event::originalTitle()
{
    return originalTitle_;
}

QString Event::smallImagePortrait()
{
    return smallImagePortrait_;
}

QString Event::getLargeImageLandscape()
{
    return largeImageLandscape_;
}

QString Event::getTrailer()
{
    return trailer_;
}

QString Event::getRating()
{
    return rating_;
}

QString Event::getProductionYear()
{
    return productionYear_;
}

QString Event::getLengthInMinutes()
{
    return lengthInMinutes_;
}

void Event::addSchedule(QMap<QString, QString> data)
{
    schedule_->addShow(new Show(data));
}

ShowTimeModel *Event::getModel() const
{
    return schedule_;
}
