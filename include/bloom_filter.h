#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

typedef struct BloomFilter {
    int count;
    int bits;
    int bytes;
    unsigned char *bf;
} BloomFilter;

int bloom_init(BloomFilter *, int);
int test_bit(unsigned char *, unsigned int);
int set_bit(unsigned char *, unsigned int);
int bloom_check(BloomFilter, void *, int);
int bloom_add(BloomFilter *, void *, int);
void bloom_print(BloomFilter);
void bloom_free(BloomFilter);
int bloom_reset(BloomFilter *);

#endif