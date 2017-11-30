#ifndef MURMURHASH_H_INCLUDED
#define MURMURHASH_H_INCLUDED

#include "common.h"

/* do not change it if not necessary */
#define HASH_SEED	0
/* use the murmurhash3 algorithm */
/* you can change the hash algorithm without worrying damaging the program */
uint32_t hash_32(const char *key, uint32_t seed);

#endif // MURMURHASH_H_INCLUDED
