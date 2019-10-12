#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "voter_list.h"


void vl_init(VoterList *VL) {
    VL->head = NULL;
    VL->count = 0;
}


VoterNode* create_voter_node(RedBlackNode *voter) {
    VoterNode *new_voter_node = (VoterNode *) malloc(sizeof(VoterNode));
    new_voter_node->voter = voter;
    new_voter_node->next = NULL;
    return new_voter_node;
}


VoterNode* vl_find_node_by_key(VoterList VL, char key[9]) {
    VoterNode *tmp_node = VL.head;

    while (tmp_node != NULL && strcmp(tmp_node->voter->key, key) != 0) {
        tmp_node = tmp_node->next;
    }

    return tmp_node;
}


void vl_insert(VoterList *VL, RedBlackNode *voter) {
    VoterNode *new_voter_node = create_voter_node(voter);

    if (VL->head == NULL) {
        VL->head = new_voter_node;
    } else {
        new_voter_node->next = VL->head;
        VL->head = new_voter_node;
    }

    (VL->count)++;
}


void vl_remove(VoterList *VL, char key[9]) {
    VoterNode *node_to_delete = VL->head;
    VoterNode *previous_node = NULL;

    if (node_to_delete == NULL) return;

    while (strcmp(node_to_delete->voter->key, key) != 0) {
        previous_node = node_to_delete;
        node_to_delete = node_to_delete->next;
    }

    if (previous_node == NULL) {        // Node to be deleted is the head
        VL->head = node_to_delete->next;;
    } else {                            // Node to be deleted is NOT the head
        previous_node->next = node_to_delete->next;
    }
    free(node_to_delete);

    (VL->count)--;
}


void vl_print(VoterList VL) {
    VoterNode *tmp_node = VL.head;

    printf("\t\tVoterList:\n");
    printf("\t\t\tCount: = %d\n", VL.count);
    printf("\t\t\t");
    while(tmp_node != NULL) {
        printf("%s -> ", tmp_node->voter->key);
        tmp_node = tmp_node->next;
    }
    printf("\n\n");
}


void vl_free(VoterList VL) {
    VoterNode *current_node = VL.head;
    VoterNode *next_node;

    while (current_node != NULL) {
        next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
}
