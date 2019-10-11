#ifndef UTILITIES_H
#define UTILITIES_H

#include "bloom_filter.h"
#include "red_black_tree.h"
#include "voter_list.h"


int get_prime_number(int);
int get_lines_count(FILE *);
void tokenize_string(char *, char *[6]);
void insert_records(BloomFilter *, RedBlackTree *, VoterList *, FILE *);

#endif
