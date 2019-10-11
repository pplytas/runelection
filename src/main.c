#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities.h"
#include "bloom_filter.h"
#include "red_black_tree.h"


int main(int argc, char *argv[]) {
    int lines_count, bf_size;
    FILE *infile;

    BloomFilter *my_bloom_ptr = (BloomFilter*) malloc(sizeof(BloomFilter));
    Node *root = NULL;

    // Open input file
    infile = fopen(argv[2], "r");
    if (infile == NULL) {
        perror("fopen");
        return -1;
    }

    lines_count = get_lines_count(infile);
    printf("Number of lines: %d\n", lines_count);

    bf_size = get_prime_number(lines_count * 3);
    printf("%d\n", bf_size);

    bloom_init(my_bloom_ptr, bf_size);
    bloom_print(my_bloom_ptr);
    printf("\n");

    insert_records(infile, &root);

    // rbt_print(root);

    fclose(infile);
    bloom_free(my_bloom_ptr);
    rbt_free(root);

    return 0;
}
