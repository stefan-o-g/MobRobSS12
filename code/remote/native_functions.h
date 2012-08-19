#ifndef NATIVE_FUNCTIONS_H
#define NATIVE_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "memory.h"
#include "stack.h"

struct native_function{
	char* name;
	void (*func)(struct stack*, struct memory*);
	struct native_function* next;
};




struct native_function* new_native_function(char* name, void (func)(struct stack*, struct memory*), struct native_function* next);
struct native_function* find_native_function(char* name, struct native_function* list);


void nmem_dump(struct stack* evalstack, struct memory* mem);
void nstack_dump(struct stack* evalstack, struct memory* mem);

void nprint_double(struct stack* evalstack, struct memory* mem);
void nprint_int(struct stack* evalstack, struct memory* mem);
void nread_int(struct stack* evalstack, struct memory* mem);
void nread_double(struct stack* evalstack, struct memory* mem);
void nrandom(struct stack* evalstack, struct memory* mem);
void nsin(struct stack* evalstack, struct memory* mem);

#endif
