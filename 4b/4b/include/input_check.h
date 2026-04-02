#ifndef INPUT
#define INPUT

#include <stdio.h>
#include "macros_for_return.h"

Err_Input input_unsigned(unsigned int* number, const char* message);

char* file_readline(FILE* file);

#endif
