#ifndef EVENTSMODEL_H
#define EVENTSMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QString>
#include <QDate>
#include <QDebug>
#include <event.h>

class EventsModel : public QAbstractListModel
 {
     Q_OBJECT
 public:
     enum EventRoles {
         TitleRole,
         IDRole,
         ShortSynopsisRole,
         SmallImagePortraitRole,
         MediumImagePortraitRole,
         GenreRole
     };

     EventsModel(QObject *parent = 0);
     void addEvent(Event* event);
     int rowCount(const QModelIndex & parent = QModelIndex()) const;
     QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
     Q_INVOKABLE void clear();
     Event *getEvent(QString id);
     void clearSchedules();
     void filterShows();
     Q_INVOKABLE int count();
     Q_INVOKABLE QVariantMap get(int row);
     QList<Event *> all();

public slots:

protected:
     QHash<int, QByteArray> roleNames() const;

private:
     int type_;
     QList<Event*> events_;
 };

#endif // EVENTSMODEL_H
