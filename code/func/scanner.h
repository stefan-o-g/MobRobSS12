#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include "token.h"




struct token* get_token();
struct token* next_token();

#endif
