/**
 * @file main.h
 * @brief threas synchronize experiment
 *
 * @details
 * this is part of the operating system labs
 *
 * @date 2017-11-17
 * @author Sixu Hu
**/
#ifndef MAIN_H_
#define MAIN_H_

#include<unistd.h>
#include<sys/sem.h>

/// @brief routine of the first thread
/// This function is called when first thread started
/// @param[in] param the general parameter
/// @return a pointer to nonetype
void *routineThread1(void * param);

/// @brief routine of the second thread
/// This function is called when first thread started
/// @param[in] param the general parameter
/// @return a pointer to nonetype
void *routineThread2(void * param);

#endif // MAIN_H_
