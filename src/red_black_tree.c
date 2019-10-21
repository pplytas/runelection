#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "red_black_tree.h"
#include "utilities.h"

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
    check_errors(new_node, "malloc", 1);

    new_node->key = (char *) malloc((strlen(key) + 1) * sizeof(char));
    check_errors(new_node->key, "malloc", 1);
    strcpy(new_node->key, key);

    new_node->firstname = (char *) malloc((strlen(firstname) + 1) * sizeof(char));
    check_errors(new_node->firstname, "malloc", 1);
    strcpy(new_node->firstname, firstname);

    new_node->lastname = (char *) malloc((strlen(lastname) + 1) * sizeof(char));
    check_errors(new_node->lastname, "malloc", 1);
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


void rbt_insert_fix(RedBlackTree *RBT, RedBlackNode *new_node) {
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
    (RBT->count)++;

    rbt_insert_fix(RBT, new_node);                 // Ensure the Red-Black property is maintained

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


RedBlackNode *rbt_find_min_node(RedBlackNode *node){
    while(node->left != NULL){
        node = node->left;
    }
    return node;
}


void rbt_copy_node(RedBlackNode *src_node, RedBlackNode *dest_node) {
    free(dest_node->key);
    dest_node->key = (char *) malloc((strlen(src_node->key) + 1) * sizeof(char));
    check_errors(dest_node->key, "malloc", 1);
    strcpy(dest_node->key, src_node->key);

    free(dest_node->firstname);
    dest_node->firstname = (char *) malloc((strlen(src_node->firstname) + 1) * sizeof(char));
    check_errors(dest_node->firstname, "malloc", 1);
    strcpy(dest_node->firstname, src_node->firstname);

    free(dest_node->lastname);
    dest_node->lastname = (char *) malloc((strlen(src_node->lastname) + 1) * sizeof(char));
    check_errors(dest_node->lastname, "malloc", 1);
    strcpy(dest_node->lastname, src_node->lastname);

    dest_node->age = src_node->age;
    dest_node->gender = src_node->gender;
    dest_node->postcode = src_node->postcode;
    dest_node->has_voted = src_node->has_voted;
}


void rbt_delete_node(RedBlackTree *RBT, RedBlackNode *node_to_delete){
    RedBlackNode *successor_node, *child_node, *parent_node_to_delete;

    if (node_to_delete->has_voted == 1) (RBT->have_voted_count)--;
    (RBT->count)--;

    if (node_to_delete->left != NULL && node_to_delete->right != NULL) {
        successor_node = rbt_find_min_node(node_to_delete->right);
        rbt_copy_node(successor_node, node_to_delete);
        node_to_delete = successor_node;    // Node to delete now becomes the successor which is either leaf or has one child
    }

    if (node_to_delete->left != NULL) {
        child_node = node_to_delete->left;
    } else if (node_to_delete->right != NULL) {
        child_node = node_to_delete->right;
    } else {
        child_node = node_to_delete->left;
    }

    if ((node_to_delete->color == RED) || (get_color(child_node) == RED)) {   // if node_to_delete or its child is RED
        rbt_replace_node(RBT, node_to_delete, child_node);
        if (child_node != NULL) {
            child_node->color = BLACK;
        }
    } else {
        parent_node_to_delete = node_to_delete->parent;
        rbt_replace_node(RBT, node_to_delete, child_node);
        rbt_delete_fix(RBT, child_node, parent_node_to_delete);
    }

    rbt_free_node(node_to_delete);
}


int is_left_child(RedBlackNode *node) {
    return node == node->parent->left;
}


int is_right_child(RedBlackNode *node) {
    return node == node->parent->right;
}


char get_color(RedBlackNode *node) {
    return ((node == NULL) || (node->color == BLACK)) ? BLACK : RED;
}


void rbt_delete_fix(RedBlackTree *RBT, RedBlackNode *node, RedBlackNode *parent_node) {
    if (node == RBT->root) return;

    printf("parent_node 1: %s\n", parent_node->key);

    RedBlackNode *sibling_node = (node == parent_node->left) ? parent_node->right : parent_node->left;
    printf("sibling_node 2 key: %s %c\n", sibling_node->key, sibling_node->color);

    if (sibling_node == NULL) {
        rbt_delete_fix(RBT, parent_node, parent_node->parent);
    } else {
        if (get_color(sibling_node) == BLACK) {   // sibling is BLACK
            if (get_color(sibling_node->left) == RED || get_color(sibling_node->right) == RED) {   // at least one child is RED
                if (is_left_child(sibling_node) && get_color(sibling_node->left) == RED) {
                    sibling_node->left->color = sibling_node->color;
                    sibling_node->color = parent_node->color;
                    rbt_rotate_right(RBT, parent_node);
                } else if (is_left_child(sibling_node) && get_color(sibling_node->right) == RED) {
                    sibling_node->right->color = parent_node->color;
                    rbt_rotate_left(RBT, sibling_node);
                    rbt_rotate_right(RBT, parent_node);
                } else if (is_right_child(sibling_node) && get_color(sibling_node->right) == RED) {
                    sibling_node->right->color = sibling_node->color;
                    sibling_node->color = parent_node->color;
                    rbt_rotate_left(RBT, parent_node);
                } else {
                    sibling_node->left->color = parent_node->color;
                    rbt_rotate_right(RBT, sibling_node);
                    rbt_rotate_left(RBT, parent_node);
                }
                parent_node->color = BLACK;
            } else {     // both children are black
                printf("sibling_node 2 key: %s\n", sibling_node->key);
                printf("parent key 2: %s\n", parent_node->key);
                sibling_node->color = RED;
                if (parent_node->color == BLACK) {
                    rbt_delete_fix(RBT, parent_node, parent_node->parent);
                } else {
                    parent_node->color = BLACK;
                }
            }
        } else {    // sibling is RED
            parent_node->color = RED;
            sibling_node->color = BLACK;
            if (is_left_child(sibling_node)) {
                rbt_rotate_right(RBT, parent_node);
            } else {
                rbt_rotate_left(RBT, parent_node);
            }
            rbt_print(*RBT);
            printf("333333\n");
            printf("Parent key: %s\n", parent_node->key);
            rbt_delete_fix(RBT, node, parent_node);
        }
    }
}


void rbt_replace_node(RedBlackTree *RBT, RedBlackNode *old_node, RedBlackNode *new_node){
    if(old_node->parent == NULL){
        RBT->root = new_node;
    }
    else if(old_node == old_node->parent->left){
        old_node->parent->left = new_node;
    }
    else{
        old_node->parent->right = new_node;
    }

    if (new_node != NULL) {
        new_node->parent = old_node->parent;
    }
}


void rbt_print_node(RedBlackNode *node) {
    if ((node != NULL) && ((node->left != NULL || node->right != NULL) || (node->parent == NULL))) {
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
