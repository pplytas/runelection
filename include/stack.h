#ifndef STACK_H
#define STACK_H

#include "red_black_tree.h"
#include "utilities.h"


typedef struct StackNode StackNode;

struct StackNode {
    RedBlackNode *rbt_node;
    StackNode *next;
};

typedef struct {
    StackNode *head;
} Stack;

void stack_init(Stack *);
int stack_is_empty(Stack);
void stack_push(Stack *, RedBlackNode *);
RedBlackNode* stack_pop(Stack *);

#endif
