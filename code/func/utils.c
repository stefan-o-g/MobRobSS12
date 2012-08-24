#include "utils.h"

char* new_strcpy(char* text){
	char* copy = (char*)malloc((strlen(text)+1)*sizeof(char));
	return strcpy(copy,text);
}

