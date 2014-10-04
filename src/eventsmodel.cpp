#include "eventsmodel.h"
#include "event.h"


EventsModel::EventsModel(QObject *parent)
     : QAbstractListModel(parent)
{

}

QHash<int, QByteArray> EventsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[IDRole] = "id";
    roles[ShortSynopsisRole] = "shortsynopsis";
    roles[SmallImagePortraitRole] = "smallimageportrait";
    return roles;
}

void EventsModel::addEvent(Event* event)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    events_.append(event);
    endInsertRows();
}

int EventsModel::rowCount(const QModelIndex & parent) const {
    return events_.size();
}


void EventsModel::clear()
{
    beginResetModel();
    foreach(Event* event, events_) {
        event->getModel()->clear();
        delete event;
        event = NULL;
    }
    events_.clear();
    endResetModel();
}

Event *EventsModel::getEvent(QString id)
{
    foreach(Event* event, events_) {
        if (event->getID() == id) {
            return event;
        }
    }
    return NULL;
}

QVariant EventsModel::data(const QModelIndex & index, int role) const {

    if (index.row() < 0 || index.row() > events_.count())
     return QVariant();

    Event *event = events_[index.row()];
    if (role == TitleRole)
        return event->getTitle();
    else if(role == IDRole)
        return event->getID();
    else if(role == ShortSynopsisRole)
        return event->getShortSynopsis();
    else if(role == SmallImagePortraitRole)
        return event->smallImagePortrait();

    return QVariant();
}
