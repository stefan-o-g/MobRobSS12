#ifndef UTILS_H
#define UTILS_H

#include <malloc.h>
#include <string.h>

#define new_array(type, size) (type*)malloc(sizeof(type)*size)

//#define new(type) (type*)malloc(sizeof(type))
#define new(type) new_array(type,1)



char* new_strcpy(char* text);

#endif
