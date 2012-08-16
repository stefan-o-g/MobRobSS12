#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include "token.h"


struct token* get_token(void);
struct token* next_token(void);

#endif
