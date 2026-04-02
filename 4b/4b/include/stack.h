#ifndef STACK
#define STACK

#include "struct_private.h"
typedef struct StackNode {
    Node_Tree* tree_node;
    struct StackNode* next;
} StackNode;

typedef struct {
    StackNode* top;
} Stack;

Stack* create_stack();

int stack_push(Stack* stack, Node_Tree* tree_node);

Node_Tree* stack_pop(Stack* stack);

int stack_is_empty(Stack* stack);

void free_stack(Stack* stack);

#endif
