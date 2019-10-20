#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

typedef struct BloomFilter {
    int count;
    int bits;
    int bytes;
    unsigned char *bf;
} BloomFilter;

void bloom_init(BloomFilter *, int);
int test_bit(unsigned char *, unsigned int);
int set_bit(unsigned char *, unsigned int);
int bloom_check(BloomFilter, char *);
int bloom_add(BloomFilter *, char *);
void bloom_print(BloomFilter);
int bloom_reset(BloomFilter *);
void bloom_free(BloomFilter);

#endif
