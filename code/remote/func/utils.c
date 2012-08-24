#include "utils.h"

char* new_strcpy(char* text){
	char* copy = (char*)malloc((strlen(text)+1)*sizeof(char));
	return strcpy(copy,text);
}

void print_indent(FILE* file, int indent){
	fprintf(file,"%*s",indent,"");
}
