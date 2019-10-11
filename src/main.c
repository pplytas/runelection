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

    // Find optimal number of bits for Bloom Filter
    bf_size = get_prime_number(lines_count * 3);

    // Init data structures
    bloom_init(&BF, bf_size);
    rbt_init(&RBT);

    // Insert records to data structures
    insert_records(infile, &BF, &RBT);

    // Close file
    fclose(infile);

    // Print data structures
    bloom_print(BF);
    // rbt_print(RBT);

    // Free allocated memory of data structures
    rbt_free(RBT);
    bloom_free(BF);

    return 0;
}
