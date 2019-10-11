#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <red_black_tree.h>

#define RED 'R'
#define BLACK 'B'
#define MAX(a,b) (((a)>(b))?(a):(b))


Node* create_node(char *key) {
    Node *new_node = (Node*) malloc(sizeof(Node));

    strcpy(new_node->key, key);
    new_node->color = RED;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;

    return new_node;
}


void rbt_rotate_left(Node **root_ptr, Node *pivot_node) {
    Node *pivot_right_child = pivot_node->right;    // set pivot_right_child
    pivot_node->right = pivot_right_child->left;     // turn pivot_right_child's left subtree into pivot_node's right subtree{
    if (pivot_right_child->left != NULL) {
        pivot_right_child->left->parent = pivot_node;
    }
    pivot_right_child->parent = pivot_node->parent;  // link pivot_node's parent to pivot_right_child
    if (pivot_node->parent == NULL) {
       *root_ptr = pivot_right_child;
    } else if (pivot_node == pivot_node->parent->left) {
       pivot_node->parent->left = pivot_right_child;
    } else {
       pivot_node->parent->right = pivot_right_child;
    }
    pivot_right_child->left = pivot_node;            // put pivot_node on pivot_right_child's left
    pivot_node->parent = pivot_right_child;
}


void rbt_rotate_right(Node **root_ptr, Node *pivot_node) {
    Node *pivot_left_child = pivot_node->left;     // set pivot_left_child
    pivot_node->left = pivot_left_child->right;    // turn pivot_left_child's right subtree into pivot_node's left subtree
    if (pivot_left_child->right != NULL) {
        pivot_left_child->right->parent = pivot_node;
    }
    pivot_left_child->parent = pivot_node->parent;  // link pivot_node's parent to pivot_left_child
    if (pivot_node->parent == NULL) {
       *root_ptr = pivot_left_child;
    } else if (pivot_node == pivot_node->parent->right) {
       pivot_node->parent->right = pivot_left_child;
    } else {
       pivot_node->parent->left = pivot_left_child;
    }
    pivot_left_child->right = pivot_node;         // put pivot_node on pivot_left_child's right
    pivot_node->parent = pivot_left_child;
}


void rbt_check_fix(Node **root_ptr, Node *new_node) {
    Node *temp_node;

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
                    rbt_rotate_left(root_ptr, new_node);
                }
                new_node->parent->color = BLACK;
                new_node->parent->parent->color = RED;
                rbt_rotate_right(root_ptr, new_node->parent->parent);
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
                    rbt_rotate_right(root_ptr, new_node);
                }
                new_node->parent->color = BLACK;
                new_node->parent->parent->color = RED;
                rbt_rotate_left(root_ptr, new_node->parent->parent);
            }
        }
    }

    (*root_ptr)->color = BLACK;
}


void rbt_insert(Node **root_ptr, char *key) {
    Node *parent_node, *tmp_node, *new_node;

    // Find where to insert new key
    tmp_node = *root_ptr;
    parent_node = NULL;
    while (tmp_node != NULL) {
        parent_node = tmp_node;
        if (strcmp(key, tmp_node->key) < 0) {
            tmp_node = tmp_node->left;
        } else {
            tmp_node = tmp_node->right;
        }
    }

    // Create new node with given key
    new_node = create_node(key);

    // Insert new node in tree
    if (parent_node != NULL) {
        new_node->parent = parent_node;

        if (strcmp(new_node->key, parent_node->key) < 0) {
            parent_node->left = new_node;
        } else {
            parent_node->right = new_node;
        }
    } else {
       *root_ptr = new_node;        // Set root node
    }

    // Ensure the Red-Black property is maintained
    rbt_check_fix(root_ptr, new_node);
}


int rbt_height(Node *node) {
    int height = 0;
    int left_height, right_height;

    if (node != NULL) {
        if (node->left == NULL && node->right == NULL){
            height = 1;
        } else {
            left_height = rbt_height(node->left);
            right_height = rbt_height(node->right);
            height = MAX(left_height, right_height) + 1;
        }
    }

    return height;
}


int rbt_black_height(Node *node) {
    int height = 0;

    while (node != NULL) {
        if (node->color == BLACK) {
           height++;
        }
        node = node->left;
    }

    return height;
}


void rbt_print(Node *node) {
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

        rbt_print(node->left);
        rbt_print(node->right);
    }
}

void rbt_free(Node *node) {
    if (node == NULL) return;

    rbt_free(node->left);
    rbt_free(node->right);

    free(node);
}
