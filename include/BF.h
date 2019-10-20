/* 
Created by Antonis Karvelas, sdi1600060, for the 1st project of
K22: Operating Systems.
This is an implementation of a generic bloom filter.
*/

#ifndef ERGASIA_1_BF_H
#define ERGASIA_1_BF_H

#include <stdio.h>

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <math.h>

#include "murmur3.h"

// We'll use 32-bit integers to store 32 bits.
struct BF {
    uint32_t* bitArray;
    size_t size;
} typedef BF;


BF*         initializeBloomFilter(size_t leastSize);
void        freeBloomFilter(BF* bloomFilter);
size_t**    createHashTriplet(size_t bloomFilterSize, char* key, int keyLength);
void        freeHashTriplet(size_t** hashTriplet);
void        insertToBloomFilter(BF* bloomFilter, char* key, int keyLength);
bool        searchBloomFilter(BF* bloomFilter, char* key, int keyLength);
bool        simplePrimeCheck(size_t number);
size_t      findFirstPrime(size_t size, int multiplier);

#endif //ERGASIA_1_BF_H