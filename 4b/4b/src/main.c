#include <stdio.h>
#include <stdlib.h>
#include "macros_for_return.h"
#include "auxiliary.h"
#include "struct_private.h"
#include "input_check.h"

int main(){
    Tree_AVL* tree = create_tree_AVL();

    int flag = 0;
    while (flag == 0) {
        menu();

        unsigned int choice;
        if (input_unsigned(&choice,"Enter the number on function: \n") == INPUT_CTRL_D) {
            free_tree_AVL(tree);
            return -1;
        }
        printf("\n");

        Err_Dialog func ;
        switch(choice) {
            case 1:
                func = block_add(tree);
                break;

            case 2:
                func = block_del(tree);
                break;

            case 3:
            	func = block_traversing_AVL_Tree(tree);
                break;

            case 4:
                func = block_search_to_key(tree);
                break;

            case 5:
                func = block_special_search(tree);
                break;

            case 6:
                func = block_output_tree(tree);
                break;

            case 7:
                func = block_graphical_output(tree);
                break;

            case 8:
                func = block_import_to_txt(tree);
                break;

            case 9:
                func = block_timing();
                break;

            default:
                printf("Invalid choice!\n");
        }

        if (func == DIALOG_CTRL_D) {
            flag = 1;
            free_tree_AVL(tree);
            return -1;
        }
    }
    free_tree_AVL(tree);
    return 0;
}
