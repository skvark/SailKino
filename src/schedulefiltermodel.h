#ifndef SCHEDULEFILTERMODEL_H
#define SCHEDULEFILTERMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QDateTime>

class ScheduleFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ScheduleFilterModel(QObject *parent = 0);
    void setMinDateTime(QDateTime datetime);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

signals:

public slots:

private:

    QDateTime minDateTime_;

};

#endif // SCHEDULEFILTERMODEL_H
