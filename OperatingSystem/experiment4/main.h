/**
 * @file main.h
 * @brief An experiment trys to reimplement ls -lR
 *
 * @details
 * This program is expected to have the exactly same output with ls -lR,
 * note that ls -lR behaves differently on different system/machines so
 * maybe the outputs are not exactly the same (I'll try my best).
 *
 * @date 2017-11-23
 * @author Sixu Hu
**/
#ifndef MAIN_H_
#define MAIN_H_

#include<dirent.h>
#include<time.h>

/// @brief Print directories recursively rooted at dir
/// @param[in] dir The root directory to print
/// @param[in] depth Max depth to print
int printDir(char *dir, int depth);

/// @brief Print one directory entry
/// @param[in] entry The entry to print
/// @param[in] fmtLinkWidth The width of link in displaying
/// @param[in] fmtSizeWidth The width of file size in displaying
/// @return 0 if print is successful
int printEntry(struct dirent *entry, int fmtLinkWidth, int fmtSizeWidth);

/// @brief The compare function used to sort two directory entries
/// @param a One dir entry
/// @param b Another dir entry
/// @return An int indicating the order.
int dirEntrySortCompare(const void *a, const void *b);

#define LS_BLOCK_SIZE 1024

#endif // MAIN_H_
