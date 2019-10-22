#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

typedef struct RedBlackNode RedBlackNode;

struct RedBlackNode {
    char *key;
    char *firstname;
    char *lastname;
    int age;
    char gender;
    int postcode;
    int has_voted;

    char color;
    RedBlackNode *left;
    RedBlackNode *right;
    RedBlackNode *parent;
};

typedef struct RedBlackTree {
    RedBlackNode *root;
    int count;
    int have_voted_count;
} RedBlackTree;

void rbt_init(RedBlackTree *);
RedBlackNode* create_red_black_node(char *, char *, char *, int, char, int, int);
void rbt_rotate_left(RedBlackTree *, RedBlackNode *);
void rbt_rotate_right(RedBlackTree *, RedBlackNode *);
void rbt_insert_fix(RedBlackTree *, RedBlackNode *);
RedBlackNode* rbt_find_node_by_key(RedBlackTree, char *);
void rbt_insert_child_node(RedBlackTree *, RedBlackNode *, RedBlackNode *);
RedBlackNode* rbt_insert(RedBlackTree *, char *, char *, char *, int, char, int);
int rbt_update_node_has_voted(RedBlackNode *, int);
int rbt_update_has_voted(RedBlackTree *, RedBlackNode *, int);
void rbt_copy_node(RedBlackNode *, RedBlackNode *);
void rbt_transplant_node(RedBlackTree *, RedBlackNode *, RedBlackNode *);
void rbt_delete_node(RedBlackTree *, RedBlackNode *);
int is_left_child(RedBlackNode *);
int is_right_child(RedBlackNode *);
char get_color(RedBlackNode *);
void rbt_delete_fix(RedBlackTree *, RedBlackNode *, RedBlackNode *);
void rbt_replace_node(RedBlackTree *, RedBlackNode *, RedBlackNode *);
void rbt_print_node(RedBlackNode *);
void rbt_print(RedBlackTree);
void rbt_free_node(RedBlackNode *);
void rbt_free(RedBlackTree);

#endif