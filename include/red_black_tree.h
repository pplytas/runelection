#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

typedef struct Node Node;

struct Node {
    char key[9];
    char color;
    Node *left;
    Node *right;
    Node *parent;
};

typedef struct RedBlackTree {
    int count;
    Node *root;
} RedBlackTree;

Node* create_node(char *);
void rbt_rotate_left(RedBlackTree *, Node *);
void rbt_rotate_right(RedBlackTree *, Node *);
void rbt_check_fix(RedBlackTree *, Node *);
void rbt_insert(RedBlackTree *, char *);
void rbt_print_node(Node *);
void rbt_print(RedBlackTree);
void rbt_free_node(Node *);
void rbt_free(RedBlackTree);

#endif