/* 
Create by Antonis Karvelas, sdi1600060.
This is an implementation of a generic bloom filter for the 1st project of
K22: Operating Systems.
*/

#include "BF.h" 

/*
Initialize a bloom filter with a given size.
It's basically an array of uint32s all set to 0.
*/
BF* initializeBloomFilter(size_t size) 
{
    BF* bloomFilter = malloc(sizeof(BF));
    bloomFilter->size = size;
    bloomFilter->bitArray = calloc(size, 32);
    return bloomFilter;
}

/*
Free the given bloom filter's array of bits and
then the struct itself.
*/
void freeBloomFilter(BF* bloomFilter)
{
    free(bloomFilter->bitArray);
    free(bloomFilter);
}

/*
Create a hash triplet to be used for inserting
and searching in the bloom filter.
*/
size_t** createHashTriplet(size_t bloomFilterSize, char* key, int keyLength)
{
    size_t** hashTriplet = malloc(3 * sizeof(size_t*));

    hashTriplet[0] = malloc(sizeof(size_t));
    hashTriplet[1] = malloc(sizeof(size_t));
    hashTriplet[2] = malloc(sizeof(size_t));

    MurmurHash3_x86_32(key, keyLength, 314, hashTriplet[0]);
    MurmurHash3_x86_32(key, keyLength, 159, hashTriplet[1]);
    MurmurHash3_x86_32(key, keyLength, 265, hashTriplet[2]);

    *hashTriplet[0] %= bloomFilterSize * 32;
    *hashTriplet[1] %= bloomFilterSize * 32;
    *hashTriplet[2] %= bloomFilterSize * 32;

    return hashTriplet;
}

/*
Hash the given key with 3 different hash functions and
set the resulting bit positions to 1.
*/
void insertToBloomFilter(BF* bloomFilter, char* key, int keyLength)
{
    size_t** hashTriplet = createHashTriplet(bloomFilter->size, key, keyLength);

    for(int i = 0; i <=2; i++)
    {
        bloomFilter->bitArray[*hashTriplet[i] / 32] |= 1 << *hashTriplet[i] % 32;
    }
}

/*
Search bloom filter. Return 0 if something certainly doesn't exist,
return 1 if something may or may not exist.
*/
int searchBloomFilter(BF* bloomFilter, char* key, int keyLength)
{
    size_t** hashTriplet = createHashTriplet(bloomFilter->size, key, keyLength);

    for(int i = 0; i <= 2; i++)
    {
        if(!(bloomFilter->bitArray[*hashTriplet[i] / 32] & 1 << *hashTriplet[i] % 32))
        return 0;
    }
    
    return 1;
}

//TODO: helper function, remove before the end.
void printUINT32Bits(uint32_t num)
{
    for(int bitPos=0; bitPos<32; bitPos++)
    {
        printf("%i ", num & 0x01);
        num = num >> 1;
    }
}

int main() {
    BF* newBF = initializeBloomFilter(10);
    printf("%i\n", newBF->bitArray[0]);
    insertToBloomFilter(newBF, "hello", 5);
    insertToBloomFilter(newBF, "england", 7);

    int b = searchBloomFilter(newBF, "hello", 5);
    printf("%i\n", b);
    b = searchBloomFilter(newBF, "aaa", 3);
    printf("%i\n", b);
    b = searchBloomFilter(newBF, "england", 7);
    printf("%i\n", b);
    for(int i = 0; i < newBF->size; i++){
        printUINT32Bits(newBF->bitArray[i]);
        printf("\n");
    }
    freeBloomFilter(newBF);
    return 0;
}