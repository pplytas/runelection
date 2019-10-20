#ifndef POST_CODE_LIST_H
#define POST_CODE_LIST_H

#include "voter_list.h"


typedef struct PostCodeNode PostCodeNode;

struct PostCodeNode {
    int postcode;
    VoterList VL;
    PostCodeNode *next;
};

typedef struct PostCodeList {
    PostCodeNode *head;
    int count;
} PostCodeList;

void pcl_init(PostCodeList *);
PostCodeNode* create_postcode_node(int);
PostCodeNode* pcl_find_node_by_postcode(PostCodeList, int);
void pcl_insert(PostCodeList *, RedBlackNode *);
void pcl_print_voted_per_postcode(PostCodeList);
void pcl_print_node(PostCodeNode *);
void pcl_print(PostCodeList);
void pcl_free_node(PostCodeNode *);
void pcl_free(PostCodeList);

#endif
