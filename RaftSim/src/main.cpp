/// @file main.cpp
/// @author 胡思勖<Sixu Hu, husixu1@hotmail.com>
/// @copyright LGPL v3
#include "mainwindow.h"
#include "raftnode.h"
#include "network.h"

#include <QApplication>
#include <QTextStream>
#include <QList>
#include <QMap>
#include <QObject>
#include <QJsonObject>
#include <QDebug>

static QTextStream out(stdout);
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    srand(time(0));

//    // initialize N nodes
//    QList<RaftNode *> nodes;
//    QList<qint32> ids;
//    QMap<qint32, RaftNode *> nodesIdMap;
//    for (quint32 i = 0; i < raftNodeNum; ++i) {
//        nodes.push_back(new RaftNode());
//        ids.push_back(i);
//    }
//    for (quint32 i = 0; i < raftNodeNum; ++i)
//        nodesIdMap.insert(ids[i], nodes[i]);
//
//    // initialize network and register all nodes to it
//    Network network;
//    network.run(nodesIdMap);
//
//    // start all nodes
//    using R = RaftNode;
//    for (quint32 i = 0; i < raftNodeNum; ++i) {
//        QObject::connect(nodes[i], &R::info, [ = ](QJsonObject obj) {
//            qInfo().noquote().nospace() << "id: " << obj[QString(R::keyId)].toInt() << " "
//                    << "ev: " << obj[QString(R::keyEvent)].toInt();
//            if (obj.contains(QString(R::keyContent)))
//                qInfo().noquote() << " " << obj[QString(R::keyContent)].toString();
//        });
//        nodes[i]->run(ids, i, speed);
//    }
    return a.exec();
}
