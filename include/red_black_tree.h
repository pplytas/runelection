#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

typedef struct RedBlackNode RedBlackNode;

struct RedBlackNode {
    char key[9];
    char firstname[12];
    char lastname[12];
    int age;
    char gender;
    char postcode[6];

    int has_voted;

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
RedBlackNode* create_red_black_node(char[9], char[12], char[12], int, char, char[6]);
void rbt_rotate_left(RedBlackTree *, RedBlackNode *);
void rbt_rotate_right(RedBlackTree *, RedBlackNode *);
void rbt_check_fix(RedBlackTree *, RedBlackNode *);
RedBlackNode* rbt_find_node_by_key(RedBlackTree, char[9]);
RedBlackNode* rbt_insert(RedBlackTree *, char[9], char[12], char[12], int, char, char[6]);
int rbt_update_node_has_voted(RedBlackNode *, int);
int rbt_update_has_voted(RedBlackTree, char[9], int);
void rbt_print_node(RedBlackNode *);
void rbt_print(RedBlackTree);
void rbt_free_node(RedBlackNode *);
void rbt_free(RedBlackTree);

#endif