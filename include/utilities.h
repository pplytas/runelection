#ifndef UTILITIES_H
#define UTILITIES_H

#include "bloom_filter.h"
#include "red_black_tree.h"
#include "postcode_list.h"


void check_errors(void *, char *, int);
void get_command_line_arguments(int, char *[], char *[3]);
int get_optimal_bf_size(int);
int get_lines_count(FILE *);
int get_record_info_from_tokenized_string(char *, char *[6]);
int get_record_info(char *, char *[6]);
int insert_key(BloomFilter *, RedBlackTree *, PostCodeList *, char *[6]);
void insert_records(BloomFilter *, RedBlackTree *, PostCodeList *, FILE *);
RedBlackNode* find_key(BloomFilter, RedBlackTree, char[9]);
int vote_key(BloomFilter, RedBlackTree *, PostCodeList, char[9]);
void load_fileofkeys(BloomFilter, RedBlackTree *, PostCodeList, char *);
void listen_for_commands(BloomFilter *, RedBlackTree *, PostCodeList *);
void write_new_registry(RedBlackTree, char *);

#endif
