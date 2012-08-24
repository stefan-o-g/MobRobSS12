#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include "memory.h"

struct stack{
	int capacity;
	int sp;
	union entry data[];
};

struct stack* new_stack(int size);
void push(struct stack* s, union entry entry);
union entry pop(struct stack* s);
int is_empty(struct stack* s);
int is_full(struct stack* s);
int stack_size(struct stack* s);

void stack_dump(struct stack* s, FILE* out);

#endif
