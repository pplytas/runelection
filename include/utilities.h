#ifndef UTILITIES_H
#define UTILITIES_H

#include "bloom_filter.h"
#include "red_black_tree.h"
#include "postcode_list.h"


int get_optimal_bf_size(int);
int get_lines_count(FILE *);
void tokenize_string(char *, char *[6]);
void insert_records(BloomFilter *, RedBlackTree *, PostCodeList *, FILE *);
void listen_for_commands(BloomFilter *, RedBlackTree *, PostCodeList *);

#endif
