/// @file mainwindow.cpp
/// @author 胡思勖<Sixu Hu, husixu1@hotmail.com>
/// @copyright LGPL v3
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "raftnode.h"
#include "network.h"

#include <QGraphicsScene>
#include <QtMath>
#include <QDebug>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QGridLayout>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    network = nullptr;
    lastItem = nullptr;

    connect(ui->buttonStartSim, SIGNAL(clicked()), this, SLOT(startSim()));
    connect(ui->buttonReset, SIGNAL(clicked()), this, SLOT(stopSim()));
    connect(ui->sliderNodeNum, SIGNAL(sliderMoved(int)), this, SLOT(nodeNumChanged(int)));
    connect(ui->sliderSpeed, SIGNAL(sliderMoved(int)), this, SLOT(speedChanged(int)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::startSim() {
    ui->sliderNodeNum->setEnabled(false);
    ui->buttonStartSim->setEnabled(false);
    ui->buttonReset->setEnabled(true);
    // TODO:
    static qint32 speed = 300;
    qint32 nodeNum = ui->sliderNodeNum->value();

    // construct ui
    ui->mainTreeWidget->clear();
    ui->mainTreeWidget->setColumnCount(nodeNum);
    for (int i = 0; i < nodeNum; ++i)
        ui->mainTreeWidget->setColumnWidth(i, 25);
    // initailize
    raftEvenet(QJsonObject());

    // initialize N nodes
    QList<RaftNode *> nodes;
    QList<qint32> ids;
    QMap<qint32, RaftNode *> nodesIdMap;

    for (qint32 i = 0; i < nodeNum; ++i) {
        nodes.push_back(new RaftNode());
        ids.push_back(i);
    }

    // for network
    for (qint32 i = 0; i < nodeNum; ++i)
        nodesIdMap.insert(ids[i], nodes[i]);

    // insert to map
    for (int i = 0; i < nodeNum; ++i) {
        // make Node and insert to map
        Node tempNode;
        tempNode.id = ids[i];
        tempNode.isUp = true;
        tempNode.realNode = nodes[i];
        this->allNodes.insert(ids[i], tempNode);
    }

    // initialize network and register all nodes to it
    delete network;
    network = new Network;
    network->run(nodesIdMap);

    network->setRandomizePropagationTime(ui->buttonRandomize->checkState() == Qt::Checked);
    connect(ui->buttonRandomize, SIGNAL(toggled(bool)), network, SLOT(setRandomizePropagationTime(bool)));
    speedChanged(ui->sliderSpeed->value());

    // treewidget ui
    QStringList labels;
    for (qint32 i = 0; i < nodeNum; ++i) {
        labels.push_back(QString::number(i));
    }
    ui->mainTreeWidget->setHeaderLabels(labels);

    // crashbutton ui
    static const qint32 rowButtonNum = 5;

    // remove all widgets
    QLayoutItem *item;
    while ((item = ui->crashButtonsLayout->takeAt( 0 ) ) != nullptr) {
        delete item->widget();
        delete item;
    }

    // add crash buttons
    for (qint32 i = 0; i < nodeNum; ++i) {
        QCheckBox *checkbox = new QCheckBox(QString::number(ids[i]));
        ui->crashButtonsLayout->addWidget(checkbox, i / rowButtonNum, i % rowButtonNum);
        connect(checkbox, &QCheckBox::stateChanged, [ = ](bool state) {
            qint32 id  = checkbox->text().toInt();
            if (state == true) {
                network->takeNodeDown(id);

                QTreeWidgetItem *newItem = new QTreeWidgetItem(*lastItem);
                newItem->setText(id, "X");
                newItem->setBackgroundColor(id, QColor(205, 205, 205));

                ui->mainTreeWidget->addTopLevelItem(newItem);
                lastItem = newItem;
                ui->mainTreeWidget->scrollToBottom();
                allNodes[id].isUp = false;
            } else {
                network->bringNodeUp(id);

                QTreeWidgetItem *newItem = new QTreeWidgetItem(*lastItem);
                newItem->setText(id, "R");
                newItem->setBackgroundColor(id, QColor(205, 205, 205));

                ui->mainTreeWidget->addTopLevelItem(newItem);
                lastItem = newItem;
                ui->mainTreeWidget->scrollToBottom();
                allNodes[id].isUp = true;

                QJsonObject tempJson;
                tempJson.insert(QString(RaftNode::keyId), id);
                tempJson.insert(QString(RaftNode::keyEvent), RaftNode::eventBecameFollower);
                raftEvenet(tempJson);
            }
        });
    }

    // start all nodes
    for (qint32 i = 0; i < nodeNum; ++i) {
        connect(allNodes[i].realNode, SIGNAL(info(QJsonObject)), this, SLOT(raftEvenet(QJsonObject)));
        allNodes[i].realNode->run(ids, i, speed);
    }
}

void MainWindow::raftEvenet(QJsonObject event) {
    qint32 id = event[QString(RaftNode::keyId)].toInt();
    static const QMap<QChar, QColor> colorMap = {
        {'c', QColor(75, 117, 232)},
        {'l', QColor(251, 60, 90)},
        {'f', QColor(255, 255, 171)},
        {'o', QColor(205, 205, 205)},

    };
    // is first round
    if (!lastItem) {
        QTreeWidgetItem *newItem = new QTreeWidgetItem;
        for (int i = 0; i < ui->mainTreeWidget->columnCount(); ++i) {
            newItem->setText(i, "F");
            newItem->setBackgroundColor(i, colorMap['f']);
        }
        ui->mainTreeWidget->addTopLevelItem(newItem);
        lastItem = newItem;
        ui->mainTreeWidget->scrollToBottom();
        return;
    }

    // ignore down nodes
    if (!allNodes[id].isUp)
        return;

    switch (event[QString(RaftNode::keyEvent)].toInt()) {
        case RaftNode::eventBecameCandidate: {
            QTreeWidgetItem *newItem = new QTreeWidgetItem(*lastItem);
            newItem->setText(id, "C");
            newItem->setBackgroundColor(id, colorMap['c']);

            ui->mainTreeWidget->addTopLevelItem(newItem);
            lastItem = newItem;
            break;
        }
        case RaftNode::eventBecameFollower: {
            QTreeWidgetItem *newItem = new QTreeWidgetItem(*lastItem);
            newItem->setText(id, "F");
            newItem->setBackgroundColor(id, colorMap['f']);

            ui->mainTreeWidget->addTopLevelItem(newItem);
            lastItem = newItem;
            break;
        }
        case RaftNode::eventBecameLeader: {
            QTreeWidgetItem *newItem = new QTreeWidgetItem(*lastItem);
            newItem->setText(id, "L");
            newItem->setBackgroundColor(id, colorMap['l']);

            ui->mainTreeWidget->addTopLevelItem(newItem);
            lastItem = newItem;
            break;
        }
        case RaftNode::eventReceivedResponse: {
            qint32 voteNum = event[QString(RaftNode::keyVoteNum)].toInt();
            QTreeWidgetItem *newItem = new QTreeWidgetItem(*lastItem);
            newItem->setText(id, QString::number(voteNum));
            newItem->setBackgroundColor(id, colorMap['c']);

            ui->mainTreeWidget->addTopLevelItem(newItem);
            lastItem = newItem;
            break;
        }
    }
    ui->mainTreeWidget->scrollToBottom();
}


void MainWindow::stopSim() {
    ui->buttonStartSim->setEnabled(true);
    ui->buttonReset->setEnabled(false);
    for (auto &node : allNodes)
        delete node.realNode;
    allNodes.clear();
    ui->mainTreeWidget->clear();
    lastItem = nullptr;
    delete network;
    network = nullptr;

    // remove all widgets
    QLayoutItem *item;
    while ((item = ui->crashButtonsLayout->takeAt( 0 ) ) != nullptr) {
        delete item->widget();
        delete item;
    }
    ui->sliderNodeNum->setEnabled(true);
}

void MainWindow::nodeNumChanged(int num) {
    ui->labelNodeNum->setText(QString("Node Number: ") + QString::number(num));
}

void MainWindow::speedChanged(int speed) {
    if (network)
        network->setPropagationTime(((300 - speed) / 20));
    for (auto &node : allNodes) {
        node.realNode->setTimerSpeed(300 - speed);
    }
}
