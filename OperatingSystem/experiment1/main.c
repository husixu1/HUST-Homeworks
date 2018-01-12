#define _POSIX_SOURCE
#include"main.h"

#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
#include<stdlib.h>
#include<stdio.h>

// the process id of two child process
static int pidChild1, pidChild2;

int main(int argc, char *argv[]) {
    // register the main process signal handler
    signal(SIGINT, handlerMain);

    int piped[2];
    // create a pipe
    pipe(piped);

    // try fork the first child
    if ((pidChild1 = fork()) == -1)
        return -1;
    // run child process
    if (pidChild1 == 0) {
        // ignore the interrupt signal
        signal(SIGINT, SIG_IGN);
        // register the child 1 signal handler
        signal(SIGUSR1, handlerChild1);
        return procChild1(piped);
    }

    // try fork the second child
    if ((pidChild2 = fork()) == -1)
        return -1;
    // run child process
    if (pidChild2 == 0) {
        // ignore the interrupt signal
        signal(SIGINT, SIG_IGN);
        // register the child 2 signal handler
        signal(SIGUSR1, handlerChild2);
        return procChild2(piped);
    }

    close(piped[0]);
    close(piped[1]);
    // close unused pipe
    while (1) {
        // reduce CPU occupation
        sleep(1);
    }
    return 0;
}

int procChild1(int piped[2]) {
    close(piped[0]);

    // the message buffer
    char buffer[MAX_MESSAGE_SIZE];

    // run the main loop
    unsigned counterSend = 1;
    // the pipe discriptor
    for (;; ++counterSend) {
        // interval: 1 second
        sleep(1);
        // initialize and write message to the pipe
        sprintf(buffer, "I send you %u times", counterSend);
        write(piped[1], buffer, strlen(buffer));
    }
}

int procChild2(int piped[2]) {
    // initialize the buffer
    char buffer[MAX_MESSAGE_SIZE];
    // close unused pipe
    close(piped[1]);

    ssize_t size;
    while (1) {
        memset(buffer, 0, MAX_MESSAGE_SIZE);
        size = read(piped[0], buffer, MAX_MESSAGE_SIZE);
        // handle error
        if (size < 0)
            return -1;

        // print the message read
        buffer[size] = 0;
        printf("%s\n", buffer);
    }
}

void handlerMain(int sig) {
    kill(pidChild1, SIGUSR1);
    kill(pidChild2, SIGUSR1);
    int status;
    waitpid(pidChild1, &status, 0);
    waitpid(pidChild2, &status, 0);
    printf("Parent Process is Killed!\n");
    exit(0);
}

void handlerChild1(int sig) {
    printf("Child Process 1 is Killed by Parent!\n");
    exit(0);
}

void handlerChild2(int sig) {
    printf("Child Process 2 is Killed by Parent!\n");
    exit(0);
}
