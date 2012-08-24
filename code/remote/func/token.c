#include "token.h"

void free_token(struct token* t){
  free(t->text);
  free(t);
}



void print_token(FILE* file, struct token* t){
	fprintf(file, "%s (%d, %d) %s\n",token_name(t->type), t->line, t->column, t->text);
}

void print_token_bot(FILE* file, struct token* t){
	fprintf(file, "%d %s\n",t->type, t->text);
}


char* token_name(enum token_type t){
	switch(t){
		case T_PLUS:		return "T_PLUS";
		case T_MINUS:		return "T_MINUS";
		case T_MUL:		return "T_MUL";
		case T_DIV:		return "T_DIV";
		case T_MOD:		return "T_MOD";
		case T_BECOMES:		return "T_BECOMES";
		case T_LPAR:		return "T_LPAR";
		case T_RPAR:		return "T_RPAR";
		case T_LT:		return "T_LT";
		case T_GT:		return "T_GT";
		case T_LE:		return "T_LE";
		case T_GE:		return "T_GE";
		case T_NE:		return "T_NE";
		case T_EQ:		return "T_EQ";
		case T_SEMICOLON:	return "T_SEMICOLON";
		case T_DOT:		return "T_DOT";
		case T_COMMA:		return "T_COMMA";
		case T_QUEST:		return "T_QUEST";
		case T_NOT:		return "T_NOT";
		case T_WHILE:		return "T_WHILE";
		case T_DO:		return "T_DO";
		case T_VARS:		return "T_VARS";
		case T_CONSTS:		return "T_CONSTS";
		case T_IF:		return "T_IF";
		case T_THEN:		return "T_THEN";
		case T_ELSE:		return "T_ELSE";
		case T_BEGIN:		return "T_BEGIN";
		case T_END:		return "T_END";
		case T_CALL:		return "T_CALL";
		case T_FUNCTION:	return "T_FUNCTION";
		case T_INT:		return "T_INT";
		case T_DOUBLE:		return "T_DOUBLE";
		case T_IDENT:		return "T_IDENT";
		case T_AND:		return "T_AND";
		case T_OR:		return "T_OR";
		case T_COMMENT:		return "T_COMMENT";
		case T_NATIVE:		return "T_NATIVE";
		case T_COLON:		return "T_COLON";
		case T_UNKNOWN:		return "T_UNKNOWN";
		case T_EOF:		return "T_EOF";
		default:		return "";
	}
}
