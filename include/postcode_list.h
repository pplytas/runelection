#ifndef POST_CODE_LIST_H
#define POST_CODE_LIST_H

#include "voter_list.h"


typedef struct PostCodeNode PostCodeNode;

struct PostCodeNode {
    char postcode[6];
    VoterList VL;
    PostCodeNode *next;
};

typedef struct PostCodeList {
    PostCodeNode *head;
    int postcodes_count;
    int voters_count;
} PostCodeList;

void pcl_init(PostCodeList *);
PostCodeNode* create_postcode_node(char[6]);
PostCodeNode* pcl_find_node_by_postcode(PostCodeList, char[6]);
void pcl_insert(PostCodeList *, RedBlackNode *);
void pcl_print_node(PostCodeNode *);
void pcl_print(PostCodeList);
void pcl_free_node(PostCodeNode *);
void pcl_free(PostCodeList);

#endif
