#ifndef COURSEMODEL_H
#define COURSEMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

class CourseModel : public QAbstractTableModel {
    Q_OBJECT
private:
    static const int colCount = 5;
public:
    enum Roles {
        id = Qt::UserRole,
        name,
        span,
        season,
        selected, ///< @brief is the course registered to teacher/student
        pending,  ///< @brief is the course pending to be approved/declied
    };
    struct Course {
        QString classId;
        QString name;
        int span;
        int season;
        bool selected;
        bool pending;
        Course() : selected(false) {}
        Course(QString _classId, QString _name, int _span, int _season,
                bool _selected = false, bool _pending = false) :
            classId(_classId), name(_name), span(_span), season(_season),
            selected(_selected), pending(_pending) { }
    };
private:
    QList<Course> courseList;
    QMap<QString, Course> idMap;        ///< @brief find course by id

public:
    void addCourse(const Course &course);
    void clearCourse();

    // re-implementations
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    bool getIsSelected(QString classId);
    bool getIsPending(QString classId);
};

class CourseFilterModel : public QSortFilterProxyModel {
    Q_OBJECT
private:
    QString sortingId;
    Q_PROPERTY(QString sortingId MEMBER sortingId)
    QString sortingName;
    Q_PROPERTY(QString sortingName MEMBER sortingName)
    int sortingSpan;
    Q_PROPERTY(int sortingSpan MEMBER sortingSpan)
public:
    CourseFilterModel() :
        sortingId(QString()), sortingName(QString()), sortingSpan(0) { }
protected:
    virtual bool filterAcceptsRow(
            int source_row, const QModelIndex &source_parent) const override;

public slots:
    /// @brief refresh the filtered model
    void refresh();
};


#endif // COURSEMODEL_H
