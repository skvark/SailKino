#ifndef SHOW_H
#define SHOW_H

#include <QString>
#include <QDateTime>
#include <QMap>

class Show
{
public:
    Show(QMap<QString, QString> init);

    QString getId();
    QString getStart();
    QString getEnd();
    QString getTheatre();
    QString getAuditorium();
    QString getShowUrl();
    QString getEventUrl();
    QDateTime getStartTime();

private:

    QString id_;
    QDateTime showStart_;
    QDateTime showEnd_;
    QString theatre_;
    QString theatreAuditorium_;
    QString showUrl_;
    QString eventUrl_;

};

#endif // SHOW_H
