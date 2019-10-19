#include <stdio.h>
#include <stdlib.h>

#include "stack.h"


void stack_init(Stack *S) {
    S->head = NULL;
}


int stack_is_empty(Stack S) {
    return S.head == NULL;
}


void stack_push(Stack *S, RedBlackNode *new_rbt_node) {
    StackNode *new_node = (StackNode *) malloc(sizeof(StackNode));
    check_errors(new_node, "malloc", 1);

    new_node->next = S->head;
    new_node->rbt_node = new_rbt_node;
    S->head = new_node;
}


RedBlackNode* stack_pop(Stack *S) {
    StackNode *tmp_node;
    RedBlackNode *rbt_node;

    if (stack_is_empty(*S)) {
        printf("attempt to pop the empty stack");
        return NULL;
    }

    tmp_node = S->head;
    S->head = tmp_node->next;
    rbt_node = tmp_node->rbt_node;
    free(tmp_node);

    return rbt_node;
}
