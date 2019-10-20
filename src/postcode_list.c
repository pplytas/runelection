#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "postcode_list.h"
#include "utilities.h"


void pcl_init(PostCodeList *PCL) {
    PCL->head = NULL;
    PCL->count = 0;
}


PostCodeNode* create_postcode_node(int postcode) {
    PostCodeNode *new_postcode_node = (PostCodeNode *) malloc(sizeof(PostCodeNode));
    check_errors(new_postcode_node, "malloc", 1);

    VoterList VL;
    vl_init(&VL);
    new_postcode_node->VL = VL;
    new_postcode_node->postcode = postcode;
    new_postcode_node->next = NULL;

    return new_postcode_node;
}


PostCodeNode* pcl_find_node_by_postcode(PostCodeList PCL, int postcode) {
    PostCodeNode *found_node = PCL.head;

    while (found_node != NULL && found_node->postcode != postcode) {
        found_node = found_node->next;
    }

    return found_node;
}


void pcl_insert(PostCodeList *PCL, RedBlackNode *voter) {
    PostCodeNode *found_node =  pcl_find_node_by_postcode(*PCL, voter->postcode);

    if (found_node == NULL) {
        PostCodeNode *new_postcode_node = create_postcode_node(voter->postcode);
        new_postcode_node->next = PCL->head;
        PCL->head = new_postcode_node;

        (PCL->count)++;

        found_node = new_postcode_node;
    }

    vl_insert(&(found_node->VL), voter);
}


// void pcl_remove(PostCodeList *VL, char key[9]) {
//     VoterNode *node_to_delete = VL->head;
//     VoterNode *previous_node = NULL;

//     if (node_to_delete == NULL) return;

//     while (strcmp(node_to_delete->voter->key, key) != 0) {
//         previous_node = node_to_delete;
//         node_to_delete = node_to_delete->next;
//     }

//     if (previous_node == NULL) {        // Node to be deleted is the head
//         VL->head = node_to_delete->next;;
//     } else {                            // Node to be deleted is NOT the head
//         previous_node->next = node_to_delete->next;
//     }
//     free(node_to_delete);

//     (VL->count)--;
// }


void pcl_print_voted_per_postcode(PostCodeList PCL) {
    PostCodeNode *tmp_node = PCL.head;
    int voted_percentage;

    while (tmp_node != NULL) {
        voted_percentage = ((float)tmp_node->VL.have_voted_count / tmp_node->VL.voters_count) * 100;
        printf("# IN %d VOTERS-ARE %d%%\n", tmp_node->postcode, voted_percentage);
        tmp_node = tmp_node->next;
    }
}


void pcl_print_node(PostCodeNode *node) {
    printf("\t\tPostcode = %d\n", node->postcode);
    vl_print(node->VL);
}


void pcl_print(PostCodeList PCL) {
    PostCodeNode *tmp_node = PCL.head;

    printf("PostCodeList:\n");
    printf("\tCount = %d\n", PCL.count);
    printf("\tPostCodeNodes:\n\n");
    while(tmp_node != NULL) {
        pcl_print_node(tmp_node);
        tmp_node = tmp_node->next;
    }
    printf("\n");
}


void pcl_free_node(PostCodeNode *node) {
    vl_free(node->VL);
    free(node);
}


void pcl_free(PostCodeList PCL) {
    PostCodeNode *current_node = PCL.head;
    PostCodeNode *next_node;

    while (current_node != NULL) {
        next_node = current_node->next;
        pcl_free_node(current_node);
        current_node = next_node;
    }
}
