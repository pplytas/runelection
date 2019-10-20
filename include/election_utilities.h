#ifndef ELECTION_UTILITIES_H
#define ELECTION_UTILITIES_H

#include "bloom_filter.h"
#include "red_black_tree.h"
#include "postcode_list.h"


int insert_key(BloomFilter *, RedBlackTree *, PostCodeList *, char *[6]);
void insert_records(BloomFilter *, RedBlackTree *, PostCodeList *, FILE *);
RedBlackNode* find_key(BloomFilter, RedBlackTree, char *);
int vote_key(BloomFilter, RedBlackTree *, PostCodeList, char *);
void load_fileofkeys(BloomFilter, RedBlackTree *, PostCodeList, char *);
void listen_for_commands(BloomFilter *, RedBlackTree *, PostCodeList *);
void write_new_registry(RedBlackTree, char *);

#endif
