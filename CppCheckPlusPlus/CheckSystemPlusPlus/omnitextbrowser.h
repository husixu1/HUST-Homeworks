/// @file omnitextbrowser.h
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#ifndef OMNITEXTBROWSER_H
#define OMNITEXTBROWSER_H

#include <QObject>
#include <QTextBrowser>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QtDebug>
#include <QList>
#include <QUrl>

/// @brief a text browser that implements the drag-drop events
class OmniTextBrowser : public QTextBrowser {
    Q_OBJECT

public:
    explicit OmniTextBrowser(QWidget *parent = 0);
    ~OmniTextBrowser();

    virtual void dragMoveEvent(QDragMoveEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;

signals:
    void fileDraggedIn(QList<QUrl> filePaths);
    void fileDropped(QList<QUrl> filePaths);
};

#endif // OMNITEXTBROWSER_H
