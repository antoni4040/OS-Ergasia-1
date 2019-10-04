/* Create by Antonis Karvelas, sdi1600060.
This is an implementation of a generic bloom filter for the 1st project of
K22: Operating Systems.
*/

#include <stdio.h>

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "murmur3.h"

// We'll use 32-bit integers to store 32 bits.
struct BF {
    uint32_t* bitArray;
    size_t size;
} typedef BF;


