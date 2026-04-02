#include "timing.h"
#include <stddef.h>
#include "struct_private.h"
#include "struct_public.h"
#include <time.h>

static void shuffle(char** array, int size) {
    for (int i = size - 1; i > 0;--i) {
        int j = rand() % (i + 1);
        char* temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void timer_start(Timer *t) {
    gettimeofday(&t->start, NULL);
}

double timer_elapsed_ms(Timer *t) {
    struct timeval end;
    gettimeofday(&end, NULL);
    return (end.tv_sec - t->start.tv_sec) * 1000.0 + (end.tv_usec - t->start.tv_usec) / 1000.0;
}

void choice(){
    printf("\n");
    printf("1) Timing for function add element \n");
    printf("2) Timing for function del element \n");
    printf("3) Timing for function search element \n");
    printf("4) Timing for function traversing the tree \n");
    printf("\n");
}

static char* generate_key(int max_length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int length = 1 + rand() % (max_length - 1);
    char* key = malloc(length + 1);
    if (!key) return NULL;

    for (int i = 0; i < length;++i) {
        key[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    key[length] = '\0';
    return key;
}

static char** fill_tree_and_get_keys(Tree_AVL* tree, int size, int max_key_length) {
    char** keys = malloc(size * sizeof(char*));
    if (!keys) return NULL;

    for (int i = 0; i < size;++i) {
        keys[i] = generate_key(max_key_length);
        if (!keys[i]) {
            for (int j = 0; j < i;++j) free(keys[j]);
            free(keys);
            return NULL;
        }

        if (add_el(tree, keys[i], (unsigned)rand()) != FUNC_SUCCESS) {
            free(keys[i]);
            for (int j = 0; j < i;++j) free(keys[j]);
            free(keys);
            return NULL;
        }
    }
    return keys;
}

Err_Timing timing_add() {
    fflush(stdout);
    const int initial_sizes[] = {1000, 50000, 100000, 500000, 1000000, 2000000, 3000000};
    const int num_sizes = sizeof(initial_sizes)/sizeof(initial_sizes[0]);
    const int num_repeats = 10;
    const int nodes_to_add = 100;
    const int max_key_length = 1000;

    FILE* file = fopen("add_timing.csv", "w");
    if (!file) {
        return TIMING_UNLUCK;
    }

    fprintf(file, "Size,Run,TotalTimeMs,AvgTimePerAddMs\n");
    srand(time(NULL));

    for (int i = 0; i < num_sizes;++i) {
        int size = initial_sizes[i];
        double total_time = 0.0;
        double avg_time = 0.0;
        printf("\n=== Testing size: %d ===\n", size);
        for (int run = 0; run < num_repeats;++run) {
            Tree_AVL* tree = create_tree_AVL();
            if (tree == NULL) continue;

            char** initial_keys = fill_tree_and_get_keys(tree, size, max_key_length);
            if (initial_keys == NULL) {
                free_tree_AVL(tree);
                continue;
            }

            char** keys_to_add = (char**)malloc(nodes_to_add * sizeof(char*));
            unsigned* infos = (unsigned*)malloc(nodes_to_add * sizeof(unsigned));
            if (!keys_to_add || !infos) {

                for (int j = 0; j < size;++j) free(initial_keys[j]);
                free(initial_keys);
                free(keys_to_add);
                free(infos);
                free_tree_AVL(tree);
                continue;
            }


            int success = 1;
            for (int j = 0; j < nodes_to_add && success;++j) {
                keys_to_add[j] = generate_key(max_key_length);
                if (!keys_to_add[j]) success = 0;
                infos[j] = (unsigned)rand();
            }

            if (!success) {

                for (int j = 0; j < nodes_to_add;++j) free(keys_to_add[j]);
                for (int j = 0; j < size;++j) free(initial_keys[j]);
                if (initial_keys) {
                    free(initial_keys);
                }

                if (keys_to_add) {
                    free(keys_to_add);
                }

                if (infos) {
                    free(infos);
                }

                if (tree) {
                    free_tree_AVL(tree);
                }
                continue;
            }

            Timer t;
            timer_start(&t);
            for (int j = 0; j < nodes_to_add;++j) {
                add_el(tree, keys_to_add[j], infos[j]);
            }
            double elapsed = timer_elapsed_ms(&t);

            total_time += elapsed;

            fprintf(file, "%d,%d,%.15f\n", size, run+1, elapsed);

            for (int j = 0; j < nodes_to_add;++j) free(keys_to_add[j]);
            for (int j = 0; j < size;++j) free(initial_keys[j]);

			if (initial_keys) {
            	free(initial_keys);
            }

            if (keys_to_add) {
            	free(keys_to_add);
			}

			if (infos) {
            	free(infos);
            }

            if (tree) {
            	free_tree_AVL(tree);
            }
        }
        avg_time = total_time / num_repeats;
        printf("\n>>> Size %5d: Average time to add_el = %.15f ms\n\n", size, avg_time);
    }

    fclose(file);
    return TIMING_SUCCESS;
}

Err_Timing timing_del() {
    const int initial_sizes[] = {1000, 50000, 100000, 500000, 1000000, 2000000, 3000000};
    const int num_sizes = sizeof(initial_sizes)/sizeof(initial_sizes[0]);
    const int num_repeats = 100;  
    const int max_key_length = 1000;

    FILE* file = fopen("del_timing.csv", "w");
    if (!file) return TIMING_UNLUCK;

    fprintf(file, "Size,Run,TimeMs\n");
    srand(time(NULL));

    for (int i = 0; i < num_sizes;++i) {
        int size = initial_sizes[i];
        double total_time = 0.0;
        double avg_time = 0.0;
        printf("=== Testing size: %d ===\n", size);

        for (int run = 0; run < num_repeats;++run) {
            Tree_AVL* tree = create_tree_AVL();
            if (!tree) continue;

            char** keys = fill_tree_and_get_keys(tree, size, max_key_length);
            if (!keys) {
                free_tree_AVL(tree);
                continue;
            }

            Timer t;
            timer_start(&t);
            for (int j = 0; j < 100;++j) {  
                del_el(tree, keys[j], 0);
            }
            double elapsed = timer_elapsed_ms(&t);
			total_time += elapsed;
            fprintf(file, "%d,%d,%.15f\n", size, run+1, elapsed);

            for (int j = 0; j < size;++j) free(keys[j]);
            free(keys);
            free_tree_AVL(tree);
        }
        avg_time = total_time / num_repeats;
        printf("\n>>> Size %5d: Average time to del_el = %.15f ms\n\n", size, avg_time);        
    }
    fclose(file);
    return TIMING_SUCCESS;
}


Err_Timing timing_search() {
    const int initial_sizes[] = {1000, 50000, 100000, 500000, 1000000, 2000000, 3000000};
    const int num_sizes = sizeof(initial_sizes)/sizeof(initial_sizes[0]);
    const int nodes_to_del = 100;
    const int num_repeats = 100;
    const int max_key_length = 1000;

    FILE* file = fopen("search_timing.csv", "w");
    if (!file) return TIMING_UNLUCK;

    fprintf(file, "Size,Run,TotalTimeMs,AvgTimePerDelMs\n");
    srand(time(NULL));

    for (int i = 0; i < num_sizes;++i) {
        int size = initial_sizes[i];
        double total_time = 0.0;
        double avg_time = 0.0;
        printf("\n=== Testing size: %d ===\n", size);

        for (int run = 0; run < num_repeats;++run) {
            Tree_AVL* tree = create_tree_AVL();
            if (tree == NULL) continue;

            char** keys = fill_tree_and_get_keys(tree, size, max_key_length); 
            if (!keys) {
                free_tree_AVL(tree);
                continue;
            }

            shuffle(keys, size);

            Timer t;
            timer_start(&t);

            for (int j = 0; j < nodes_to_del;++j) {
                Spisok* result = search_by_key(tree, keys[j]);
                
                if (result != NULL) {
                	free_spisok(result);
                }
            }

            double elapsed = timer_elapsed_ms(&t);
            total_time += elapsed;
            fprintf(file, "%d,%d,%.15f\n", size, run+1, elapsed);
            for (int j = 0; j < size;++j) free(keys[j]);
            free(keys);
            free_tree_AVL(tree);
            
        }
        avg_time = total_time / num_repeats;
        printf("\n>>> Size %5d: Average time to search_el = %.15f ms\n\n", size, avg_time);
    }

    fclose(file);
    return TIMING_SUCCESS;
}



Err_Timing timing_traverse() {
    const int tree_sizes[] = {1000, 50000, 100000, 500000, 1000000, 2000000, 3000000};
    const int num_sizes = sizeof(tree_sizes)/sizeof(tree_sizes[0]);
    const int num_repeats = 100;
    const int max_key_length = 100;

    FILE* file = fopen("traverse_timing.csv", "w");
    if (!file) return TIMING_UNLUCK;

    fprintf(file, "Size,Run,TimeMs\n");
    srand(time(NULL));

    for (int i = 0; i < num_sizes;++i) {
        int size = tree_sizes[i];
        double total_time = 0.0;
        printf("\n=== Testing full traverse size: %d ===\n", size);

        for (int run = 0; run < num_repeats;++run) {
            Tree_AVL* tree = create_tree_AVL();
            if (tree == NULL) continue;

            char** keys = fill_tree_and_get_keys(tree, size, max_key_length);
            if (!keys) {
                free_tree_AVL(tree);
                continue;
            }

            Timer t;
            timer_start(&t);
            
            traversing_AVL_tree_range(tree, NULL, NULL, 1);
            
            double elapsed = timer_elapsed_ms(&t);
            total_time += elapsed;
            
            fprintf(file, "%d,%d,%.15f\n", size, run+1, elapsed);

            for (int j = 0; j < size;++j) free(keys[j]);
            free(keys);
            free_tree_AVL(tree);
        }

        double avg_time = total_time / num_repeats;
        printf(">>> Size %6d: Avg full traverse time = %.15f ms\n", size, avg_time);
    }

    fclose(file);
    return TIMING_SUCCESS;
}
