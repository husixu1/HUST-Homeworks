/**
 * @file main.c
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
#define _DEFAULT_SOURCE
#include"main.h"

#include<sys/stat.h>
#include<sys/types.h>
#include<sys/cdefs.h>
#include<bsd/string.h>

#include<grp.h>
#include<pwd.h>
#include<dirent.h>
#include<libgen.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<locale.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage %s <directory-or-file\n", argv[0]);
        return 1;
    }

    /// set system locale according to
    /// \ [this](https://www.gnu.org/software/libc/manual/html_node/Setting-the-Locale.html)
    /// \ for correct sorting order using the system locale sort
    setlocale(LC_COLLATE, "");

    // start pringting directory
    char *path = malloc(strlen(argv[1]) + 1);
    strcpy(path, argv[1]);
    printDir(path, 0);
    free(path);
}

static time_t now;

int printDir(char *dir, int depth) {
    // set current time
    now = time(NULL);

    // backup the old directory
    char *dirOld = NULL;
    long currentPathSize = pathconf(".", _PC_PATH_MAX);
    dirOld = getcwd(dirOld, currentPathSize);

    // get current directory status
    /// @quote Both dirname() and basename() may modify the contents of path,
    /// \ so it may be desirable to pass a copy when calling one of these functions.
    /// -- `man dirname.3`
    char *dirTemp = malloc(strlen(dir) + 1);
    strcpy(dirTemp, dir);
    char *dirBase = basename(dirTemp);
    struct stat statusTemp;
    stat(dirBase, &statusTemp);
    __blkcnt_t blockTotalCount = 0;

    // set current directory
    DIR *dirCurrent = NULL;
    // judge if absolute path
    if (dir[0] == '/') {
        if ((dirCurrent = opendir(dir)) == NULL) {
            perror("Failed to open directory");
            return 1;
        }
        chdir(dir);
    } else {
        if ((dirCurrent = opendir(dirBase)) == NULL) {
            perror("Failed to open directory");
            return 1;
        }
        chdir(dirBase);
    }

    // print current directory (following the upper directory)
    printf("%s:\n", dir);

    //////////////////////
    // Read All entries //
    //////////////////////

    // all entries in the current directory
    struct dirent *entryItems;
    // read entry nums
    struct dirent *entryTemp;
    size_t entryCount = 0;
    while ((entryTemp = readdir(dirCurrent)))
        entryCount++;

    // read all entries, add to entryItems
    entryItems = (struct dirent *)malloc(entryCount * sizeof(struct dirent));
    rewinddir(dirCurrent);
    for (int i = 0; (entryTemp = readdir(dirCurrent)); ++i)
        memcpy(entryItems + i, entryTemp, sizeof(struct dirent));

    // sort all entries
    qsort(entryItems, entryCount, sizeof(struct dirent), dirEntrySortCompare);

    ///////////////////////////////
    // Process current directory //
    ///////////////////////////////

    // get max link width, max size with, total block size,
    // and gather some other informations for formatting and printing
    __nlink_t maxLinkNum = 0;
    __off_t maxSize = 0;
    int sizeWidth = 0, linkWidth = 0;
    struct stat status;
    for (int i = 0; i < entryCount; ++i) {
        entryTemp = entryItems + i;
        stat(entryTemp->d_name, &status);
        // update max size
        if (status.st_size > maxSize)
            maxSize = status.st_size;


        // update total block size
        if (entryTemp->d_name[0] != '.' &&            // pass current file
                strcmp(entryTemp->d_name, ".") &&     // pass upper file
                strcmp(entryTemp->d_name, "..") ) {   // pass hidden file
            blockTotalCount += status.st_blocks;
            // update max link-num
            if (status.st_nlink > maxLinkNum)
                maxLinkNum = status.st_nlink;
        }
    }
    // calculate widths
    for (; maxLinkNum; maxLinkNum /= 10)
        ++linkWidth;
    for (; maxSize; maxSize /= 10)
        ++sizeWidth;

    /// print block size
    /// according to [this](https://stackoverflow.com/a/33730697) and `man stat.2`
    printf("total %lu\n", blockTotalCount * 512 / LS_BLOCK_SIZE );

    // read and print currnet entries
    rewinddir(dirCurrent);
    for (int i = 0; i < entryCount; ++i) {
        entryTemp = entryItems + i;
        // skip current and upper directory
        if (strcmp(entryTemp->d_name, ".") &&               // pass current file
                strcmp(entryTemp->d_name, "..") &&          // pass upper file
                entryTemp->d_name[0] != '.')                // pass hidden file
            printEntry(entryTemp, linkWidth, sizeWidth);
    }

    ///////////////
    // Recursion //
    ///////////////

    // recurse into deeper directories
    rewinddir(dirCurrent);
    for (int i = 0; i < entryCount; ++i) {
        entryTemp = entryItems + i;
        if (entryTemp->d_type == DT_DIR &&
                entryTemp->d_name[0] != '.' &&             // pass current file
                strcmp(entryTemp->d_name, ".") &&          // pass upper file
                strcmp(entryTemp->d_name, "..") ) {        // pass hidden file

            // construct path
            char *dirTemp = (char *)malloc(strlen(dir) + strlen(entryTemp->d_name) + 2);
            strcpy(dirTemp, dir);
            if (dir[strlen(dir) - 1] != '/')
                strcat(dirTemp, "/");
            strcat(dirTemp, entryTemp->d_name);

            // recurse into next directory
            putchar('\n');
            printDir(dirTemp, depth + 1);
            free(dirTemp);
        }
    }

    // clean resources and go back to the upper directory
    closedir(dirCurrent);
    chdir(dirOld);
    free(dirOld);
    free(entryItems);
    return 0;
}

int printEntry(struct dirent *entry, int fmtLinkWidth, int fmtSizeWidth) {
    struct stat status;

    // read and print status
    if (stat(entry->d_name, &status) == 0) {
        // get permission (strmode will append a space after the string)
        char strMode[13];
        strmode(status.st_mode, strMode);

        // get username and groupname
        struct passwd *passwdTemp;
        struct group *groupTemp;
        /// @quote The  return  value may point to a static area,
        /// \ and may be overwritten by subsequent calls to getpwent(3),
        /// \ getpwnam(), or getpwuid().  (Do not pass the returned pointer to free(3).)
        /// -- `man getpwuid`
        passwdTemp = getpwuid(status.st_uid);
        /// @quote The  return  value may point to a static area,
        /// \ and may be overwritten by subsequent calls to getpwent(3),
        /// \ getpwnam(), or getpwuid().  (Do not pass the returned pointer to free(3).)
        /// -- `man getgrgid`
        groupTemp = getgrgid(status.st_gid);

        // convert time to redable format
        /// @quote A timestamp is considered to be recent if it is less than six months old, and is not dated in the future. If a timestamp dated today is not listed in recent form, the timestamp is in the future, which means you probably have clock skew problems which may break programs like make that rely on file timestamps.
        /// -- from [here](https://www.gnu.org/software/coreutils/manual/coreutils.html#Formatting-file-timestamps)
        struct tm *timeTemp = localtime(&(status.st_mtime));
        char strTime[20];
        // six month tricks
        if (now - status.st_mtime > (365 * 24 * 60 * 60 / 2) || now < status.st_mtime) {
            strftime(strTime, 20, "%b %_d  %Y", timeTemp);
        } else {
            strftime(strTime, 20, "%b %_d %H:%M", timeTemp);
        }

        // print permission, hardlink-number, username, groupe name, size, modify-date, name
        printf("%s%*ld %s %s %*ld %s %s\n",
                strMode,
                fmtLinkWidth, status.st_nlink,
                passwdTemp->pw_name,
                groupTemp->gr_name,
                fmtSizeWidth, status.st_size,
                strTime,
                entry->d_name);
        return 0;
    }
    return 1;
}

int dirEntrySortCompare(const void *a, const void *b) {
    // the two directory
    struct dirent *dirA = (struct dirent *) a,
                   *dirB = (struct dirent *)b;

    // sort according to current locale
    return strcoll(dirA->d_name, dirB->d_name);
}
