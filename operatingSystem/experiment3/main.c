#define _GNU_SOURCE
#include"../common/log.c/src/log.h"

#include"main.h"
#include"sem.h"

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>

// the process id
static int idProcRead = 0, idProcWrite = 0;

int main(int argc, char *argv[]) {
    // display hint
    if (argc != 3) {
        printf("Usage: %s <source-file> <dst-file>", argv[0]);
        return 1;
    }

    // check files' validity
    FILE *fileIn;
    FILE *fileOut;
    if ((fileIn = fopen(argv[1], "rb")) == NULL) {
        perror("failed to open file for reading.");
        return 1;
    }
    if ((fileOut = fopen(argv[2], "wb")) == NULL) {
        perror("failed to open file for writing,");
        return 1;
    }

    // get a unique key
    key_t shmkey;
    if ((shmkey = ftok("./", 'a')) == (key_t)(-1)) {
        perror("IPC error: ftok");
        exit(1);
    }
    // create the shared memory and initialise it
    int idHeadShm;
    if ((idHeadShm = shmget(shmkey, sizeof(Message), IPC_CREAT | 0660)) <= 0) {
        perror("Failed to create shared memory");
        exit(1);
    }
    Message *messageTemp = (Message *)shmat(idHeadShm, NULL, 0);
    if((int64_t)(messageTemp) == -1){
        perror("Failed to retrieve shared memory");
        exit(1);
    }
    messageTemp->status = STATUS_PENDING | STATUS_HEAD | STATUS_TAIL;

    // create the circle buffer
    for(int i=0; i < BUFFERNUM; ++i){
        // get a unique key
        key_t shmkey;
        if ((shmkey = ftok("./", i)) == (key_t)(-1)) {
            perror("IPC error: ftok");
            exit(1);
        }
        // create the shared memory and initialise it
        int idShm;
        if ((idShm = shmget(shmkey, sizeof(Message), IPC_CREAT | 0660)) <= 0) {
            perror("Failed to create shared memory");
            exit(1);
        }
        messageTemp->nextShmId = idShm;
        messageTemp->status = STATUS_PENDING;
        messageTemp = (Message *)shmat(idShm, NULL, 0);
        if((int64_t)(messageTemp) == -1){
            perror("Failed to retrieve shared memory");
            exit(1);
        }
    }
    messageTemp->nextShmId = idHeadShm;

    // get a unique key
    key_t semKey;
    if ((semKey = ftok("./", 'b')) == (key_t)(-1)) {
        perror("IPC error: ftok");
        exit(1);
    }
    // create lock for PV operation
    int idSem;
    if ((idSem = create_Sem(semKey, 4)) < 0) {
        perror("Failed to create semaphore");
        exit(1);
    }
    set_N(idSem, 0, 0);
    set_N(idSem, 1, BUFFERNUM);
    set_N(idSem, 2, 1);

    // create read process
    if ((idProcRead = fork()) == -1) {
        perror("Failed to create process.");
        return 1;
    }
    // run the reader's process
    if (idProcRead == 0)
        return procRead(fileIn, idHeadShm, idHeadShm, idSem);

    // create write process
    if ((idProcWrite = fork()) == -1) {
        perror("Failed to create process.");
        // clean former threads
        kill(idProcRead, SIGKILL);
        return 1;
    }
    // run the writer's process
    if (idProcWrite == 0)
        return procWrite(fileOut, idHeadShm, idHeadShm, idSem);

    // wait two process to stop
    int status;
    waitpid(idProcRead, &status, 0);
    waitpid(idProcWrite, &status, 0);

    destroy_Sem(idSem);
    return 0;
}

int procRead(FILE *inFile, int idShmHead, int idShmTail, int idSem) {
    // start to read file to buffer
    while (1) {
        Message *messageTail = (Message *)shmat(idShmTail, NULL, 0);
        Message *messageNextTail = (Message *)shmat(messageTail->nextShmId, NULL, 0);

        P(idSem, 1);
        P(idSem, 2);
        // read if quque is not full
        size_t bytesRead;
        if ((bytesRead = fread((void *)(messageTail->data), 1, DATASIZE, inFile)) == 0) {
            // if file read finished
            messageTail->status = STATUS_ALL;
            messageTail->size = bytesRead;
            fclose(inFile);
            V(idSem, 0);
            V(idSem, 1);
            V(idSem, 2);
            return 0;
        }
        messageTail->size = bytesRead;
        messageTail->status = STATUS_READ;

        // move tail to next node
        messageNextTail->status |= STATUS_TAIL;
        idShmTail = messageTail->nextShmId;
        V(idSem, 2);
        V(idSem, 0);
    }
}

int procWrite(FILE *outFile, int idShmHead, int idShmTail, int idSem) {
    while (1) {
        Message *messageHead = (Message *)shmat(idShmHead, NULL, 0);

        P(idSem, 0);
        P(idSem, 2);
        // if all the messages are read from file
        if (messageHead->status & STATUS_ALL) {
            fwrite((void *)(messageHead->data), messageHead->size, 1, outFile);
            fclose(outFile);
            V(idSem, 0);
            V(idSem, 2);
            return 0;
        }

        fwrite((void *)(messageHead->data), messageHead->size, 1, outFile);
        messageHead->status = STATUS_WRITTEN;
        // clear current node and move head to next
        messageHead->status = STATUS_PENDING;
        idShmHead = messageHead->nextShmId;
        messageHead = (Message *)shmat(messageHead->nextShmId, NULL, 0);
        messageHead->status |= STATUS_HEAD;
        V(idSem, 2);
        V(idSem, 0);
    }
}
