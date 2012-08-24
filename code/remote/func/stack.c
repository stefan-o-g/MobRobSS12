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
	s->data[s->sp++] = entry;
	//printf("push %d %f sp %d\n", entry._int,entry._double, s->sp);
}

union entry pop(struct stack* s){
	if(is_empty(s)) exit(-1);
	union entry e = s->data[--s->sp];
	//printf("pop %d %f sp %d\n", e._int, e._double, s->sp);
	return e;
}

int is_empty(struct stack* s){
	return s->sp == 0;
}

int is_full(struct stack* s){
	return s->sp == s->capacity;
}


void stack_dump(struct stack* s, FILE* out){
	fprintf(out,"Stack Dump sp=%d\n[address][index][double][int]\n", s->sp);
	fflush(out);
	int i;
	for(i = 0; i < s->capacity ; i++){
		fprintf(out,"[%d][%d][%f][%d]\n",s->data+i, i, s->data[i]._double, s->data[i]._int);
		fflush(out);
	}
}
