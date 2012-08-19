#include "memory.h"


struct memory* new_memory(int size){
	struct memory* memory = new(struct memory);
	memory->size = size;
	memory->entries = new_array(union entry, size);

	memset(memory->entries, 0, size * sizeof(union entry));

	memory->tos = size - ARSIZE;

	struct frame* initframe = get_frame(memory,memory->tos);
	initframe->dl._int = -1;
	initframe->sl._int = -1;

	return memory;
}



struct frame* get_frame(struct memory* memory, int index){
	return (struct frame*)(memory->entries + index);
}


struct frame* push_stack_frame(struct memory* memory, int var_count, int delta){
	int old_tos = memory->tos;
	memory->tos = memory->tos - (ARSIZE+var_count);
	struct frame* new_frame = get_frame(memory,memory->tos);
	new_frame->dl._int = old_tos;
	int new_sl = old_tos;
	int i;
	for(i = 0; i < delta; i++){
		new_sl = get_frame(memory,new_sl)->sl._int;
	}
	new_frame->sl._int = new_sl;
	return new_frame;
}

void pop_stack_frame(struct memory* memory){
	memory->tos = get_frame(memory, memory->tos)->dl._int;
}


void write(struct memory* memory, int delta, int offset, union entry entry){
	struct frame* frame = jump_sl(memory,get_frame(memory,memory->tos),delta);
	frame->vars[offset] = entry;
}

union entry read(struct memory* memory, int delta, int offset){
	struct frame* frame = jump_sl(memory,get_frame(memory,memory->tos),delta);
	return frame->vars[offset];
}

struct frame* jump_sl(struct memory* memory,struct frame* start, int steps){
	struct frame* frame = start;
	int i;
	for(i = 0; i < steps; i++){
		frame = get_frame(memory,frame->sl._int);
	}
	return frame;
}


void mem_dump(struct memory* memory, FILE* out){

	fprintf(out,"Memory Dump tos=%d\n[address][index][double][int]\n", memory->tos);
	fflush(out);
	int i;
	for(i = memory->size-1; i >= 0 ; i--){
		fprintf(out,"[%d][%d][%f][%d]\n",memory->entries+i, i, memory->entries[i]._double, memory->entries[i]._int);
		fflush(out);
	}
}

void mem_dump_frames(struct memory* memory, FILE* out){
	struct frame* frame = get_frame(memory, memory->tos);

}

int varcount(struct frame* frame){

}

union entry int_entry(int i){
	union entry e;
	e._int = i;
	return e;
}

union entry double_entry(double d){
	union entry e;
	e._double = d;
	return e;
}


/*
int main(){
	struct memory* mem =  new_memory(20);
	mem_dump(mem,stdout);
	push_stack_frame(mem,3,0);
	mem_dump(mem,stdout);
	write(mem,0,0,int_entry(42));
	write(mem,0,1,int_entry(43));
	write(mem,0,2,int_entry(44));
	mem_dump(mem,stdout);
	push_stack_frame(mem,0,0);
	write(mem,1,1,int_entry(50));
	mem_dump(mem,stdout);
	push_stack_frame(mem,0,1);
	write(mem,1,2,int_entry(55));
	mem_dump(mem,stdout);
}
*/

