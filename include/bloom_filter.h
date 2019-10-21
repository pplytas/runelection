#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

typedef struct BloomFilter {
    unsigned char *bf;
    int count;
    int bits;
    int bytes;
    int updates_count;
    int max_updates_count;
} BloomFilter;

int bf_calculate_bits(int);
void bloom_init(BloomFilter *, int, int);
void bloom_reinit(BloomFilter *, int);
int bloom_increase_updates_count_and_check(BloomFilter *);
int test_bit(unsigned char *, unsigned int);
int set_bit(unsigned char *, unsigned int);
int bloom_check(BloomFilter, char *);
void bloom_add(BloomFilter *, char *);
void bloom_print(BloomFilter);
void bloom_free(BloomFilter);

#endif
