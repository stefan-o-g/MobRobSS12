#ifndef TOKEN_H
#define TOKEN_H

#include "utils.h"


enum token_type{
	T_EOF,
	T_PLUS,
	T_MINUS,
	T_MUL,
	T_DIV,
	T_MOD,
	T_BECOMES,
	T_LPAR,
	T_RPAR,
	T_LT,
	T_GT,
	T_LE,
	T_GE,
	T_NE,
	T_EQ,
	T_AND,
	T_OR,
	T_NOT,
	T_SEMICOLON,
	T_DOT,
	T_COMMA,
	T_QUEST,
	T_WHILE,
	T_DO,
	T_VARS,
	T_CONSTS,
	T_IF,
	T_THEN,
	T_ELSE,
	T_BEGIN,
	T_END,
	T_CALL,
	T_FUNCTION,
	T_INT,
	T_DOUBLE,
	T_IDENT,
	T_COMMENT,
	T_NATIVE,
	T_COLON,
	T_UNKNOWN,
};

struct token{
	enum token_type type;
	int line;
	int column;
	char* text;
};


#endif
