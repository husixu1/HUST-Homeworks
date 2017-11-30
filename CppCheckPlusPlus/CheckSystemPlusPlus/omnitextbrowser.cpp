/// @file omnitextbrowser.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 11/2017/2017
/// @copyright GNU Public License v3
#include "omnitextbrowser.h"

OmniTextBrowser::OmniTextBrowser(QWidget *parent) :
    QTextBrowser(parent) {
    setAcceptDrops(true);
}

OmniTextBrowser::~OmniTextBrowser() {

}

void OmniTextBrowser::dragMoveEvent(QDragMoveEvent *event) {
    event->accept();
}

void OmniTextBrowser::dragEnterEvent(QDragEnterEvent *event) {
    event->accept();
    emit fileDraggedIn(event->mimeData()->urls());
}

void OmniTextBrowser::dropEvent(QDropEvent *event) {
    event->accept();
    //> get the filename and emit event
    emit fileDropped(event->mimeData()->urls());
}
