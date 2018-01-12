#ifndef SEM_H_
#define SEM_H_

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};

/// @brief create semaphore of size `size`
/// @param[in] key the unique key
/// @param[in] size the size to create
/// @returnt the semaphore id
int create_Sem(int key, int size);

/// @brief destroy the semaphore with id `id`
/// @param[in] semid the semaphore id
void destroy_Sem(int semid);

///// @brief get semaphore with key and size
///// @param[in] key the semaphore key
///// @param[in] size the size of the semaphore
///// @return the semaphore id
//int get_Sem(int key, int size);

/// @brief set the `index` of the semaphore with id `id` to n
/// @param[in] semid the semaphore id to set
/// @param[in] index the index
/// @param[in] n the value
void set_N(int semid, int index, int n);

/// @brief do P operation on semaphore with id `semid`
/// @param[in] semid the semaphore id
/// @param[in] index the index to operate on
void P(int semid, int index);

/// @brief do V operation on semaphore with id `semid`
/// @param[in] semid the semaphore id
/// @param[in] index the index to operate on
void V(int semid, int index);

#endif // SEM_H_
