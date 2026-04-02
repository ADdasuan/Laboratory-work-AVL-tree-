#include "balance.h"
#include "stack.h"
#include <string.h>

void update_height(Node_Tree* node) {
    if (!node) return;
    int left_height = node->left ? node->left->height : 0;
    int right_height = node->right ? node->right->height : 0;
    node->height = 1 + (left_height > right_height ? left_height : right_height);
}

int balance_check(Node_Tree* node) {
    if (!node) return 0;
    int left_height = node->left ? node->left->height : 0;
    int right_height = node->right ? node->right->height : 0;
    return left_height - right_height;
}

static Node_Tree* rotate_right(Node_Tree* y) {
    if (!y || !y->left) return y;

    Node_Tree* x = y->left;
    y->left = x->right;
    x->right = y;

    update_height(y);
    update_height(x);

    return x;
}

static Node_Tree* rotate_left(Node_Tree* x) {
    if (!x || !x->right) return x;

    Node_Tree* y = x->right;
    x->right = y->left;
    y->left = x;

    update_height(x);
    update_height(y);

    return y;
}

Node_Tree* balance_node(Node_Tree* node) {
    if (!node) return NULL;

    int balance = balance_check(node);

    if (balance > 1 && balance_check(node->left) >= 0)
        return rotate_right(node);

    if (balance > 1 && balance_check(node->left) < 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    if (balance < -1 && balance_check(node->right) <= 0)
        return rotate_left(node);

    if (balance < -1 && balance_check(node->right) > 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

void balance_path(Tree_AVL* tree, Node_Tree* start_node) {
    if (!tree || !start_node) return;

    Stack* path_stack = create_stack();
    if (!path_stack) return;

    Node_Tree* current = tree->root;
    while (current && current != start_node) {
        stack_push(path_stack, current);
        int cmp = strcmp(start_node->key, current->key);
        current = (cmp < 0) ? current->left : current->right;
    }

    if (current == start_node) {
        stack_push(path_stack, current);
    }

 
    while (!stack_is_empty(path_stack)) {
        Node_Tree* node = stack_pop(path_stack);
        Node_Tree* balanced = balance_node(node);

        if (!stack_is_empty(path_stack)) {
            Node_Tree* parent = stack_pop(path_stack);
            if (parent->left == node) {
                parent->left = balanced;
            } else {
                parent->right = balanced;
            }
            stack_push(path_stack, parent);
        } else {
            tree->root = balanced;
        }
    }

    free_stack(path_stack);
}

void balance_AVL_Tree(Tree_AVL* tree) {
    if (!tree || !tree->root) return;
    tree->root = balance_node(tree->root);
}

void balance_path_from_node(Tree_AVL* tree, Node_Tree* start_node) {
    if (!tree || !start_node) return;

    Stack* path_stack = create_stack();
    if (path_stack == NULL) return;


    Node_Tree* node_current = tree->root;
    while (node_current && node_current != start_node) {
        stack_push(path_stack, node_current);
        int cmp = strcmp(start_node->key, node_current->key);
        node_current = (cmp < 0) ? node_current->left : node_current->right;
    }
    
    if (node_current == start_node) {
        stack_push(path_stack, node_current);
    }

    while (!stack_is_empty(path_stack)) {
        Node_Tree* node = stack_pop(path_stack);
        update_height(node);
        
        int balance = balance_check(node);
        if (balance > 1 || balance < -1) {
            Node_Tree* balanced_tree = balance_node(node);
            
            if (!stack_is_empty(path_stack)) {
                Node_Tree* node_parent = stack_pop(path_stack);
                if (node_parent->left == node) {
                    node_parent->left = balanced_tree;
                } else {
                    node_parent->right = balanced_tree;
                }
                stack_push(path_stack, node_parent);
            } else {
                tree->root = balanced_tree;
            }
        }
    }

    free_stack(path_stack);
}
