#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities.h"


int main(int argc, char *argv[]) {
    int lines_count, bf_size;
    FILE *infile;

    BloomFilter BF;
    RedBlackTree RBT;
    VoterList VL;

    // Open input file
    infile = fopen(argv[2], "r");
    if (infile == NULL) {
        perror("fopen");
        return -1;
    }

    // Get number of records
    lines_count = get_lines_count(infile);
    printf("Number of lines: %d\n", lines_count);

    // Find optimal number of bits for Bloom Filter
    bf_size = get_prime_number(lines_count * 3);

    // Init data structures
    bloom_init(&BF, bf_size);
    rbt_init(&RBT);
    vl_init(&VL);

    // Insert records to data structures
    insert_records(&BF, &RBT, &VL, infile);

    // Close file
    fclose(infile);

    // Print data structures
    bloom_print(BF);
    // rbt_print(RBT);
    vl_print(VL);

    // Free allocated memory of data structures
    vl_free(VL);
    rbt_free(RBT);
    bloom_free(BF);

    return 0;
}
