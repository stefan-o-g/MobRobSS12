#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "memory.h"
#include "stack.h"
#include "native_functions.h"





struct native_function{
	char* name;
	void (*func)(struct stack*, struct memory*);
	struct native_function* next;
};




struct native_function* new_native_function(char* name, void (func)(struct stack*, struct memory*), struct native_function* next);
struct native_function* find_native_function(char* name, struct native_function* list);



union entry run(struct ast* function, int stacksize, int memorysize);
void interpret(struct ast* ast, struct ast* context, struct stack* evalstack, struct memory* mem, struct native_function* native_functions);

#endif
