#ifndef SHOWTIMEMODEL_H
#define SHOWTIMEMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QString>
#include <QDate>
#include <QDebug>
#include "show.h"

class ShowTimeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ShowRoles {
        IdRole,
        ShowStartRole,
        ShowStartDateTimeRole,
        ShowEndRole,
        TheatreRole,
        TheatreAuditoriumRole,
        ShowUrlRole,
        EventUrlRole
    };

    ShowTimeModel(QObject *parent = 0);
    void addShow(Show* show);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void clear();

public slots:

protected:
     QHash<int, QByteArray> roleNames() const;

private:
     QList<Show*> shows_;
};

#endif // SHOWTIMEMODEL_H
