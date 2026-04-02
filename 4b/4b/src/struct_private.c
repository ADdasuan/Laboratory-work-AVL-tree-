#include <stdio.h>
#include <stddef.h>
#include "input_check.h"
#include "macros_for_return.h"
#include "struct_private.h"
#include <stdlib.h>
#include <string.h>


Node_Spisok* create_list_node(InfoType info) {
    Node_Spisok* node = (Node_Spisok*)calloc(1,sizeof(Node_Spisok));
    if (node) {
        node->info = info;
        node->next = NULL;
    }
    return node;
}


int spisok_add_el(Spisok* spisok, InfoType info) {
    if (!spisok) return FUNC_UNLUCK;
    
    Node_Spisok* node_new = create_list_node(info);
    if (!node_new) return FUNC_MEMORY_ERROR;
    
    node_new->next = spisok->head;
    spisok->head = node_new;
    return FUNC_SUCCESS;
}


int spisok_del_el(Spisok* spisok, int index) {
    if (!spisok || index < 0 || !spisok->head) return FUNC_UNLUCK;
    
    Node_Spisok *prev = NULL;
    Node_Spisok *current = spisok->head;
    int count = 0;
    
    while (current && count < index) {
        prev = current;
        current = current->next;
        count++;
    }
    
    if (!current) return FUNC_UNLUCK;
    
    if (prev) {
        prev->next = current->next;
    } else {
        spisok->head = current->next;
    }
    
    free(current);
    return FUNC_SUCCESS;
}

void free_spisok(Spisok* spisok) {
    if (!spisok) return;
    
    Node_Spisok* current = spisok->head;
    while (current) {
        Node_Spisok* next = current->next;
        free(current);
        current = next;
    }
    free(spisok);
}

Node_Tree* create_tree_node(KeyType key, InfoType info) {
    if (!key) return NULL;

    Node_Tree* node = calloc(1, sizeof(Node_Tree));
    if (!node) return NULL;

    node->key = strdup(key); 
    if (!node->key) {
        free(node);  
        return NULL;
    }

    node->duplicates = calloc(1, sizeof(Spisok));
    if (!node->duplicates) {
        free(node->key);  
        free(node);
        return NULL;
    }

    if (spisok_add_el(node->duplicates, info) != FUNC_SUCCESS) {
        free(node->duplicates);  
        free(node->key);
        free(node);
        return NULL;
    }

    return node;
}

Tree_AVL* create_tree_AVL() {
    Tree_AVL* tree = (Tree_AVL*)calloc(1, sizeof(Tree_AVL)); 
    if (tree) {
        tree->root = NULL;
    }
    return tree;
}

static void free_node_tree(Node_Tree* node) {
    if (!node) return;
    
    free_node_tree(node->left);
    free_node_tree(node->right);
    
    if (node->duplicates) {
        Node_Spisok* current = node->duplicates->head;
        while (current) {
            Node_Spisok* next = current->next;
            free(current);
            current = next;
        }
        free(node->duplicates);
    }
    
    free(node->key);
    free(node);
}

void free_tree_AVL(Tree_AVL* tree) {
    if (!tree) return;
    
    free_node_tree(tree->root);
    free(tree);
}

void dot_export_AVL_recursive(FILE* f, Node_Tree* node) {
    if (!node) return;
    fprintf(f, "  \"%p\" [label=\"%s\\nheight: %d", (void*)node, node->key, node->height);
    
    Node_Spisok* current = node->duplicates->head;
    while (current) {
        fprintf(f, "\\ninfo: %u", current->info);
        current = current->next;
    }
    fprintf(f, "\"];\n");

    if (node->left) {
        fprintf(f, "  \"%p\" -> \"%p\" [label=\"L\"];\n", (void*)node, (void*)node->left);
        dot_export_AVL_recursive(f, node->left);
    }

    if (node->right) {
        fprintf(f, "  \"%p\" -> \"%p\" [label=\"R\"];\n", (void*)node, (void*)node->right);
        dot_export_AVL_recursive(f, node->right);
    }
}

Node_Tree* internal_search(Tree_AVL* tree, KeyType key, int seq_number, Node_Tree** parent_ptr) {
    if (!tree || !tree->root) return NULL;

    Node_Tree* current = tree->root;
    Node_Tree* parent = NULL;
    int count = 0;

    while (current) {
        int cmp = strcmp(key, current->key);
        
        if (cmp < 0) {
            parent = current;
            current = current->left;
        } else if (cmp > 0) {
            parent = current;
            current = current->right;
        } else {
            if (count == seq_number) {
                if (parent_ptr) *parent_ptr = parent;
                return current;
            }
            count++;
            parent = current;
            current = current->right;
        }
    }

    return NULL;
}#include <stdio.h>
#include <stddef.h>
#include "input_check.h"
#include "macros_for_return.h"
#include "struct_private.h"
#include <stdlib.h>
#include <string.h>


Node_Spisok* create_list_node(InfoType info) {
    Node_Spisok* node = (Node_Spisok*)calloc(1,sizeof(Node_Spisok));
    if (node) {
        node->info = info;
        node->next = NULL;
    }
    return node;
}


int spisok_add_el(Spisok* spisok, InfoType info) {
    if (!spisok) return FUNC_UNLUCK;
    
    Node_Spisok* node_new = create_list_node(info);
    if (!node_new) return FUNC_MEMORY_ERROR;
    
    node_new->next = spisok->head;
    spisok->head = node_new;
    return FUNC_SUCCESS;
}


int spisok_del_el(Spisok* spisok, int index) {
    if (!spisok || index < 0 || !spisok->head) return FUNC_UNLUCK;
    
    Node_Spisok *prev = NULL;
    Node_Spisok *current = spisok->head;
    int count = 0;
    
    while (current && count < index) {
        prev = current;
        current = current->next;
        count++;
    }
    
    if (!current) return FUNC_UNLUCK;
    
    if (prev) {
        prev->next = current->next;
    } else {
        spisok->head = current->next;
    }
    
    free(current);
    return FUNC_SUCCESS;
}

void free_spisok(Spisok* spisok) {
    if (!spisok) return;
    
    Node_Spisok* current = spisok->head;
    while (current) {
        Node_Spisok* next = current->next;
        free(current);
        current = next;
    }
    free(spisok);
}

Node_Tree* create_tree_node(KeyType key, InfoType info) {
    if (!key) return NULL;

    Node_Tree* node = calloc(1, sizeof(Node_Tree));
    if (!node) return NULL;

    node->key = strdup(key); 
    if (!node->key) {
        free(node);  
        return NULL;
    }

    node->duplicates = calloc(1, sizeof(Spisok));
    if (!node->duplicates) {
        free(node->key);  
        free(node);
        return NULL;
    }

    if (spisok_add_el(node->duplicates, info) != FUNC_SUCCESS) {
        free(node->duplicates);  
        free(node->key);
        free(node);
        return NULL;
    }

    return node;
}

Tree_AVL* create_tree_AVL() {
    Tree_AVL* tree = (Tree_AVL*)calloc(1, sizeof(Tree_AVL)); 
    if (tree) {
        tree->root = NULL;
    }
    return tree;
}

static void free_node_tree(Node_Tree* node) {
    if (!node) return;
    
    free_node_tree(node->left);
    free_node_tree(node->right);
    
    if (node->duplicates) {
        Node_Spisok* current = node->duplicates->head;
        while (current) {
            Node_Spisok* next = current->next;
            free(current);
            current = next;
        }
        free(node->duplicates);
    }
    
    free(node->key);
    free(node);
}

void free_tree_AVL(Tree_AVL* tree) {
    if (!tree) return;
    
    free_node_tree(tree->root);
    free(tree);
}

void dot_export_AVL_recursive(FILE* f, Node_Tree* node) {
    if (!node) return;
    fprintf(f, "  \"%p\" [label=\"%s\\nheight: %d", (void*)node, node->key, node->height);
    
    Node_Spisok* current = node->duplicates->head;
    while (current) {
        fprintf(f, "\\ninfo: %u", current->info);
        current = current->next;
    }
    fprintf(f, "\"];\n");

    if (node->left) {
        fprintf(f, "  \"%p\" -> \"%p\" [label=\"L\"];\n", (void*)node, (void*)node->left);
        dot_export_AVL_recursive(f, node->left);
    }

    if (node->right) {
        fprintf(f, "  \"%p\" -> \"%p\" [label=\"R\"];\n", (void*)node, (void*)node->right);
        dot_export_AVL_recursive(f, node->right);
    }
}

Node_Tree* internal_search(Tree_AVL* tree, KeyType key, int seq_number, Node_Tree** parent_ptr) {
    if (!tree || !tree->root) return NULL;

    Node_Tree* current = tree->root;
    Node_Tree* parent = NULL;
    int count = 0;

    while (current) {
        int cmp = strcmp(key, current->key);
        
        if (cmp < 0) {
            parent = current;
            current = current->left;
        } else if (cmp > 0) {
            parent = current;
            current = current->right;
        } else {
            if (count == seq_number) {
                if (parent_ptr) *parent_ptr = parent;
                return current;
            }
            count++;
            parent = current;
            current = current->right;
        }
    }

    return NULL;
}
