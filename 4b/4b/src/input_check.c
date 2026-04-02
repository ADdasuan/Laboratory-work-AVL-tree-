#include <stdio.h>
#include "macros_for_return.h"
#include <string.h>
#include <stdlib.h>
#include "input_check.h"

Err_Input input_unsigned(unsigned int* number, const char* message) {
        int flag = 0;
        while(flag == 0){
                char temp;
                int vvod;
                printf("%s\n", message);
                int res = scanf("%d%c", &vvod, &temp);
                if (res == 2) {
                        if (temp == '\n' && vvod >= 0) {
                                *number = (unsigned int)vvod;
                                flag = 1;
                                return INPUT_SUCCESS;
                        } else {
                                printf("You input a uncorrect number! Try again\n");
                                scanf("%*[^\n]%*c");
                        }
                }
                else if (res == EOF) {
                        flag = -1;
                        return INPUT_CTRL_D;
                }
                else {
                        printf("UNCORRECT NUMBER! NRY AGAIN!\n");
                        scanf("%*[^\n]%*c");
                }

        }
        return INPUT_CTRL_D;

}

#define BUF_INIT_SIZE 32

char* file_readline(FILE* file) {
    size_t buffer_size = BUF_INIT_SIZE;
    size_t position = 0;
    char* buffer = malloc(buffer_size);
    if (!buffer) {
        return NULL;
    }

    int symbol;

    int flag = 0;
    while(flag == 0) {
        symbol = fgetc(file);

        if (symbol == EOF) {
            if (feof(file) && position > 0) {
                flag = 1;
                break;
            }
            free(buffer);
            flag = -1;
            return NULL;
        }

        if (symbol == '\n') {
            flag = 1;
            break;
        }

        if (position + 1 >= buffer_size) {
            buffer_size *= 2;
            char* new_buffer = realloc(buffer, buffer_size);
            if (!new_buffer) {
                free(buffer);
                flag = -1;
                return NULL;
            }
            buffer = new_buffer;
        }

        buffer[position++] = (char)symbol;
    }

    buffer[position] = '\0';
    return buffer;
}
