#define _GNU_SOURCE
#include"main.h"
#include"sem.h"

#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/sem.h>
#include<sys/stat.h>
#include<errno.h>
#include<stdlib.h>
#include<limits.h>


// the global semaphore
static int semId;
// the sum
static int sum = 0;
// two threads
static pthread_t thread1, thread2;
// termination flag
static int terminateFlag = 0;

int main() {
    // initialize semaphore
    key_t semkey;
    if ((semkey = ftok("/tmp", 'a')) == (key_t)(-1)) {
        perror("IPC error: ftok");
        exit(1);
    }
    semId = create_Sem(semkey, 2);
    set_N(semId, 0, 0);
    set_N(semId, 1, 1);

    // create two thread
    pthread_create(&thread1, NULL, routineThread1, NULL);
    pthread_create(&thread2, NULL, routineThread2, NULL);

    // wait for threads to finish
    pthread_join(thread2, NULL);

    // destroy semaphore
    destroy_Sem(semId);
    return 0;
}

void *routineThread1(void *param) {
    // calaulate the sum from 1 to 100
    for (int i = 1; i <= 100; ++i) {
        P(semId, 1);
        sum += i;
        V(semId, 0);
    }
    terminateFlag = 1;
    return NULL;
}

void *routineThread2(void *param) {
    while (1) {
        P(semId, 0);
        printf("temp sum = %d\n", sum);

        // try to wait for thread 1
        if (terminateFlag) {
            pthread_join(thread1, NULL);
            return NULL;
        }

        V(semId, 1);
    }
    return NULL;
}

