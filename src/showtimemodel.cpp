#include "showtimemodel.h"
#include "show.h"


ShowTimeModel::ShowTimeModel(QObject *parent)
     : QAbstractListModel(parent)
{

}

QHash<int, QByteArray> ShowTimeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[ShowStartRole] = "start";
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
    return shows_.size();
}

void ShowTimeModel::clear()
{
    beginResetModel();
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
