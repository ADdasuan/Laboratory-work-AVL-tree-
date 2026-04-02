#ifndef BALANCE
#define BALANCE
#include "struct_private.h"

void update_height(Node_Tree* node);

int balance_check(Node_Tree* node);

Node_Tree* right_rotation(Node_Tree* y);

Node_Tree* left_rotation(Node_Tree* x);

Node_Tree* balance_node(Node_Tree* node);

void balance_AVL_Tree(Tree_AVL* tree);

void balance_path_from_node(Tree_AVL* tree, Node_Tree* start_node);

#endif
