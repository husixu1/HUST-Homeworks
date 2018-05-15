/*
 * csim.c - A cache simulator that can replay traces from Valgrind
 *     and output statistics such as number of hits, misses, and
 *     evictions.  The replacement policy is LRU.
 *
 * Implementation and assumptions:
 *  1. Each load/store can cause at most one cache miss. (I examined the trace,
 *  the largest request I saw was for 8 bytes).
 *  2. Instruction loads (I) are ignored, since we are interested in evaluating
 *  trans.c in terms of its data cache performance.
 *  3. data modify (M) is treated as a load followed by a store to the same
 *  address. Hence, an M operation can result in two cache hits, or a miss and a
 *  hit plus an possible eviction.
 *
 * The function printSummary() is given to print output.
 * Please use this function to print the number of hits, misses and evictions.
 * This is crucial for the driver to evaluate your work.
 */
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include "cachelab.h"

// #define DEBUG_ON
#define ADDRESS_LENGTH 64

/* Type: Memory address */
typedef unsigned long long int mem_addr_t;

/* Type: Cache line
   LRU is a counter used to implement LRU replacement policy  */
typedef struct cache_line {
    char valid;
    mem_addr_t tag;
    unsigned long long int lru;
} cache_line_t;

typedef cache_line_t *cache_set_t;
typedef cache_set_t *cache_t;

/* Globals set by command line args */
int verbosity = 0; /* print trace if set */
int s = 0; /* set index bits */
int b = 0; /* block offset bits */
int E = 0; /* associativity */
char *trace_file = NULL;

/* Derived from command line args */
int S; /* number of sets */
int B; /* block size (bytes) */

/* Counters used to record cache statistics */
int miss_count = 0;
int hit_count = 0;
int eviction_count = 0;
unsigned long long int lru_counter = 1;

/* The cache we are simulating */
cache_t cache;
mem_addr_t set_index_mask;

/*
 * initCache - Allocate memory, write 0's for valid and tag and LRU
 * also computes the set_index_mask
 */
void initCache() {
    int i, j;
    cache = (cache_set_t *) malloc(sizeof(cache_set_t) * S);
    for (i = 0; i < S; i++) {
        cache[i] = (cache_line_t *) malloc(sizeof(cache_line_t) * E);
        for (j = 0; j < E; j++) {
            cache[i][j].valid = 0;
            cache[i][j].tag = 0;
            cache[i][j].lru = 0;
        }
    }

    /* Computes set index mask */
    set_index_mask = (mem_addr_t) (pow(2, s) - 1);
}


/*
 * freeCache - free allocated memory
 */
void freeCache() {
    int i;
    for (i = 0; i < S; ++i)
        free(cache[i]);
    free(cache);
}


/*
 * accessData - Access data at memory address addr.
 *   If it is already in cache, increast hit_count
 *   If it is not in cache, bring it in cache, increase miss count.
 *   Also increase eviction_count if a line is evicted.
 */
void accessData(mem_addr_t addr) {
    int i;
    /* unsigned long long int eviction_lru = ULONG_MAX; */
    /* unsigned int eviction_line = 0; */
    mem_addr_t set_index = (addr >> b) & set_index_mask;
    mem_addr_t tag = addr >> (s + b);

    cache_set_t cache_set = cache[set_index];

    /* judge if hit */
    for (i = 0; i < E; ++i) {
        /* hit */
        if (cache_set[i].valid && cache_set[i].tag == tag) {
            if (verbosity)
                printf("hit ");
            ++hit_count;

            /* update entry whose lru is less than the current lru (newer) */
            for (int j = 0; j < E; ++j)
                if (cache_set[j].valid && cache_set[j].lru < cache_set[i].lru)
                    ++cache_set[j].lru;
            cache_set[i].lru = 0;
            return;
        }
    }

    /* missed */
    if (verbosity)
        printf("miss ");
    ++miss_count;

    /* find the biggest lru or the first invlaid line */
    int j, maxIndex = 0;
    unsigned long long maxLru = 0;
    for (j = 0; j < E && cache_set[j].valid; ++j) {
        if (cache_set[j].lru >= maxLru) {
            maxLru = cache_set[j].lru;
            maxIndex = j;
        }
    }

    if (j != E) {
        /* found an invalid entry */
        /* update other entries */
        for (int k = 0; k < E; ++k)
            if (cache_set[k].valid)
                ++cache_set[k].lru;
        /* insert line */
        cache_set[j].lru = 0;
        cache_set[j].valid = 1;
        cache_set[j].tag = tag;
    } else {
        /* all entry is valid, replace the oldest one*/
        if (verbosity)
            printf("eviction ");
        ++eviction_count;
        for (int k = 0; k < E; ++k)
            ++cache_set[k].lru;
        cache_set[maxIndex].lru = 0;
        cache_set[maxIndex].tag = tag;
    }
}


/*
 * replayTrace - replays the given trace file against the cache
 */
void replayTrace(char *trace_fn) {
    char buf[1000];
    mem_addr_t addr = 0;
    unsigned int len = 0;
    FILE *trace_fp = fopen(trace_fn, "r");

    int testcount = 0;

    while (fscanf(trace_fp, " %c %llx,%d", buf, &addr, &len) > 0) {
        if (verbosity && buf[0] != 'I')
            printf("%c %llx,%d ", buf[0], addr, len);
        switch (buf[0]) {
            case 'I':
                break;
            case 'L':
            case 'S':
                accessData(addr);
                ++testcount;
                break;
            case 'M':
                accessData(addr);
                accessData(addr);
                ++testcount;
                break;
            default:
                break;
        }
        if (verbosity && buf[0] != 'I')
            putchar('\n');
    }
    fclose(trace_fp);
}

void debug() {
    printf("========================\n");
    for (int i = 0; i < S; ++i) {
        for (int j = 0; j < E; ++j)
            printf("%d,%10llx,%4llu | ", cache[i][j].valid, cache[i][j].tag, cache[i][j].lru);
        printf("\n");
    }
}

/*
 * printUsage - Print usage info
 */
void printUsage(char *argv[]) {
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}

/*
 * main - Main routine
 */
int main(int argc, char *argv[]) {
    char c;

    while ( (c = getopt(argc, argv, "s:E:b:t:vh")) != -1) {
        switch (c) {
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            case 'v':
                verbosity = 1;
                break;
            case 'h':
                printUsage(argv);
                exit(0);
            default:
                printUsage(argv);
                exit(1);
        }
    }

    /* Make sure that all required command line args were specified */
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        printUsage(argv);
        exit(1);
    }

    /* Compute S, E and B from command line args */
    S = (unsigned int) pow(2, s);
    B = (unsigned int) pow(2, b);

    /* Initialize cache */
    initCache();

#ifdef DEBUG_ON
    printf("DEBUG: S:%u E:%u B:%u trace:%s\n", S, E, B, trace_file);
    printf("DEBUG: set_index_mask: %llu\n", set_index_mask);
#endif

    replayTrace(trace_file);

    /* Free allocated memory */
    freeCache();

    /* Output the hit and miss statistics for the autograder */
    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}
