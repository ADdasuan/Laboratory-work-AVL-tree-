#ifndef STRUCT_PUBLIC
#define STRUCT_PUBLIC

#include "struct_private.h"
#include "macros_for_return.h"
#include <stdbool.h>

Err_Func add_el(Tree_AVL* tree, KeyType key, InfoType info);

Err_Func del_el(Tree_AVL* tree, KeyType key, int index);

Err_Func traversing_AVL_tree_range(const Tree_AVL* tree, const KeyType min_key, const KeyType max_key, int silent_mode);

Spisok* search_by_key(const Tree_AVL* tree, KeyType key);

Spisok* special_search(const Tree_AVL* tree, KeyType target_key);

void print_tree_recursive(Node_Tree* node, int level, int is_right);

Err_Func output_tree(const Tree_AVL* tree);

Err_Func graphical_output(const Tree_AVL* tree);

Err_Func import_to_txt(Tree_AVL* tree, const char* filename);

#endif
