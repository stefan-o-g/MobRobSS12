#include "stack.h"


struct stack* new_stack(int size){
	struct stack* s = malloc(sizeof(struct stack) + (size*sizeof(union entry)));
	s->capacity = size;
	s->sp = 0;
	return s;
}

int stack_size(struct stack* s){
	return s->sp;
}

void push(struct stack* s, union entry entry){
	if(is_full(s)) exit(-1);
	//printf("push %d\n", entry._int);
	s->data[s->sp++] = entry;
}

union entry pop(struct stack* s){
	if(is_empty(s)) exit(-1);
	union entry e = s->data[--s->sp];
	//printf("pop %d\n", e._int);
	return e;
}

int is_empty(struct stack* s){
	return s->sp == 0;
}

int is_full(struct stack* s){
	return s->sp == s->capacity;
}
