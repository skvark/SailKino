#include "show.h"

Show::Show(QMap<QString, QString> init)
{
    id_ = init.value("ID");
    theatre_ = init.value("Theatre");
    theatreAuditorium_ = init.value("TheatreAuditorium");
    showUrl_ = init.value("ShowURL");
    eventUrl_ = init.value("EventURL");
    // i.e. 2014-09-29T10:30:00
    showStart_ = QDateTime::fromString(init.value("dttmShowStart"), Qt::ISODate);
    showEnd_ = QDateTime::fromString(init.value("dttmShowEnd"), Qt::ISODate);
}

QString Show::getId()
{
    return id_;
}

QString Show::getStart()
{
    return showStart_.toString("dd.MM.yy hh:mm");
}

QString Show::getEnd()
{
    return showEnd_.toString("hh:mm");
}

QString Show::getTheatre()
{
    return theatre_;
}

QString Show::getAuditorium()
{
    return theatreAuditorium_;
}

QString Show::getShowUrl()
{
    return showUrl_.replace(QLatin1String("http://www"), QString("http://m"));
}

QString Show::getEventUrl()
{
    return eventUrl_.replace(QLatin1String("http://www"), QString("http://m"));
}
