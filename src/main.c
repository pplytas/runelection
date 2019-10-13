#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities.h"


int main(int argc, char *argv[]) {
    int lines_count, bf_size;
    FILE *infile;

    BloomFilter BF;
    RedBlackTree RBT;
    PostCodeList PCL;

    // Open input file
    infile = fopen(argv[2], "r");
    if (infile == NULL) {
        perror("fopen");
        return -1;
    }

    // Get number of records
    lines_count = get_lines_count(infile);
    printf("Number of lines in file: %d\n\n", lines_count);

    // Find optimal number of bits for Bloom Filter
    bf_size = get_optimal_bf_size(lines_count * 3);

    // Init data structures
    bloom_init(&BF, bf_size);
    rbt_init(&RBT);
    pcl_init(&PCL);

    // Insert records to data structures
    insert_records(&BF, &RBT, &PCL, infile);

    // Close file
    fclose(infile);

    // Print data structures
    bloom_print(BF);
    // rbt_print(RBT);
    pcl_print(PCL);

    // Listen for user input
    listen_for_commands(&BF, &RBT, &PCL);

    // Free allocated memory of data structures
    pcl_free(PCL);
    rbt_free(RBT);
    bloom_free(BF);

    return 0;
}
