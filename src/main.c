#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities.h"
#include "bloom_filter.h"
#include "red_black_tree.h"


int main(int argc, char *argv[]) {
    int lines_count, bf_size;
    FILE *infile;

    BloomFilter BF;
    RedBlackTree RBT;

    // Open input file
    infile = fopen(argv[2], "r");
    if (infile == NULL) {
        perror("fopen");
        return -1;
    }

    // Get number of records
    lines_count = get_lines_count(infile);
    printf("Number of lines: %d\n", lines_count);

    // Find optimal number of bits for bloom filter
    bf_size = get_prime_number(lines_count * 3);

    // Create bloom filter
    bloom_init(&BF, bf_size);
    bloom_print(BF);

    // Insert records to red-black tree
    insert_records(infile, &RBT);
    // rbt_print(RBT);

    // Close file and free allocated memory
    fclose(infile);

    rbt_free(RBT);
    bloom_free(BF);

    return 0;
}
