#include "stack.h"
#include <stdlib.h>

Stack* create_stack() {
    Stack* stack = malloc(sizeof(Stack));
    if (!stack) return NULL;
    stack->top = NULL;
    return stack;
}

void free_stack(Stack* stack) {
    if (!stack) return;

    StackNode* current = stack->top;
    while (current) {
        StackNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(stack);
}

int stack_push(Stack* stack, Node_Tree* item) {
    if (!stack) return 0;

    StackNode* new_node = malloc(sizeof(StackNode));
    if (!new_node) return 0;

    new_node->tree_node = item;
    new_node->next = stack->top;
    stack->top = new_node;
    return 1;
}

Node_Tree* stack_pop(Stack* stack) {
    if (!stack || !stack->top) return NULL;

    StackNode* top_node = stack->top;
    Node_Tree* item = top_node->tree_node;
    stack->top = top_node->next;
    free(top_node);
    return item;
}

int stack_is_empty(Stack* stack) {
    return stack == NULL || stack->top == NULL;
}
