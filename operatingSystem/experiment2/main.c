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
// thread operation threadFlag
int threadFlag = 0;

int main() {
    // initialize semaphore
    key_t semkey;
    if ((semkey = ftok("/tmp", 'a')) == (key_t)(-1)) {
        perror("IPC error: ftok");
        exit(1);
    }
    semId = create_Sem(semkey, 2);
    set_N(semId, 0);

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
    for (int i = 1; i <= 100;) {
        P(semId, 0);
        if (threadFlag == 0) {
            sum += i;
            ++i;
            threadFlag = 1;
        }
        V(semId, 0);
    }
    return NULL;
}

void *routineThread2(void *param) {
    while (1) {
        P(semId, 0);
        if (threadFlag == 1) {
            printf("temp sum = %d\n", sum);
            threadFlag = 0;
        }
        V(semId, 0);

        // try to wait for thread 1
        if (pthread_tryjoin_np(thread1, NULL) == 0)
            return NULL;
    }
}

