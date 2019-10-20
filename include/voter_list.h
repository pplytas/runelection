#ifndef VOTER_LIST_H
#define VOTER_LIST_H

#include "red_black_tree.h"

typedef struct VoterNode VoterNode;

struct VoterNode {
    RedBlackNode *voter;
    VoterNode *next;
};

typedef struct VoterList {
    VoterNode *head;
    int voters_count;
    int have_voted_count;
} VoterList;

void vl_init(VoterList *);
VoterNode* create_voter_node(RedBlackNode *);
VoterNode* vl_find_node_by_key(VoterList, char *);
void vl_insert(VoterList *, RedBlackNode *);
void vl_increase_have_voted_count(VoterList *, int);
void vl_remove(VoterList *, char *);
void vl_print(VoterList);
void vl_free(VoterList);

#endif
