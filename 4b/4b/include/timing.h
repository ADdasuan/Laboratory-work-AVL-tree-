#ifndef TIMING_H
#define TIMING_H

#include <sys/time.h>
#include "macros_for_return.h"

typedef struct {
    struct timeval start;
} Timer;

void timer_start(Timer *t);

double timer_elapsed_ms(Timer *t);

void choice();

Err_Timing timing_add();

Err_Timing timing_del();

Err_Timing timing_search();

Err_Timing timing_traverse();

#endif
