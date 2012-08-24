#include "native_functions.h"


void nmem_dump(struct stack* evalstack, struct memory* mem){
	mem_dump(mem, stdout);
}

void nstack_dump(struct stack* evalstack, struct memory* mem){
	stack_dump(evalstack, stdout);
}

void nprint_double(struct stack* evalstack, struct memory* mem){
	printf("%f\n",read(mem,0,0)._double);
}

void nprint_int(struct stack* evalstack, struct memory* mem){
	printf("%d\n",read(mem,0,0)._int);
}

void nread_int(struct stack* evalstack, struct memory* mem){
	int i;
	do{
		printf("Enter a valid integer: ");
	}while(scanf("%d",&i) == EOF);
	push(evalstack,int_entry(i));
}

void nread_double(struct stack* evalstack, struct memory* mem){
	double d;
	do{
		printf("Enter a valid double: ");
	}while(scanf("%lf",&d) == EOF);
	push(evalstack,double_entry(d));
}


void nrandom(struct stack* evalstack, struct memory* mem){
	static int first = 1;
	if(first){
		srand ( time(NULL) );
		first = 0;
	}
	push(evalstack, int_entry(rand()));
}

void nsin(struct stack* evalstack, struct memory* mem){
	double param = read(mem,0,0)._double;
	double result = sin(param);
	push(evalstack, double_entry(result));
}
