/* 
Created by Antonis Karvelas, sdi1600060, for the 1st project of
K22: Operating Systems.
This is an implementation of a generic bloom filter.
*/

#include "BF.h" 

/*
Initialize a bloom filter with a given least size.
Find a good prime number for the size and then allocate
the appropriate amount of int32s.
It's basically an array of uint32s all set to 0.
*/
BF* initializeBloomFilter(size_t leastSize) {
    BF* bloomFilter = malloc(sizeof(BF));

    size_t size = findFirstPrime(leastSize, 3);
    unsigned int numberOfInt32s = (size / 32) + 1;

    bloomFilter->size = size;
    bloomFilter->bitArray = calloc(numberOfInt32s, 32);
    return bloomFilter;
}

/*
Free the given bloom filter's array of bits and
then the struct itself.
*/
void freeBloomFilter(BF* bloomFilter) {
    free(bloomFilter->bitArray);
    free(bloomFilter);
}

/*
Create a hash triplet to be used for inserting
and searching in the bloom filter.
*/
size_t** createHashTriplet(size_t bloomFilterSize, char* key, int keyLength) {
    size_t** hashTriplet = malloc(3 * sizeof(size_t));

    hashTriplet[0] = malloc(sizeof(size_t));
    hashTriplet[1] = malloc(sizeof(size_t));
    hashTriplet[2] = malloc(sizeof(size_t));

    *hashTriplet[0] = 0;
    *hashTriplet[1] = 0;
    *hashTriplet[2] = 0;

    MurmurHash3_x86_32(key, keyLength, 314, hashTriplet[0]);
    MurmurHash3_x86_32(key, keyLength, 159, hashTriplet[1]);
    MurmurHash3_x86_32(key, keyLength, 265, hashTriplet[2]);

    *hashTriplet[0] %= bloomFilterSize;
    *hashTriplet[1] %= bloomFilterSize;
    *hashTriplet[2] %= bloomFilterSize;

    return hashTriplet;
}

/*
Free a created hash triplet. For convenience.
*/
void freeHashTriplet(size_t** hashTriplet) {
    free(hashTriplet[0]);
    free(hashTriplet[1]);
    free(hashTriplet[2]);
    free(hashTriplet);
}

/*
Hash the given key with 3 different hash functions and
set the resulting bit positions to 1.
*/
void insertToBloomFilter(BF* bloomFilter, char* key, int keyLength) {
    size_t** hashTriplet = createHashTriplet(bloomFilter->size, key, keyLength);

    for(int i = 0; i <=2; i++)
    {
        bloomFilter->bitArray[*hashTriplet[i] / 32] |= 1 << *hashTriplet[i] % 32;
    }

    freeHashTriplet(hashTriplet);
}

/*
Search bloom filter. Return 0 if something certainly doesn't exist,
return 1 if something may or may not exist.
*/
bool searchBloomFilter(BF* bloomFilter, char* key, int keyLength) {
    size_t** hashTriplet = createHashTriplet(bloomFilter->size, key, keyLength);

    for(int i = 0; i <= 2; i++)
    {
        if(!(bloomFilter->bitArray[*hashTriplet[i] / 32] & 1 << *hashTriplet[i] % 32))
            return false;
    }

    freeHashTriplet(hashTriplet);  
    return true;
}

/*
Very simple function to check whether a given number is a prime or not.
*/
bool simplePrimeCheck(size_t number) {
    size_t upperLimit = (size_t)(sqrt(number));
    if(number % 2 == 0) return false;
    for(int i = 3; i < upperLimit; i += 2) {
        if(number % i == 0) return false;
    }
    return true;
}

/*
Given a size and a multiplier, find the first prime number x such
that x >= size*multiplier.
*/
size_t findFirstPrime(size_t size, int multiplier) {
    size_t number = size * multiplier;
    while(!simplePrimeCheck(number)) {
        number++;
    }
    return number;
}

//TODO: helper function, remove before the end.
void printUINT32Bits(uint32_t num) {
    for(int bitPos=0; bitPos<32; bitPos++)
    {
        printf("%i ", num & 0x01);
        num = num >> 1;
    }
}

