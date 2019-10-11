#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

typedef struct RedBlackNode RedBlackNode;

struct RedBlackNode {
    char key[9];
    char color;
    RedBlackNode *left;
    RedBlackNode *right;
    RedBlackNode *parent;
};

typedef struct RedBlackTree {
    RedBlackNode *root;
    int count;
} RedBlackTree;

void rbt_init(RedBlackTree *);
RedBlackNode* create_red_black_node(char *);
void rbt_rotate_left(RedBlackTree *, RedBlackNode *);
void rbt_rotate_right(RedBlackTree *, RedBlackNode *);
void rbt_check_fix(RedBlackTree *, RedBlackNode *);
RedBlackNode* rbt_insert(RedBlackTree *, char *);
void rbt_print_node(RedBlackNode *);
void rbt_print(RedBlackTree);
void rbt_free_node(RedBlackNode *);
void rbt_free(RedBlackTree);

#endif