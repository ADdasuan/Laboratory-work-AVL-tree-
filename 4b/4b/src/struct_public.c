#include "struct_public.h"
#include <graphviz/gvc.h>
#include "struct_private.h"
#include <stdio.h>
#include "macros_for_return.h"
#include <string.h>
#include "balance.h"
#include <stdbool.h>
#include "stack.h"
#include <unistd.h>
#include "input_check.h"

Err_Func add_el(Tree_AVL* tree, KeyType key, InfoType info) {
    if (!tree || !key) return FUNC_UNLUCK;

    Node_Tree* parent = NULL;
    Node_Tree* existing = internal_search(tree, key, 0, &parent);

    if (existing) {
        Node_Spisok* new_dup = create_list_node(info);
        if (!new_dup) return FUNC_MEMORY_ERROR;
        
        new_dup->next = existing->duplicates->head;
        existing->duplicates->head = new_dup;
        return FUNC_SUCCESS;
    }

    Node_Tree* new_node = create_tree_node(key, info);
    if (!new_node) return FUNC_MEMORY_ERROR;

    if (!parent) {
        tree->root = new_node;
    } else {
        int cmp = strcmp(key, parent->key);
        if (cmp < 0) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
    }

    balance_path_from_node(tree, new_node);
    return FUNC_SUCCESS;
}

Err_Func del_el(Tree_AVL* tree, KeyType key, int index) {
    if (!tree || !tree->root) return FUNC_UNLUCK;

    Node_Tree* parent = NULL;
    Node_Tree* to_delete = internal_search(tree, key, index, &parent);

    if (!to_delete) return FUNC_UNLUCK;

    Node_Spisok* prev = NULL;
    Node_Spisok* curr = to_delete->duplicates->head;
    int i = 0;
    
    while (curr && i < index) {
        prev = curr;
        curr = curr->next;
        i++;
    }

    if (!curr) return FUNC_UNLUCK;

    if (prev) {
        prev->next = curr->next;
    } else {
        to_delete->duplicates->head = curr->next;
    }
    free(curr);

    if (to_delete->duplicates->head) {
        return FUNC_SUCCESS;
    }


    Node_Tree* replacement = NULL;
    
    if (!to_delete->left && !to_delete->right) {
        replacement = NULL;
    } 
    else if (!to_delete->left) {
        replacement = to_delete->right;
    } 
    else if (!to_delete->right) {
        replacement = to_delete->left;
    } 
    else {
        Node_Tree* succ_parent = to_delete;
        Node_Tree* successor = to_delete->right;

        while (successor->left) {
            succ_parent = successor;
            successor = successor->left;
        }

        if (succ_parent != to_delete) {
            succ_parent->left = successor->right;
            successor->right = to_delete->right;
        }
        successor->left = to_delete->left;
        replacement = successor;
    }

    if (!parent) {
        tree->root = replacement;
    } else if (parent->left == to_delete) {
        parent->left = replacement;
    } else {
        parent->right = replacement;
    }

    free(to_delete->key);
    free(to_delete->duplicates);
    free(to_delete);

    balance_path_from_node(tree, parent ? parent : replacement);
    return FUNC_SUCCESS;
}

Spisok* search_by_key(const Tree_AVL* tree, KeyType key) {
    if (!tree || !tree->root) return NULL;

    if (key == NULL) return NULL;


    Spisok* result = (Spisok*)malloc(sizeof(Spisok));
    if (!result) return NULL;
    result->head = NULL;

    int seq_number = 0;
    Node_Tree* current_node;

    while ((current_node = internal_search((Tree_AVL*)tree, key, seq_number, NULL)) != NULL) {
        Node_Spisok* info_curr = current_node->duplicates->head;
        while (info_curr) {
            Node_Spisok* new_node = (Node_Spisok*)malloc(sizeof(Node_Spisok));
            if (!new_node) {
                free_spisok(result);
                return NULL;
            }
            new_node->info = info_curr->info;
            new_node->next = result->head;
            result->head = new_node;

            info_curr = info_curr->next;
        }
        seq_number++;
    }

    return result;
}

Spisok* special_search(const Tree_AVL* tree, KeyType target_key) {
    if (!tree || !tree->root) return NULL;

    Node_Tree* current = tree->root;
    Node_Tree* candidate = NULL;
    int min_diff = -1;

    while (current) {
        int cmp = strcmp(current->key, target_key);
        if (cmp != 0) {
            int diff = abs(cmp);
            if (min_diff == -1 || diff < min_diff) {
                min_diff = diff;
                candidate = current;
            }
        }
        
        current = (strcmp(target_key, current->key) < 0) ? current->left : current->right;
    }

    if (!candidate) return NULL;

    return search_by_key(tree, candidate->key);
}

Err_Func traversing_AVL_tree_range(const Tree_AVL* tree, const KeyType min_key, const KeyType max_key, int silent_mode) {
    if (!tree || !tree->root) return FUNC_UNLUCK;

    Stack* stack = create_stack();
    if (!stack) return FUNC_MEMORY_ERROR;

    int found = 0;
    Node_Tree* current = tree->root;
    int use_range = (min_key != NULL && max_key != NULL);

    while (!stack_is_empty(stack) || current) {
        while (current) {
            if (use_range && strcmp(current->key, min_key) < 0) {
                current = current->right;
            } else {
                if (!stack_push(stack, current)) {
                    free_stack(stack);
                    return FUNC_MEMORY_ERROR;
                }
                current = current->left;
            }
        }

        current = stack_pop(stack);
        if (!current) break;
        if (!use_range || (strcmp(current->key, min_key) >= 0 && (strcmp(current->key, max_key) <= 0))) {
            found = 1;
            if (!silent_mode) {
                printf("Key: %s\n", current->key);
                if (current->duplicates) {
                    Node_Spisok* list_node = current->duplicates->head;
                    while (list_node) {
                        printf("  Info: %d\n", list_node->info);
                        list_node = list_node->next;
                    }
                }
            }
        }
        if (use_range && strcmp(current->key, max_key) > 0) {
            break;
        }

        current = current->right;
    }

    free_stack(stack);
    return found ? FUNC_SUCCESS : FUNC_UNLUCK;
}

Spisok* search_to_key(const Tree_AVL* tree, KeyType key) {
    if (!tree || !tree->root) return NULL;
    
    Node_Tree* current = tree->root;
    while (current) {
        int cmp = strcmp(key, current->key);
        if (cmp == 0) {
            return current->duplicates;
        }
        current = (cmp < 0) ? current->left : current->right;
    }
    return NULL;
}

void print_tree_recursive(Node_Tree* node, int level, int is_right) {
    if (!node) return;

    print_tree_recursive(node->right, level + 1, 1);

    for (int i = 0; i < level; i++) {
        printf(i == level - 1 ? "    " : "    ");
    }

    if (level > 0) {
        printf(is_right ? "┌── " : "└── ");
    }

    printf("%s", node->key);
    
    if (node->duplicates->head) {
        printf(" [");
        Node_Spisok* current = node->duplicates->head;
        while (current) {
            printf("%u", current->info);
            if (current->next) printf(", ");
            current = current->next;
        }
        printf("]");
    }
    printf("\n");

    print_tree_recursive(node->left, level + 1, 0);
}

Err_Func output_tree(const Tree_AVL* tree) {
    if (!tree || !tree->root) {
        printf("\nTree is empty\n");
        return FUNC_UNLUCK;
    }

    printf("\n──── AVL Tree ────\n");
    print_tree_recursive(tree->root, 0, 0);
    printf("───────────────────\n");
    return FUNC_SUCCESS;
}

Err_Func graphical_output(const Tree_AVL* tree) {
    if (!tree) return FUNC_UNLUCK;

    if (!tree->root) {
        printf("Tree is empty. Nothing to visualize.\n");
        return FUNC_UNLUCK;
    }

    FILE* file = fopen("tree.dot", "w");
    if (!file) {
        return FUNC_UNLUCK;
    }
    
    fprintf(file, "digraph G {\n");
    fprintf(file, "  node [shape=circle, fontname=\"Arial\", fontsize=12];\n");
    fprintf(file, "  edge [arrowhead=vee];\n\n");

    dot_export_AVL_recursive(file, tree->root);

    fprintf(file, "}\n");
    fclose(file);

    file = fopen("tree.dot", "r");
    if (!file) {
        return FUNC_UNLUCK;
    }

    Agraph_t* G;
    GVC_t* gvc;

    gvc = gvContext();
    G = agread(file, NULL);
    fclose(file);

    gvLayout(gvc, G, "dot");
    gvRenderFilename(gvc, G, "png", "tree.png");
    gvFreeLayout(gvc, G);
    agclose(G);
    gvFreeContext(gvc);
/*
    if (system("command -v dot >/dev/null 2>&1") != 0) {
        return FUNC_UNLUCK;
    }

    int res = system("dot -Tpng tree.dot -o tree.png 2>/dev/null");
    if (res != 0) {
        return FUNC_UNLUCK;
    }

    if (access("tree.png", F_OK) != 0) {
        return FUNC_UNLUCK;
    }

    if (system("xdg-open tree.png >/dev/null 2>&1") != 0) {
        system("open tree.png >/dev/null 2>&1");
    }
*/
    return FUNC_SUCCESS;
}


Err_Func import_to_txt(Tree_AVL* tree, const char* filename) {
    if (!tree || !filename) return FUNC_UNLUCK;

    FILE* file = fopen(filename, "r");
    if (!file) return FUNC_UNLUCK;

    char* line = NULL;
    KeyType key = NULL;
    InfoType info = 0;
    int line_count = 0;
    Err_Func result = FUNC_UNLUCK;

    while ((line = file_readline(file)) != NULL) {
        if (line_count % 2 == 0) {
            free(key);
            key = strdup(line);
            if (!key) {
                result = FUNC_MEMORY_ERROR;
                break;
            }
        } else {
            if (sscanf(line, "%u", &info) != 1) {
                result = FUNC_UNLUCK;
                break;
            }

            Node_Tree* current = tree->root;
            Node_Tree* found = NULL;
            
            while (current && !found) {
                int cmp = strcmp(key, current->key);
                if (cmp == 0) {
                    found = current;
                } else if (cmp < 0) {
                    current = current->left;
                } else {
                    current = current->right;
                }
            }

            if (found) {
                Node_Spisok* new_duplicate = create_list_node(info);
                if (!new_duplicate) {
                    result = FUNC_MEMORY_ERROR;
                    break;
                }
                new_duplicate->next = found->duplicates->head;
                found->duplicates->head = new_duplicate;
                result = FUNC_SUCCESS;
            } else {
                result = add_el(tree, key, info);
                if (result != FUNC_SUCCESS) {
                    break;
                }
            }
        }
        free(line);
        line = NULL;
        line_count++;
    }
    free(key);
    free(line);
    fclose(file);

    if (result != FUNC_SUCCESS) {
        return result;
    }

    return (line_count > 0 && line_count % 2 == 0) ? FUNC_SUCCESS : FUNC_UNLUCK;
}
