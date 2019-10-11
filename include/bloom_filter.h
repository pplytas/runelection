#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

/** ***************************************************************************
 * Structure to keep track of one bloom filter.  Caller needs to
 * allocate this and pass it to the functions below. First call for
 * every struct must be to bloom_init().
 *
 */
typedef struct bloom {
  int bits;
  int bytes;
  unsigned char * bf;
} BloomFilter;

int bloom_init(BloomFilter *, int);
int test_bit(unsigned char *, unsigned int);
int set_bit(unsigned char *, unsigned int);
int bloom_check(BloomFilter *, const void *, int);
int bloom_add(BloomFilter *, const void *, int);
void bloom_print(BloomFilter *);
void bloom_free(BloomFilter *);
int bloom_reset(BloomFilter *);

#endif