#include "event.h"
#include <QDateTime>

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
             QString mediumImagePortrait,
             QString largeImageLandscape,
             QString trailer,
             QString trailerType,
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
    mediumImagePortrait_(mediumImagePortrait),
    largeImageLandscape_(largeImageLandscape),
    trailer_(trailer),
    trailerType_(trailerType),
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

QString Event::mediumImagePortrait()
{
    return mediumImagePortrait_;
}

QString Event::getLargeImageLandscape()
{
    return largeImageLandscape_;
}

void Event::verifyTrailerUrl() {
    if (trailerType_ != "YouTubeVideo") {
        return;
    } else {
        emit parseYoutube(trailer_, id_);
    }
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

bool Event::filteredHasShows()
{
    if(filteredSchedule_->rowCount() > 0) {
        return true;
    } else {
        return false;
    }
}

bool Event::hasShows()
{ 
    if(schedule_->rowCount() > 0) {
        return true;
    } else {
        return false;
    }
}

Show* Event::addSchedule(QMap<QString, QString> data)
{
    Show* show = new Show(data);
    schedule_->addShow(show);
    return show;
}

ScheduleFilterModel *Event::getFilteredModel() const
{
    return filteredSchedule_;
}

void Event::filter() {
    auto scheduleProxyModel = new ScheduleFilterModel();
    scheduleProxyModel->setSourceModel(schedule_);
    scheduleProxyModel->setMinDateTime(QDateTime::currentDateTime());
    scheduleProxyModel->setDynamicSortFilter(true);
    filteredSchedule_ = scheduleProxyModel;
}

void Event::reFilter() {
    filteredSchedule_->invalidate();
}

ShowTimeModel *Event::getModel() const
{
    return schedule_;
}

void Event::setTrailerUrl(QString url)
{
    trailer_ = url;
}
