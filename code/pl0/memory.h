#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include "utils.h"

#define ARSIZE (sizeof(struct frame) / sizeof(union entry))



union entry{
	int _int;
	double _double;
};


struct frame{
	union entry dl;
	union entry sl;
	union entry vars[];
};

struct memory{
	int size;
	int tos;
	union entry* entries;
};

struct memory* new_memory(int size);



struct frame* push_stack_frame(struct memory* memory, int var_count, int delta);
void pop_stack_frame(struct memory* memory);


void write(struct memory* memory, int delta, int offset, union entry entry);
union entry read(struct memory* memory, int delta, int offset);

struct frame* jump_sl(struct memory* memory,struct frame* start, int steps);
struct frame* get_frame(struct memory* memory, int index);


void mem_dump(struct memory* memory, FILE* out);
void mem_dump_frames(struct memory* memory, FILE* out);

union entry int_entry(int i);
union entry double_entry(double d);








#endif
