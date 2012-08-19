#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "memory.h"
#include "stack.h"
#include "native_functions.h"










void run(struct ast* function, int stacksize, int memorysize);
void interpret(struct ast* ast, struct ast* context, struct stack* evalstack, struct memory* mem, struct native_function* native_functions);

#endif
