#include "showtimemodel.h"
#include "show.h"
#include <QDateTime>


ShowTimeModel::ShowTimeModel(QObject *parent)
     : QAbstractListModel(parent)
{

}

QHash<int, QByteArray> ShowTimeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[ShowStartRole] = "start";
    roles[ShowStartDateTimeRole] = "startdatetime";
    roles[ShowEndRole] = "end";
    roles[TheatreRole] = "theatre";
    roles[TheatreAuditoriumRole] = "auditorium";
    roles[ShowUrlRole] = "showurl";
    roles[EventUrlRole] = "eventurl";
    return roles;
}

void ShowTimeModel::addShow(Show* show)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    shows_.append(show);
    endInsertRows();
}

int ShowTimeModel::rowCount(const QModelIndex & parent) const {

    // only shows which are in the future taken into account
    int count = 0;
    foreach(Show* show, shows_) {
        if(show->getStartTime() > QDateTime::currentDateTime()) {
            ++count;
        }
    }

    return count;
}

void ShowTimeModel::clear()
{
    beginResetModel();
    foreach(Show* show, shows_) {
        delete show;
        show = NULL;
    }
    shows_.clear();
    endResetModel();
}

QVariant ShowTimeModel::data(const QModelIndex & index, int role) const {

    if (index.row() < 0 || index.row() > shows_.count())
     return QVariant();

    Show *show = shows_[index.row()];

    if (role == IdRole)
        return show->getId();
    else if(role == ShowStartRole)
        return show->getStart();
    else if(role == ShowStartDateTimeRole)
        return show->getStartTime();
    else if(role == ShowEndRole)
        return show->getEnd();
    else if(role == TheatreRole)
        return show->getTheatre();
    else if(role == TheatreAuditoriumRole)
        return show->getAuditorium();
    else if(role == ShowUrlRole)
        return show->getShowUrl();
    else if(role == EventUrlRole)
        return show->getEventUrl();
    return QVariant();

}
