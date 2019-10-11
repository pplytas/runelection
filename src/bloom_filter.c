#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bloom_filter.h"
#include "murmurhash2.h"


int bloom_init(BloomFilter *bloom, int size) {
    bloom->bits = size;

    if (bloom->bits % 8) {
        bloom->bytes = (bloom->bits / 8) + 1;
    } else {
        bloom->bytes = bloom->bits / 8;
    }

    bloom->bf = (unsigned char *) calloc(bloom->bytes, sizeof(unsigned char));
    if (bloom->bf == NULL) {
        return 1;
    }

    return 0;
}


int test_bit(unsigned char *buf, unsigned int x) {
    unsigned int byte = x >> 3;
    unsigned char c = buf[byte]; // expensive memory access
    unsigned int mask = 1 << (x % 8);

    if (c & mask) {
        return 1;
    } else {
        return 0;
    }
}


int set_bit(unsigned char *buf, unsigned int x) {
    unsigned int byte = x >> 3;
    unsigned char c = buf[byte]; // expensive memory access
    unsigned int mask = 1 << (x % 8);

    if (c & mask) {
        return 1;
    } else {
        buf[byte] = c | mask;
        return 0;
    }
}


int bloom_check(BloomFilter *bloom, const void *buffer, int len) {
    int i;
    unsigned int a = murmurhash2(buffer, len, 0x9747b28c);
    unsigned int b = murmurhash2(buffer, len, a);
    unsigned int hash_bits;

    for (i = 0; i < 3; i++) {
        hash_bits = (a + i * b) % bloom->bits;
        if (!test_bit(bloom->bf, hash_bits)) {
            return 0;
        }
    }
    return 1;
}


int bloom_add(BloomFilter *bloom, const void *buffer, int len) {
    int i;
    int hits = 0;
    unsigned int a = murmurhash2(buffer, len, 0x9747b28c);
    unsigned int b = murmurhash2(buffer, len, a);
    unsigned int hash_bits;

    for (i = 0; i < 3; i++) {
        hash_bits = (a + i * b) % bloom->bits;
        if (set_bit(bloom->bf, hash_bits)) {
            hits++;
        }
    }

    if (hits == 3) {
        return 1; // 1 == element already in (or collision)
    }

    return 0;
}

void bloom_print(BloomFilter *bloom) {
    printf("bloom at %p\n", (void * ) bloom);
    printf(" ->bits = %d\n", bloom->bits);
    printf(" ->bytes = %d\n", bloom->bytes);
}


void bloom_free(BloomFilter *bloom) {
    if (bloom != NULL) {
        free(bloom->bf);
        free(bloom);
    }
}


int bloom_reset(BloomFilter *bloom) {
    memset(bloom->bf, 0, bloom->bytes);
    return 0;
}