#ifndef NATIVE_FUNCTIONS_H
#define NATIVE_FUNCTIONS_H

#include "memory.h"
#include "stack.h"

void debug(struct stack* evalstack, struct memory* mem);
void print_double(struct stack* evalstack, struct memory* mem);
void print_int(struct stack* evalstack, struct memory* mem);

#endif
