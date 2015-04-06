#include "schedulefiltermodel.h"
#include "showtimemodel.h"

ScheduleFilterModel::ScheduleFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void ScheduleFilterModel::setMinDateTime(QDateTime datetime)
{
    minDateTime_ = datetime;
    invalidateFilter();
}

bool ScheduleFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QDateTime date = sourceModel()->data(index, ShowTimeModel::ShowRoles::ShowStartDateTimeRole).toDateTime();
    return date > minDateTime_;
}
