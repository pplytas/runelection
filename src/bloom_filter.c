#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bloom_filter.h"
#include "murmurhash2.h"
#include "utilities.h"


void bloom_init(BloomFilter *BF, int bits) {
    BF->count = 0;
    BF->bits = bits;

    if (BF->bits % 8) {
        BF->bytes = (BF->bits / 8) + 1;
    } else {
        BF->bytes = BF->bits / 8;
    }

    BF->bf = (unsigned char *) calloc(BF->bytes, sizeof(unsigned char));
    check_errors(BF->bf, "malloc", 1);
}


int test_bit(unsigned char *buf, unsigned int x) {
    unsigned int byte = x >> 3;
    unsigned char c = buf[byte];
    unsigned int mask = 1 << (x % 8);

    if (c & mask) {
        return 1;
    }

    return 0;
}


int set_bit(unsigned char *buf, unsigned int x) {
    unsigned int byte = x >> 3;
    unsigned char c = buf[byte];
    unsigned int mask = 1 << (x % 8);

    if (c & mask) {
        return 1;
    }

    buf[byte] = c | mask;
    return 0;
}


int bloom_check(BloomFilter BF, char *string) {
    int length = strlen(string);
    unsigned int a = murmurhash2(string, length, 0x9747b28c);
    unsigned int b = murmurhash2(string, length, a);
    unsigned int hash_bits;

    for (int i = 0; i < 3; i++) {
        hash_bits = (a + i * b) % BF.bits;
        if (!test_bit(BF.bf, hash_bits)) return 0;
    }

    return 1;
}


int bloom_add(BloomFilter *BF, char *string) {
    int hits = 0;
    int length = strlen(string);
    unsigned int a = murmurhash2(string, length, 0x9747b28c);
    unsigned int b = murmurhash2(string, length, a);
    unsigned int hash_bits;

    for (int i = 0; i < 3; i++) {
        hash_bits = (a + i * b) % BF->bits;
        if (set_bit(BF->bf, hash_bits)) {
            hits++;
        }
    }
    (BF->count)++;

    if (hits == 3) {
        return 1;
    }

    return 0;
}

void bloom_print(BloomFilter BF) {
    printf("Bloom Filter\n");
    printf("\tCount = %d\n", BF.count);
    printf("\tBits = %d\n", BF.bits);
    printf("\tBytes = %d\n\n", BF.bytes);
}


int bloom_reset(BloomFilter *BF) {
    memset(BF->bf, 0, BF->bytes);
    return 0;
}


void bloom_free(BloomFilter BF) {
    if (BF.bf != NULL) {
        free(BF.bf);
    }
}
