#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "red_black_tree.h"

#define RED 'R'
#define BLACK 'B'
#define MAX(a,b) (((a)>(b))?(a):(b))


void rbt_init(RedBlackTree *RBT) {
    RBT->root = NULL;
    RBT->count = 0;
    RBT->have_voted_count = 0;
}


RedBlackNode* create_red_black_node(char *key, char *firstname, char *lastname, int age, char gender, int postcode) {
    RedBlackNode *new_node = (RedBlackNode*) malloc(sizeof(RedBlackNode));

    new_node->key = (char *) malloc((strlen(key) + 1) * sizeof(char));
    strcpy(new_node->key, key);

    new_node->firstname = (char *) malloc((strlen(firstname) + 1) * sizeof(char));
    strcpy(new_node->firstname, firstname);

    new_node->lastname = (char *) malloc((strlen(lastname) + 1) * sizeof(char));
    strcpy(new_node->lastname, lastname);

    new_node->age = age;
    new_node->gender = gender;
    new_node->postcode = postcode;
    new_node->has_voted = 0;

    new_node->color = RED;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;

    return new_node;
}


void rbt_rotate_left(RedBlackTree *RBT, RedBlackNode *pivot_node) {
    RedBlackNode *pivot_right_child = pivot_node->right;    // set pivot_right_child
    pivot_node->right = pivot_right_child->left;     // turn pivot_right_child's left subtree into pivot_node's right subtree{
    if (pivot_right_child->left != NULL) {
        pivot_right_child->left->parent = pivot_node;
    }
    pivot_right_child->parent = pivot_node->parent;  // link pivot_node's parent to pivot_right_child
    if (pivot_node->parent == NULL) {
       RBT->root = pivot_right_child;
    } else if (pivot_node == pivot_node->parent->left) {
       pivot_node->parent->left = pivot_right_child;
    } else {
       pivot_node->parent->right = pivot_right_child;
    }
    pivot_right_child->left = pivot_node;            // put pivot_node on pivot_right_child's left
    pivot_node->parent = pivot_right_child;
}


void rbt_rotate_right(RedBlackTree *RBT, RedBlackNode *pivot_node) {
    RedBlackNode *pivot_left_child = pivot_node->left;     // set pivot_left_child
    pivot_node->left = pivot_left_child->right;    // turn pivot_left_child's right subtree into pivot_node's left subtree
    if (pivot_left_child->right != NULL) {
        pivot_left_child->right->parent = pivot_node;
    }
    pivot_left_child->parent = pivot_node->parent;  // link pivot_node's parent to pivot_left_child
    if (pivot_node->parent == NULL) {
       RBT->root = pivot_left_child;
    } else if (pivot_node == pivot_node->parent->right) {
       pivot_node->parent->right = pivot_left_child;
    } else {
       pivot_node->parent->left = pivot_left_child;
    }
    pivot_left_child->right = pivot_node;         // put pivot_node on pivot_left_child's right
    pivot_node->parent = pivot_left_child;
}


void rbt_fix(RedBlackTree *RBT, RedBlackNode *new_node) {
    RedBlackNode *temp_node;

    while (new_node->parent != NULL && new_node->parent->color == RED) {
        if (new_node->parent == new_node->parent->parent->left) {   // Parent of new node is a left child itself
            temp_node = new_node->parent->parent->right;
            if (temp_node != NULL && temp_node->color == RED) {
                new_node->parent->color = BLACK;
                temp_node->color = BLACK;
                new_node->parent->parent->color = RED;
                new_node = new_node->parent->parent;
            } else {
                if (new_node == new_node->parent->right) {
                    new_node = new_node->parent;
                    rbt_rotate_left(RBT, new_node);
                }
                new_node->parent->color = BLACK;
                new_node->parent->parent->color = RED;
                rbt_rotate_right(RBT, new_node->parent->parent);
            }
        } else {                                                    // Parent of new node is a right child itself
            temp_node = new_node->parent->parent->left;
            if (temp_node != NULL && temp_node->color == RED) {
                new_node->parent->color = BLACK;
                temp_node->color = BLACK;
                new_node->parent->parent->color = RED;
                new_node = new_node->parent->parent;
            } else {
                if (new_node == new_node->parent->left) {
                    new_node = new_node->parent;
                    rbt_rotate_right(RBT, new_node);
                }
                new_node->parent->color = BLACK;
                new_node->parent->parent->color = RED;
                rbt_rotate_left(RBT, new_node->parent->parent);
            }
        }
    }

    RBT->root->color = BLACK;
}


RedBlackNode* rbt_find_node_by_key(RedBlackTree RBT, char *key) {
    RedBlackNode *found_node = RBT.root;

    while (found_node != NULL && strcmp(found_node->key, key) != 0) {
        if (strcmp(found_node->key, key) < 0) {
            found_node = found_node->right;
        } else {
            found_node = found_node->left;
        }
    }

    return found_node;
}


void rbt_insert_child_node(RedBlackTree *RBT, RedBlackNode *parent_node, RedBlackNode *child_node) {
    if (parent_node != NULL) {
        child_node->parent = parent_node;
        if (strcmp(child_node->key, parent_node->key) < 0) {
            parent_node->left = child_node;
        } else {
            parent_node->right = child_node;
        }
    } else {
       RBT->root = child_node;        // Set root node
    }
}


RedBlackNode* rbt_insert(RedBlackTree *RBT, char *key, char *lastname, char *firstname, int age, char gender, int postcode) {
    RedBlackNode *parent_node, *tmp_node, *new_node;

    // Find where to insert new key
    tmp_node = RBT->root;
    parent_node = NULL;
    while (tmp_node != NULL) {
        parent_node = tmp_node;
        if (strcmp(key, tmp_node->key) < 0) {
            tmp_node = tmp_node->left;      // key is smaller than currently checked node
        } else if (strcmp(key, tmp_node->key) > 0) {
            tmp_node = tmp_node->right;     // key is greater than currently checked node
        } else {
            return NULL;                    // key already exists in currently checked node
        }
    }

    new_node = create_red_black_node(key, firstname, lastname, age, gender, postcode);
    rbt_insert_child_node(RBT, parent_node, new_node);

    rbt_fix(RBT, new_node);                 // Ensure the Red-Black property is maintained

    return new_node;
}


int rbt_update_node_has_voted(RedBlackNode *node, int has_voted) {
    if (node->has_voted == has_voted) {
        return 0;       // Node already updated
    }

    node->has_voted = has_voted;
    return 1;       // Node updated successfully
}


int rbt_update_has_voted(RedBlackTree *RBT, RedBlackNode *node, int has_voted) {
    int updated;

    updated = rbt_update_node_has_voted(node, has_voted);
    if (updated) {
        (RBT->have_voted_count)++;
    }
    return updated;
}


void rbt_print_node(RedBlackNode *node) {
    if (node != NULL && (node->left != NULL || node->right != NULL)) {
        printf("        %s(%c)\n", node->key, (node->color == BLACK ? BLACK : RED));
        printf("       _____|_____\n");
        printf("      |           |\n");

        if (node->left != NULL) {
            printf(" %s(%c) ", node->left->key, (node->left->color == BLACK ? BLACK : RED));
        } else {
            printf("             ");
        }
        if (node->right != NULL) {
            printf("  %s(%c)", node->right->key, (node->right->color == BLACK ? BLACK : RED));
        }
        printf("\n\n");

        rbt_print_node(node->left);
        rbt_print_node(node->right);
    }
}


void rbt_print(RedBlackTree RBT) {
    rbt_print_node(RBT.root);
}


void rbt_free_node(RedBlackNode *node) {
    free(node->key);
    free(node->firstname);
    free(node->lastname);
    free(node);
}


void rbt_free(RedBlackTree RBT) {
    RedBlackNode *current_node = RBT.root, *next_node;

    while (current_node != NULL) {
        next_node = current_node->left;
        current_node->left = NULL;

        if (next_node == NULL) {
            if (current_node->right != NULL) {
                next_node = current_node->right;
                current_node->right = NULL;
            } else {
                next_node = current_node->parent;
                rbt_free_node(current_node);
            }
        }

        current_node = next_node;
    }
}
