#include "coursemodel.h"

void CourseModel::addCourse(const CourseModel::Course &course) {
    // NOTE: use correct Index !!!
    // NOTE: dataChange only applies on existing data !!!
    // NOTE: use begin/endInsert rows whenever new row is to be inserted!!!
    emit beginInsertRows(QModelIndex(), courseList.size(), courseList.size());
    courseList.append(course);
    idMap.insert(course.classId, course);
    emit endInsertRows();
    emit dataChanged(createIndex(courseList.size(), 0), createIndex(courseList.size(), colCount - 1));
}

void CourseModel::clearCourse() {
    beginResetModel();
    courseList.clear();
    idMap.clear();
    endResetModel();
    emit dataChanged(createIndex(0, 0), createIndex(courseList.size(), colCount - 1));
}

int CourseModel::rowCount(const QModelIndex &) const {
    return courseList.size();
}

int CourseModel::columnCount(const QModelIndex &) const {
    return colCount;
}

QVariant CourseModel::data(const QModelIndex &index, int role) const {
    switch (role) {
        case id:
            return courseList.at(index.row()).classId;
        case name:
            return courseList.at(index.row()).name;
        case span:
            return QString::number(courseList.at(index.row()).span);
        case season:
            return QString::number(courseList.at(index.row()).season);
        case selected:
            return courseList.at(index.row()).selected;
        case pending:
            return courseList.at(index.row()).pending;
    }
    return QVariant::Invalid;
}

QHash<int, QByteArray> CourseModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[id] = "id";
    roles[name] = "name";
    roles[span] = "span";
    roles[season] = "season";
    roles[selected] = "selected";
    roles[pending] = "pending";
    return roles;
}

bool CourseModel::getIsSelected(QString classId) {
    if (idMap.contains(classId))
        return idMap[classId].selected;
    return false;
}

bool CourseModel::getIsPending(QString classId) {
    if (idMap.contains(classId))
        return idMap[classId].pending;
    return false;
}

bool CourseFilterModel::filterAcceptsRow(int source_row, const QModelIndex &) const {
    // NOTE: directly acquire data from index, not from model
    QModelIndex idIndex = sourceModel()->index(source_row, 0) ;
    QModelIndex nameIndex = sourceModel()->index(source_row, 1);
    QModelIndex spanIndex = sourceModel()->index(source_row, 2);

    // TODO: debug this
    if (idIndex.data(CourseModel::id).toString().contains(sortingId) &&
            nameIndex.data(CourseModel::name).toString().contains(sortingName) &&
            (sortingSpan == 0 || spanIndex.data(CourseModel::span).toInt() == sortingSpan))
        return true;
    return false;
}

void CourseFilterModel::refresh() {
    invalidateFilter();

    // NOTE: should not call dataChanged here, or will cuase error
    //emit dataChanged(createIndex(0, 0), createIndex(rowCount(), 3));
}
