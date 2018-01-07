/// @file mainwindow.h
/// @author 胡思勖<Sixu Hu, husixu1@hotmail.com>
/// @copyright LGPL v3
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QMap>
#include <QGraphicsScene>
#include <QTreeWidgetItem>

#include "raftnode.h"
#include "network.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    struct Node {
        qint32 id;
        bool isUp;
        RaftNode *realNode;
    };

    QMap<qint32, Node> allNodes;

    // initialize nodes, contaray to clearAll
    QMap<qint32, RaftNode *> addNodes(qint32 nodeNum);

    Network *network;

    QTreeWidgetItem *lastItem;
    // go back to start point
    void clearAll();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void startSim();
    void stopSim();
    void raftEvenet(QJsonObject event);
    void nodeNumChanged(int);
    void speedChanged(int speed);
};

#endif // MAINWINDOW_H
