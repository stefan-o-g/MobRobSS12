#ifndef NATIVE_FUNCTIONS_H
#define NATIVE_FUNCTIONS_H



#include "memory.h"
#include "stack.h"
#include "../include/andy.h"

struct native_function{
	char* name;
	void (*func)(struct stack*, struct memory*);
	struct native_function* next;
};




struct native_function* new_native_function(char* name, void (func)(struct stack*, struct memory*), struct native_function* next);
struct native_function* find_native_function(char* name, struct native_function* list);


void nprint_int(struct stack* evalstack, struct memory* mem);
void nmotor_set(struct stack* evalstack, struct memory* mem);

void nsens_LDRL(struct stack* evalstack, struct memory* mem);
void nsens_LDRR(struct stack* evalstack, struct memory* mem);


#endif
