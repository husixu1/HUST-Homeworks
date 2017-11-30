/// @file basefiletransfer.cpp
/// @author Sixu Hu <husixu1@hotmail.com>
/// @date 10/2017/2017
/// @copyright GNU Public License v3
#include "basefiletransceiver.h"

void BaseFileTransceiver::insertToSendMap(
        QSharedPointer<OperationDescriptor> operation,
        QSharedPointer<BaseFileTransceiver::FileInfo> info) {
    //> insert to three send maps
    pendingSendMap.insert(operation, info);
    sendIdentifierMap.insert(
            info->address.toString()
            + QString::number(info->port)
            + QString::number(info->identifier), operation);
    sendTimerMap.insert(&info->timeoutTimer, operation);
}

void BaseFileTransceiver::removeFromSendMap(QSharedPointer<OperationDescriptor> operation) {
    if (pendingSendMap.contains(operation)) {
        auto info  = pendingSendMap[operation];
        sendTimerMap.remove(&info->timeoutTimer);
        sendIdentifierMap.remove(info->address.toString()
                + QString::number(info->port)
                + QString::number(info->identifier));
        pendingSendMap.remove(operation);
    }
}

void BaseFileTransceiver::insertToReceiveMap(
        QSharedPointer<OperationDescriptor> operation,
        QSharedPointer<BaseFileTransceiver::FileInfo> info) {
    //> insert to three receive maps
    pendingReceiveMap.insert(operation, info);
    receiveIdentifierMap.insert(
            info->address.toString()
            + QString::number(info->port)
            + QString::number(info->identifier), operation);
    receiveTimerMap.insert(&info->timeoutTimer, operation);
}

void BaseFileTransceiver::removeFromReceiveMap(QSharedPointer<OperationDescriptor> operation) {
    if (pendingSendMap.contains(operation)) {
        auto info  = pendingReceiveMap[operation];
        receiveTimerMap.remove(&info->timeoutTimer);
        receiveIdentifierMap.remove(info->address.toString()
                + QString::number(info->port)
                + QString::number(info->identifier));
        pendingReceiveMap.remove(operation);
    }
}

BaseFileTransceiver::~BaseFileTransceiver() {
    sendTimer->stop();
    delete sendTimer;
}

void BaseFileTransceiver::init() {
    QThread *objectThread = this->thread();
    QThread *currentThread = QThread::currentThread();

    //> this object must be inited in another thread
    Q_ASSERT (objectThread != currentThread);

    //> execute in the thread that this object is running on
    QMetaObject::invokeMethod(this, "initialize", Qt::QueuedConnection);
}

void BaseFileTransceiver::sendTimerTimedOut() {
    QByteArray dataGram;
    QByteArray rawData;
    char convertBuf[8];
    uint32_t crc;

    // only send if has data
    if (!pendingSendMap.isEmpty()) {
        // send all files simultaineously
        for (auto info : pendingSendMap) {

            // get a data that is not marked received by receiver
            for (; info->pos < info->progress.size() && info->progress.testBit(info->pos); info->pos++);

            // send data if pos does not get to the end
            if (info->pos < info->progress.size()) {
                //> placeholder for CRC
                dataGram.append(4, 0);
                //> control byte
                dataGram.append('D');

                //> compose a message and send
                //>> identifier
                memcpy(convertBuf, &(info->identifier), 2);
                dataGram.append(convertBuf, 2);
                //>> seqNum
                memcpy(convertBuf, &(info->pos), 4);
                dataGram.append(convertBuf, 4);
                //>> data
                info->file.seek(info->pos * segmentSize);
                rawData = info->file.read(segmentSize);
                dataGram.append(rawData);

                //> fill crc32
                //crc = crc32(dataGram.right(dataGram.size() - 4).data(), dataGram.size() - 4);
                crc = crc32(dataGram.data() + 4, dataGram.size() - 4);
                memcpy(convertBuf, &(crc), 4);
                dataGram.replace(0, 4, convertBuf, 4);

                //> send data !
                socket->writeDatagram(dataGram, info->address, info->port);
                qDebug() << "segment " << info->pos << " send";
                info->pos++;

            }

            //> point to next segment
            if (info->pos >= info->progress.size())
                info->pos = 0;
        }
    }
}

void BaseFileTransceiver::processData() {
    QNetworkDatagram dataGram;
    QByteArray rawData;
    uint32_t crc;
    uint32_t standartCrc;

    // deal with data
    while (socket->hasPendingDatagrams()) {
        //> receive data
        dataGram = socket->receiveDatagram(segmentSize + headerSize + 1);

        //> discard invalid datagram
        if (!dataGram.isValid()) {
            qDebug() << "package discarded due to invalid";
            continue;
        }
        if (dataGram.data().size() < headerSize) {
            qDebug() << "package discarded due to too small";
            continue;
        }

        //> check the validity of data
        crc = crc32(dataGram.data().right(dataGram.data().size() - 4),
                        dataGram.data().size() - 4);
        memcpy(&standartCrc, dataGram.data().left(4).data(), 4);
        //> discard invalid datagram
        if (crc != standartCrc) {
            qDebug() << "package discarded due to invalid crc";
            continue;
        }

        quint16 identifier;
        memcpy(&identifier, dataGram.data().mid(5, 2), 2);
        //> get the identity
        QString identity = dataGram.senderAddress().toString()
                + QString::number(dataGram.senderPort())
                + QString::number(identifier);

        // judge if the received data type
        if (dataGram.data()[4] == 'A') {
            // if ACK datagram received, "this" is sender

            //> get the operation
            if (!sendIdentifierMap.contains(identity)) {
                qDebug() << "package A discarded due to sender not found, identity = " << identity;
                continue;
            }
            QSharedPointer<OperationDescriptor> operation = sendIdentifierMap[identity];
            QSharedPointer<FileInfo> info = pendingSendMap[operation];

            //> get data from datagram, set progress
            int pos;
            memcpy(&pos, dataGram.data().mid(7, 4), 4);
            info->progress[pos] = true;
            //> reset timer
            info->timeoutTimer.start(timeoutInterval);

            // if all ACK received
            int segNum = info->progress.count(true);
            if (segNum == info->progress.size()) {
                //> send fin datagarm
                QByteArray FIN(dataGram.data().left(headerSize));
                FIN[4] = 'F';
                crc = crc32(FIN.right(FIN.size() - 4), FIN.size() - 4);
                memcpy(FIN.data(), &crc, 4);
                socket->writeDatagram(FIN, info->address, info->port);
                qDebug() << "all ACK received, sending FIN";

                emit forwardFileSent(operation);

                //> release resources
                info->file.close();
                removeFromSendMap(operation);
            } else {
                emit forwardFileProgress(operation, static_cast<quint64>(info->pos) * segmentSize);
                qDebug() << "ACK " << pos << " received";
            }

        } else if (dataGram.data()[4] == 'E') {
            // discard unidentified datagrams
            if (!sendIdentifierMap.contains(identity))
                continue;

            // if ALL datagram received, "this" is sender
            QSharedPointer<OperationDescriptor> operation = sendIdentifierMap[identity];
            QSharedPointer<FileInfo> info = pendingSendMap[operation];

            QByteArray FIN(dataGram.data().left(headerSize));
            FIN[4] = 'F';
            crc = crc32(FIN.right(FIN.size() - 4), FIN.size() - 4);
            memcpy(FIN.data(), &crc, 4);
            socket->writeDatagram(FIN, info->address, info->port);
            qDebug() << "ALL received, FIN send, releasing resources";

            //> release resources
            info->file.close();
            //> stop timer
            info->timeoutTimer.stop();
            removeFromSendMap(operation);

            //> send signal
            emit forwardFileSent(operation);

        } else if (dataGram.data()[4] == 'F') {
            // discard unidentified datagrams
            if (!receiveIdentifierMap.contains(identity))
                continue;

            // if FIN datagram received, "this" is receiver
            QSharedPointer<OperationDescriptor> operation = receiveIdentifierMap[identity];
            QSharedPointer<FileInfo> info = pendingReceiveMap[operation];
            //> send signal
            qDebug() << "FIN received, releasing resources";

            //> release resources
            info->file.close();
            //> stop timer
            info->timeoutTimer.stop();
            removeFromReceiveMap(operation);

        } else if (dataGram.data()[4] == 'D') {
            // if normal datagram received, "this" is receiver

            //> discard unidentified datagram
            if (!receiveIdentifierMap.contains(identity)) {
                qDebug() << "package D discarded due to senderte not found, identity = " << identity;
                continue;
            }

            //> record progress
            QSharedPointer<OperationDescriptor> operation = receiveIdentifierMap[identity];
            QSharedPointer<FileInfo> info = pendingReceiveMap[operation];

            //> check if all received
            int segmentNum = info->progress.count(true);

            // send ALL-received for redundance
            if (segmentNum == info->progress.size()) {
                // send ALL-received
                QByteArray ALL(dataGram.data().left(headerSize));
                ALL[4] = 'E';
                crc = crc32(ALL.right(ALL.size() - 4), ALL.size() - 4);
                memcpy(ALL.data(), &crc, 4);
                socket->writeDatagram(ALL, info->address, info->port);
                qDebug() << "ALL-received sent";

                info->file.close();
                continue;
            }

            //> set progress
            int pos;
            memcpy(&pos, dataGram.data().mid(7, 4), 4);
            info->progress[pos] = true;
            //> reset the timer
            info->timeoutTimer.start(timeoutInterval);

            //> write data
            if (info->progress.testBit(pos)) {
                rawData = dataGram.data().right(dataGram.data().size() - headerSize);
                info->file.seek(pos * segmentSize);
                info->file.write(rawData);
                qDebug() << "segment " << pos << " received, writing to file";
            }

            //> check if all received
            segmentNum = info->progress.count(true);

            // send ACK or ALL-received
            if (segmentNum == info->progress.size()) {
                // send ALL-received
                QByteArray ALL(dataGram.data().left(headerSize));
                ALL[4] = 'E';
                crc = crc32(ALL.right(ALL.size() - 4), ALL.size() - 4);
                memcpy(ALL.data(), &crc, 4);
                socket->writeDatagram(ALL, info->address, info->port);
                qDebug() << "ALL segment received, ALL sent";

                info->file.close();

                //> emit progress changed
                emit forwardFileReceived(operation, info->file.fileName().toUtf8());
            } else  {
                //> send ACK
                QByteArray ACK(dataGram.data().left(headerSize));
                ACK[4] = 'A';
                crc = crc32(ACK.right(ACK.size() - 4), ACK.size() - 4);
                memcpy(ACK.data(), &crc, 4);
                socket->writeDatagram(ACK, info->address, info->port);
                qDebug() << "ACK " << pos << " sent";

                //> emit progress changed
                emit forwardFileProgress(operation, static_cast<quint64>(segmentNum) * segmentSize);
            }
        }
    }
}

void BaseFileTransceiver::pendingSendTimedOut() {
    QTimer *timer = qobject_cast<QTimer *>(sender());

    qDebug() << "pending send timed out, relasing resources";
    QSharedPointer<OperationDescriptor> operation = sendTimerMap[timer];
    Q_ASSERT(!operation.isNull());

    emit forwardSendReceiveError(operation, OperationDescriptor::errSocketTimedOut);

    //> release resources
    QSharedPointer<FileInfo> info = pendingSendMap[operation];
    //>> close the file
    info->file.close();
    removeFromSendMap(operation);
}

void BaseFileTransceiver::pendingReceiveTimedOut() {
    QTimer *timer = qobject_cast<QTimer *>(sender());

    qDebug() << "pending receive timed out, relasing resources";
    QSharedPointer<OperationDescriptor> operation = receiveTimerMap[timer];
    Q_ASSERT(!operation.isNull());

    emit forwardSendReceiveError(operation, OperationDescriptor::errSocketTimedOut);

    //> release resources
    QSharedPointer<FileInfo> info = pendingReceiveMap[operation];
    //>> remove the file rather than close it normally
    info->file.remove();
    removeFromReceiveMap(operation);
}

void BaseFileTransceiver::initialize() {
    //> create the file socket
    socket = new QUdpSocket;

    // try binding the socket to random port
    if (!socket->bind()) {
        emit failedToListen();
        return;
    }

    emit listenStarted(socket->localPort());

    connect(socket, SIGNAL(readyRead()), this, SLOT(processData()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SIGNAL(unexpectedError(QAbstractSocket::SocketError)));

    //> start the send timer
    sendTimer = new QTimer(this);
    connect(sendTimer, SIGNAL(timeout()), this, SLOT(sendTimerTimedOut()));
    sendTimer->start(sentTimerTimeoutInterval);
}

void BaseFileTransceiver::sendFile(
        QSharedPointer<OperationDescriptor> operation,
        QString fileName, quint16 identifer, QHostAddress address, quint16 port) {

    //> connect necessary signals
    if (!operation.isNull()) {
        connect(this, SIGNAL(forwardSendReceiveError(
                                QSharedPointer<OperationDescriptor>, OperationDescriptor::Error)),
                operation.data(), SLOT(forwardSendReceiveError(
                                QSharedPointer<OperationDescriptor>, OperationDescriptor::Error)),
                Qt::QueuedConnection);

        connect(this, SIGNAL(forwardFileSent(QSharedPointer<OperationDescriptor>)),
                operation.data(), SLOT(forwardDataSent(QSharedPointer<OperationDescriptor>)),
                Qt::QueuedConnection);

        connect(this, SIGNAL(forwardFileProgress(QSharedPointer<OperationDescriptor>, quint64)),
                operation.data(), SLOT(forwardFileProgress(QSharedPointer<OperationDescriptor>, quint64)),
                Qt::QueuedConnection);
    }

    //> try to open the file
    QSharedPointer<FileInfo> fileInfo(new FileInfo);
    fileInfo->file.setFileName(fileName);
    if (!fileInfo->file.open(QIODevice::ReadOnly)) {
        emit forwardSendReceiveError(operation, OperationDescriptor::errFailedOpenFile);
        return;
    }

    //> if file size is too large
    if (static_cast<quint64>(fileInfo->file.size()) >= UINT32_MAX * segmentSize) {
        emit forwardSendReceiveError(operation, OperationDescriptor::errDataGramTooLong);
        return;
    }

    //> complete infos and insert the fileInfo to the sending map
    fileInfo->address = address;
    fileInfo->port = port;
    fileInfo->identifier = identifer;
    fileInfo->pos = 0;
    fileInfo->progress.fill(false,
            (fileInfo->file.size() / segmentSize) +
            static_cast<quint64>(static_cast<bool>(fileInfo->file.size() % segmentSize)) );

    //TODO: send/receive two file with same identify number emit error

    //> insert to map and start timer
    insertToSendMap(operation, fileInfo);
    connect(&(fileInfo->timeoutTimer), SIGNAL(timeout()),
            this, SLOT(pendingSendTimedOut()));
    fileInfo->timeoutTimer.start(timeoutInterval);
}

void BaseFileTransceiver::createPendingReceive(
        QSharedPointer<OperationDescriptor> operation,
        QString fileName, quint64 fileSize, quint16 identifier,
        QHostAddress senderAddress, quint16 senderPort) {

    //> connect signals
    if (!operation.isNull()) {
        connect(this, SIGNAL(forwardSendReceiveError(
                                QSharedPointer<OperationDescriptor>, OperationDescriptor::Error)),
                operation.data(), SLOT(forwardSendReceiveError(
                                QSharedPointer<OperationDescriptor>, OperationDescriptor::Error)),
                Qt::QueuedConnection);

        connect(this, SIGNAL(forwardFileReceived(QSharedPointer<OperationDescriptor>, QByteArray)),
                operation.data(), SLOT(forwardDataReceived(QSharedPointer<OperationDescriptor>, QByteArray)),
                Qt::QueuedConnection);

        connect(this, SIGNAL(forwardFileProgress(QSharedPointer<OperationDescriptor>, quint64)),
                operation.data(), SLOT(forwardFileProgress(QSharedPointer<OperationDescriptor>, quint64)),
                Qt::QueuedConnection);
    }

    QSharedPointer<FileInfo> info(new FileInfo);
    info->file.setFileName(fileName);
    if (!info->file.open(QIODevice::WriteOnly)) {
        qDebug() << "failed to open file for writing";
        emit forwardSendReceiveError(operation, OperationDescriptor::errFailedOpenFile);
        return;
    }

    if (!info->file.resize(fileSize)) {
        qDebug() << "faild to create file (space not enough)";
        info->file.close();
        emit forwardSendReceiveError(operation, OperationDescriptor::errDataGramTooLong);
        return;
    }

    //> init file info insert to pending receive map
    info->address = senderAddress;
    info->port = senderPort;
    info->identifier = identifier;
    info->pos = 0;
    info->progress.fill(false,
            (fileSize / segmentSize) +
            static_cast<quint64>(static_cast<bool>(fileSize % segmentSize)) );

    //> insert to map and start timer
    insertToReceiveMap(operation, info);
    connect(&(info->timeoutTimer), SIGNAL(timeout()),
            this, SLOT(pendingReceiveTimedOut()));
    info->timeoutTimer.start(timeoutInterval);
}


static const uint32_t crc32_tab[] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3,	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de,	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,	0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5,	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,	0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940,	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,	0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

uint32_t BaseFileTransceiver::crc32(const char *buf, quint64 size) {
    uint32_t crc;

    crc = ~0U;
    while (size--)
        crc = crc32_tab[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
    return crc ^ ~0U;
}
