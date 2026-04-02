#ifndef STRUCT_PRIVATE
#define STRUCT_PRIVATE

#include <stdio.h>
#include <stdlib.h>

typedef char* KeyType;
typedef unsigned int InfoType;

typedef struct Node_Spisok {
    InfoType info;
    struct Node_Spisok* next;
} Node_Spisok;

typedef struct Spisok {
    Node_Spisok* head;
} Spisok;

typedef struct Node_Tree {
    KeyType key;
    Spisok* duplicates;
    struct Node_Tree* left;
    struct Node_Tree* right;
    int height;
} Node_Tree;

typedef struct Tree_AVL {
    Node_Tree* root;
} Tree_AVL;

Node_Spisok* create_list_node(InfoType info);

int spisok_add_el(Spisok* spisok, InfoType info);

int spisok_del_el(Spisok* spisok, int index);

void free_spisok(Spisok* spisok);

Node_Tree* create_tree_node(KeyType key, InfoType info);

Tree_AVL* create_tree_AVL();

void free_tree_AVL(Tree_AVL* tree);

void dot_export_AVL_recursive(FILE* f, Node_Tree* node);

Node_Tree* internal_search(Tree_AVL* tree, KeyType key, int seq_number, Node_Tree** parent_ptr);

#endif
