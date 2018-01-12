/**
 * @file main.h
 * @brief fork and pipe experiment
 *
 * @details
 * this is part of the operting system course lab
 *
 * @date 2017-11-17
 * @author Sixu Hu
**/
#ifndef MAIN_H_
#define MAIN_H_

// maximum messages
#define MAX_MESSAGE_SIZE 1024


/// @brief the child process1
/// this process send message to pipe to child process 2
/// @param[in] piped[2] the pipe
/// @return if the process exited normally
int procChild1(int piped[2]);

/// @brief the child process 2
/// this process receive signal from pipe from child process 1
/// @param[in] piped[2] the pipe
/// @return if the process exited normally
int procChild2(int piped[2]);

/// @brief the signal handler of the main process
/// @param sig required
void handlerMain(int sig);

/// @brief the signal handler of the child 1 process
/// @param sig required
void handlerChild1(int sig);

/// @brief the signal handler of the child 2 process
/// @param sig required
void handlerChild2(int sig);

#endif // MAIN_H_
