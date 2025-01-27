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

#include <stdint.h>

#include "Memory/PageDef.h"
#include "Memory/Physical.h"

#include "LibKern/Console.h"

uint64_t init_allocator(const uint8_t *startAddr, const uint8_t *endAddr)
{
        uint64_t retValue = 0; /* Bytes ready to be allocated */
        
        uint8_t *alignedStart = (uint8_t*) PALIGN(startAddr);
        uint8_t *alignedEnd = (uint8_t*) PALIGN(endAddr - PAGE_SIZE + 1);
        
        retValue = (alignedEnd - alignedStart) / PAGE_SIZE;

        return retValue;
}
