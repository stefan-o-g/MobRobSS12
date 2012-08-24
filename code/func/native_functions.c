#include "native_functions.h"
#include "sensor.h"



void nprint_int(struct stack* evalstack, struct memory* mem){
	display_clear();
	display_cursor(1,1);
	display_printf("%d",read(mem,0,0)._int);
}


void nmotor_set(struct stack* evalstack, struct memory* mem){
	motor_set(read(mem,0,0)._int,read(mem,0,1)._int);
}


void nsens_LDRL(struct stack* evalstack, struct memory* mem){
	bot_sens_isr();
	push(evalstack,int_entry(sensLDRL));
}

void nsens_LDRR(struct stack* evalstack, struct memory* mem){
	bot_sens_isr();
	push(evalstack,int_entry(sensLDRR));
}
