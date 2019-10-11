#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bloom_filter.h"
#include "murmurhash2.h"


int bloom_init(BloomFilter *BF, int size) {
    BF->count = 0;
    BF->bits = size;

    if (BF->bits % 8) {
        BF->bytes = (BF->bits / 8) + 1;
    } else {
        BF->bytes = BF->bits / 8;
    }

    BF->bf = (unsigned char *) calloc(BF->bytes, sizeof(unsigned char));
    if (BF->bf == NULL) {
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


int bloom_check(BloomFilter BF, void *buffer, int len) {
    int i;
    unsigned int a = murmurhash2(buffer, len, 0x9747b28c);
    unsigned int b = murmurhash2(buffer, len, a);
    unsigned int hash_bits;

    for (i = 0; i < 3; i++) {
        hash_bits = (a + i * b) % BF.bits;
        if (!test_bit(BF.bf, hash_bits)) {
            return 0;
        }
    }
    return 1;
}


int bloom_add(BloomFilter *BF, void *buffer, int len) {
    int i, hits = 0;
    unsigned int a = murmurhash2(buffer, len, 0x9747b28c);
    unsigned int b = murmurhash2(buffer, len, a);
    unsigned int hash_bits;

    for (i = 0; i < 3; i++) {
        hash_bits = (a + i * b) % BF->bits;
        if (set_bit(BF->bf, hash_bits)) {
            hits++;
        }
    }
    (BF->count)++;

    if (hits == 3) {
        return 1; // 1 == element already in (or collision)
    }
    return 0;
}

void bloom_print(BloomFilter BF) {
    printf("Bloom Filter\n");
    printf("\tcount = %d\n", BF.count);
    printf("\tbits = %d\n", BF.bits);
    printf("\tbytes = %d\n", BF.bytes);
}


void bloom_free(BloomFilter BF) {
    if (BF.bf != NULL) {
        free(BF.bf);
    }
}


int bloom_reset(BloomFilter *BF) {
    memset(BF->bf, 0, BF->bytes);
    return 0;
}