#ifndef STRUCT_PUBLIC
#define STRUCT_PUBLIC
#include "struct_private.h"

void menu();

Err_Dialog block_add(Tree_AVL* tree);

Err_Dialog block_del(Tree_AVL* tree);

Err_Dialog block_traversing_AVL_Tree(Tree_AVL* tree);

Err_Dialog block_search_to_key(Tree_AVL* tree);

Err_Dialog block_special_search(Tree_AVL* tree);

Err_Dialog block_output_tree(Tree_AVL* tree);

Err_Dialog block_graphical_output(Tree_AVL* tree);

Err_Dialog block_import_to_txt(Tree_AVL* tree);

Err_Dialog block_timing();

#endif
