#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include "token.h"

void set_scanner_input(FILE* file);
void set_token_output(FILE* file);
struct token* get_token();
struct token* next_token();

#endif
