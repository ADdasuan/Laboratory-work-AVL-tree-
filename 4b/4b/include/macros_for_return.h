#ifndef MACROS
#define MACROS

typedef enum Err_Dialog {
    DIALOG_CTRL_D = -1,
    DIALOG_SUCCESS = 1,
    DIALOG_UNLUCK = 0
} Err_Dialog;


typedef enum Err_Func {
    FUNC_CTRL_D = -1,
    FUNC_SUCCESS = 0,
    FUNC_UNLUCK = -2,
    FUNC_MEMORY_ERROR = -3
} Err_Func;

typedef enum Err_Input {
    INPUT_CTRL_D = -1,
    INPUT_SUCCESS = 0,
    INPUT_UNLUCK = -2
} Err_Input;


typedef enum Err_Timing{
    TIMING_SUCCESS = 1,
    TIMING_UNLUCK = 0
}Err_Timing;

typedef enum Err_Spisok{
    SPISOK_UNLUCK = 0,
    SPISOK_SUCCESS = 1
}Err_Spisok;

#endif
