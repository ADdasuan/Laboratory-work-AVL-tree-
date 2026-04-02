#include <stdio.h>
#include <stdlib.h>
#include "struct_private.h"
#include "struct_public.h"
#include "macros_for_return.h"
#include "input_check.h"
#include <limits.h>
#include <readline/readline.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "timing.h"

void menu(){
    printf("\n________Work this AVL Tree________\n");
    printf("1) Add element in tree \n");
    printf("2) Delete element in tree \n");
    printf("3) Bypass tree \n");
    printf("4) Search by key in the tree \n");
    printf("5) Special search in the tree\n");
    printf("6) Formatted tree output in the form of a tree \n");
    printf("7) Graphical output of the tree \n");
    printf("8) Loading a tree from a text file \n");
    printf("9) Timing functions \n");
    printf("\n_________________________________\n");

}

Err_Dialog block_add(Tree_AVL* tree){
    if (tree == NULL) {
    	return DIALOG_UNLUCK;
    }

    KeyType key = readline("Enter key : ");
    if (key == NULL) {
    	return DIALOG_CTRL_D;	
    }
    
    InfoType info;
    if (input_unsigned(&info,"Enter info: \n") == INPUT_CTRL_D) {
		if(key){
			free(key);
		}
        return DIALOG_CTRL_D;
    }

    Err_Dialog res = (add_el(tree, key, info) == FUNC_SUCCESS) ? DIALOG_SUCCESS : DIALOG_UNLUCK;

	free(key);
    return res;
}

Err_Dialog block_del(Tree_AVL* tree){
    if (tree == NULL) {
    	return DIALOG_UNLUCK;
	}
	
    KeyType key = readline("Enter key to delete: ");
    if (key == NULL) {
    	return DIALOG_CTRL_D;
	}

	unsigned int index = 0;
	
	
    Err_Dialog res = (del_el(tree, key, index) == FUNC_SUCCESS) ? DIALOG_SUCCESS : DIALOG_UNLUCK;
    
    if (key != NULL) {
        free(key);
    }
    
    return res;
}

Err_Dialog block_search_to_key(Tree_AVL* tree){
    if (!tree) return DIALOG_UNLUCK;

    char* key = readline("Enter key to search: ");
    if (!key) return DIALOG_CTRL_D;

    Spisok* results = search_by_key((const Tree_AVL*)tree, key);
    free(key);

    if (results && results->head) {
        printf("Found values:\n");
        Node_Spisok* current = results->head;
        int count = 1;
        while (current) {
            printf("%d. Info: %u\n", count++, current->info);
            current = current->next;
        }
        free_spisok(results);
        return DIALOG_SUCCESS;
    }

    printf("Not found\n");
    if (results) free_spisok(results);
    return DIALOG_UNLUCK;
}

Err_Dialog block_special_search(Tree_AVL* tree){
    if (!tree) return DIALOG_UNLUCK;

    char* target = readline("Enter target key: ");
    if (!target) return DIALOG_CTRL_D;

    Spisok* results = special_search((const Tree_AVL*)tree, target);
    free(target);

    if (results && results->head) {
        printf("Closest values:\n");
        Node_Spisok* current = results->head;
        int count = 1;
        while (current) {
            printf("%d. Info: %u\n", count++, current->info);
            current = current->next;
        }
        free_spisok(results);
        return DIALOG_SUCCESS;
    }

    printf("No close matches found\n");
    if (results) free_spisok(results);
    return DIALOG_UNLUCK;
}

Err_Dialog block_traversing_AVL_Tree(Tree_AVL* tree) {
    if (!tree) return DIALOG_UNLUCK;

    char* answer = readline("Use key range? (y/n): ");
    if (!answer) return DIALOG_CTRL_D;

    Err_Dialog result = DIALOG_UNLUCK;
    char* min_key = NULL;
    char* max_key = NULL;

    if (tolower(answer[0]) == 'y') {
        min_key = readline("Enter min key: ");
        if (!min_key) {
            free(answer);
            return DIALOG_CTRL_D;
        }

        max_key = readline("Enter max key: ");
        if (!max_key) {
            free(answer);
            free(min_key);
            return DIALOG_CTRL_D;
        }

        printf("Showing keys in range [%s, %s]:\n", min_key, max_key);
        result = (traversing_AVL_tree_range(tree, min_key, max_key, 0) == FUNC_SUCCESS) 
               ? DIALOG_SUCCESS : DIALOG_UNLUCK;
    } else {
        printf("Showing all keys:\n");
        result = (traversing_AVL_tree_range(tree, NULL, NULL, 0) == FUNC_SUCCESS)
               ? DIALOG_SUCCESS : DIALOG_UNLUCK;
    }

    free(answer);
    free(min_key);
    free(max_key);
    return result;
}

Err_Dialog block_output_tree(Tree_AVL* tree){
    if(!tree) return DIALOG_UNLUCK;
    return (output_tree(tree) == FUNC_SUCCESS) ? DIALOG_SUCCESS : DIALOG_UNLUCK;
}

Err_Dialog block_graphical_output(Tree_AVL* tree) {
    if (!tree) return DIALOG_UNLUCK;

    printf("Generating tree visualization...\n");
    Err_Dialog res = (graphical_output(tree) == FUNC_SUCCESS) ? DIALOG_SUCCESS : DIALOG_UNLUCK;

    if(res == DIALOG_SUCCESS) {
        printf("Visualization created successfully.\n");
    } else {
        printf("Visualization creation failed.\n");
    }

    return res;
}


Err_Dialog block_import_to_txt(Tree_AVL* tree){
    if (!tree) return DIALOG_UNLUCK;

    char *filename = readline("Enter filename: ");
    if (!filename) return DIALOG_UNLUCK;

    Err_Dialog res = (import_to_txt(tree, filename) == FUNC_SUCCESS) ? DIALOG_SUCCESS : DIALOG_UNLUCK;
    free(filename);

    if (res == DIALOG_SUCCESS) {
        printf("Tree imported successfully\n");
    } else {
        printf("Error importing tree\n");
    }

    return res;
}

Err_Dialog block_timing(){
    choice();
    unsigned int choice;
    if (input_unsigned(&choice,"Enter the number on function: \n") == INPUT_CTRL_D) {
        return DIALOG_CTRL_D;
    }
    printf("\n");

    switch(choice) {
        case 1:
            printf("%d\n", timing_add());
            break;

        case 2:
            timing_del();
            break;

        case 3:
            timing_search();
            break;

        case 4:
            timing_traverse();
            break;

        default:
            printf("Invalid choice!\n");
    }

    return DIALOG_SUCCESS;

}
