/*
 * Physical memory manager using Binary-Buddy System
 *
 * The algorithm is described in D.S. Hirschberg:
 * “A class of dynamic memory allocation algorithms”
 *
 * URL: https://doi.org/10.1145/362375.362392
 *
 * Influenced from:
 * URL: https://www.kernel.org/doc/gorman/html/understand/understand009.html
 * URL: https://people.engr.tamu.edu/bettati/Courses/313/2014A/Labs/mp1_fibo.pdf
 *
 * Author: Tuna CICI
 */

#ifndef PHYSICAL_H
#define PHYSICAL_H

#include <stdint.h>

#define PAGE_SIZE 4096 /* Bytes */
#define MAX_ORDER 11 /* Block size: 2^0 ... 2^(MAX_ORDER - 1) * PAGE_SIZE */

typedef struct freeAreaStruct {
        uint64_t *listHead;
        /* TODO: How to keep track of freemem? */
} freeArea_t;

/* TODO: Thing to answer before implementing shi': */
/* TODO:        Which data structure? */
/* TODO:        How to keep track of free PAGEs? */
/* TODO:        Maybe a bitmap? Okay. definitely a bitmap. */
/* TODO:        How to allocate space for the data structure itself? */
/* TODO:        Can't use any static size like RAM_SIZE because reasons. */
/* TODO:        Maybe 'calculate' needed space inside init_allocator()? */
/* TODO:        Then skip that area like: startAddr + calcBuddyPoolSize? */
/* TODO:        Is this a stupid design? Need more research and asking around */
/* TODO:        Btw see: kernel.org/doc/html/v4.19/core-api/boot-time-mm.html*/

uint64_t init_allocator(const uint64_t *startAddr, const uint64_t *endAddr);

#endif /* PHYSICAL_H */
