// TODO: implement this in client and server

//    /// @file basedataprocessor.cpp
//    /// @author Sixu Hu <husixu1@hotmail.com>
//    /// @date 10/2017/2017
//    /// @copyright GNU Public License v3
//    #include "basedataprocessor.h"
//    #include "baseconnector.h"
//
//    BaseDataProcessor::BaseDataProcessor() {
//
//        //> connect the signals
//        connect(&BaseConnector::getInstance(), SIGNAL(processData(QByteArray)),
//                this, SLOT(dataReceived(QByteArray)),
//                Qt::QueuedConnection);
//
//        //> start the thread and move this to the thread
//        thread.start();
//        moveToThread(&thread);
//    }
//
//    BaseDataProcessor &BaseDataProcessor::getInstance() {
//        static BaseDataProcessor instance;
//        return instance;
//    }
//
//    BaseDataProcessor::~BaseDataProcessor() {
//        thread.quit();
//        thread.wait();
//    }
//
//    void BaseDataProcessor::dataReceived(QByteArray data) {
//        processData(data);
//    }
//
