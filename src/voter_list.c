#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "voter_list.h"
#include "utilities.h"


void vl_init(VoterList *VL) {
    VL->head = NULL;
    VL->voters_count = 0;
    VL->have_voted_count = 0;
}


VoterNode* create_voter_node(RedBlackNode *voter) {
    VoterNode *new_voter_node = (VoterNode *) malloc(sizeof(VoterNode));
    check_errors(new_voter_node, "malloc", 1);
    new_voter_node->voter = voter;
    new_voter_node->next = NULL;
    return new_voter_node;
}


VoterNode* vl_find_node_by_key(VoterList VL, char *key) {
    VoterNode *found_node = VL.head;

    while (found_node != NULL && strcmp(found_node->voter->key, key) != 0) {
        found_node = found_node->next;
    }

    return found_node;
}


void vl_insert(VoterList *VL, RedBlackNode *voter) {
    VoterNode *new_voter_node = create_voter_node(voter);

    if (VL->head == NULL) {
        VL->head = new_voter_node;
    } else {
        new_voter_node->next = VL->head;
        VL->head = new_voter_node;
    }

    (VL->voters_count)++;
}


void vl_increase_have_voted_count(VoterList *VL, int amount) {
    VL->have_voted_count += amount;
}


void vl_remove(VoterList *VL, char *key) {
    VoterNode *node_to_delete = VL->head;
    VoterNode *previous_node = NULL;

    if (node_to_delete == NULL) return;

    while (strcmp(node_to_delete->voter->key, key) != 0) {
        previous_node = node_to_delete;
        node_to_delete = node_to_delete->next;
    }

    if (node_to_delete->voter->has_voted == 1) {
        (VL->have_voted_count)--;
    }
    (VL->voters_count)--;

    if (previous_node == NULL) {        // Node to be deleted is the head
        VL->head = node_to_delete->next;;
    } else {                            // Node to be deleted is NOT the head
        previous_node->next = node_to_delete->next;
    }
    free(node_to_delete);
}


void vl_print(VoterList VL) {
    VoterNode *tmp_node = VL.head;

    printf("\t\tVoterList:\n");
    printf("\t\t\tVoters Count: = %d\n", VL.voters_count);
    printf("\t\t\tHave Voted Count: = %d\n", VL.have_voted_count);
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
