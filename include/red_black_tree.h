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

Node* create_node(char *);

void rbt_rotate_left(Node **, Node *);
void rbt_rotate_right(Node **, Node *);
void rbt_check_fix(Node **, Node *);

void rbt_insert(Node **, char *);

int rbt_height(Node *);
int rbt_black_height(Node *);
void rbt_print(Node *);

void rbt_free(Node *);

#endif