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
Hash the given key with 3 different hash functions and
set the resulting bit positions to 1.
*/
void insertToBloomFilter(BF* bloomFilter, char* key, int keyLength)
{
    size_t* hash1 = malloc(sizeof(size_t));
    size_t* hash2 = malloc(sizeof(size_t));
    size_t* hash3 = malloc(sizeof(size_t));

    // TODO: change last hash to be the result of the first two.
    MurmurHash3_x86_32(key, keyLength, 314, hash1);
    MurmurHash3_x86_32(key, keyLength, 159, hash2);
    MurmurHash3_x86_32(key, keyLength, 265, hash3);

    *hash1 %= bloomFilter->size * 32;
    *hash2 %= bloomFilter->size * 32;
    *hash3 %= bloomFilter->size * 32;

    bloomFilter->bitArray[*hash1 / 32] |= 1 << *hash1 % 32;
    bloomFilter->bitArray[*hash2 / 32] |= 1 << *hash2 % 32;
    bloomFilter->bitArray[*hash3 / 32] |= 1 << *hash3 % 32;
}

void printBits(uint32_t num)
{
for(int bit=0;bit<32; bit++)
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
    for(int i = 0; i < newBF->size; i++){
        printBits(newBF->bitArray[i]);
        printf("\n");
    }
    freeBloomFilter(newBF);
    return 0;
}