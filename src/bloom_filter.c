#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bloom_filter.h"
#include "murmurhash2.h"
#include "utilities.h"


int bf_calculate_bits(int number) {
    int i, flag = 1;
    int min_prime;

    while(flag == 1) {
        flag = 0;
        for(i = 2; i < (number / 2); i++) {
            if(number % i == 0) {
                flag = 1;
                break;
            }
        }

        if (flag == 0) {
            min_prime = number;
        }

        number++;
    }

    return min_prime;
}


void bloom_init(BloomFilter *BF, int records_count, int max_updates_count) {
    BF->bits = bf_calculate_bits(records_count * 3);
    if ((BF->bits % 8) > 0) {
        BF->bytes = (BF->bits / 8) + 1;
    } else {
        BF->bytes = BF->bits / 8;
    }

    BF->bf = (unsigned char *) calloc(BF->bytes, sizeof(unsigned char));
    check_errors(BF->bf, "malloc", 1);

    BF->max_updates_count = max_updates_count;
    BF->count = 0;
    BF->updates_count = 0;
}


void bloom_reinit(BloomFilter *BF, int new_records_count) {
    int max_updates_count = BF->max_updates_count;
    bloom_free(*BF);
    bloom_init(BF, new_records_count, max_updates_count);
}


int bloom_increase_updates_count_and_check(BloomFilter *BF) {
    (BF->updates_count)++;
    return BF->updates_count >= BF->max_updates_count;
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


void bloom_add(BloomFilter *BF, char *string) {
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
}


void bloom_print(BloomFilter BF) {
    printf("Bloom Filter\n");
    printf("\tCount = %d\n", BF.count);
    printf("\tUpdates Count = %d\n", BF.updates_count);
    printf("\tMax Updates Count = %d\n", BF.max_updates_count);
    printf("\tBits = %d\n", BF.bits);
    printf("\tBytes = %d\n\n", BF.bytes);
}


void bloom_free(BloomFilter BF) {
    if (BF.bf != NULL) {
        free(BF.bf);
    }
}
