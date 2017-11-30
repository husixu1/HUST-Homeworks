#ifndef MAIN_H_
#define MAIN_H_

#include<stdio.h>
#include<stdint.h>

#define BUFFERNUM 1024

/// @brief the shared memory data size (in bytes, not including control messages)
/// This value must be less than (2 ^ 31 - 1)
#define DATASIZE 1024

#define STATUS_PENDING  0x01
#define STATUS_READ     0x02
#define STATUS_WRITTEN  0x04
#define STATUS_ALL      0x08

#define STATUS_HEAD     0x10

#define STATUS_TAIL     0x20

/// @brief the message structure
typedef struct _Message{
    unsigned char status;   //!< current shared buffer status
    uint32_t size;          //!< current message size
    int  nextShmId;         //!< pointer to the next Message
    char data[DATASIZE];    //!< where data is stored
}__attribute__((packed)) Message;

/// @brief the size of buffer header (used to store read/write status and bytes read)
/// |<- status 1byte ->|<- size 4bytes ->|
#define SIZE_HEADER 5

/// @brief the reader's process
/// @param[in] inFile the input file descriptor
/// @param[in] idShmHead the head shared memory id of the buffer
/// @param[in] idShmTail the tail shared memory id of the buffer
/// @param[in] idSem the semaphore id
/// @return return value, 0 for success
int procRead(FILE *inFile, int idShmHead, int idShmTail, int idSem);

/// @brief the writer's process
/// @param[in] outFile the output file descriptor
/// @param[in] idShmHead the head shared memory id of the buffer
/// @param[in] idShmTail the tail shared memory id of the buffer
/// @param[in] idSem the semaphore id
/// @return return value, 0 for success
int procWrite(FILE *outFile, int idShmHead, int idShmTail, int idSem);

#endif // MAIN_H_
