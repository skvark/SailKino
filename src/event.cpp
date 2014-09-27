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
             QString trailer):
    id_(id),
    title_(title),
    synopsis_(synopsis),
    originalTitle_(originalTitle),
    shortSynopsis_(shortSynopsis),
    genres_(genres),
    smallImagePortrait_(smallImagePortrait),
    largeImageLandscape_(largeImageLandscape),
    trailer_(trailer)
{}

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
