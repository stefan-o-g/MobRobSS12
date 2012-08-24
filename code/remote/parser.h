#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "utils.h"
#include "ast.h"
#include "interpreter.h"


void parser_error();

struct ast* parse_all();

struct ast* all_expr();
struct ast* control_expr();
struct ast* equality_expr();
struct ast* additive_expr();
struct ast* multiplicative_expr();
struct ast* factor_expr();

struct ast* block();
struct ast* constdec();
struct ast* vardec();
struct ast* param();
struct ast* function_dec();
