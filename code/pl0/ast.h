#ifndef AST_H
#define AST_H

#include "utils.h"


struct ast{
	enum{
		LITERAL, IDENT, ASSIGN, BINARY, UNARY, CALL, LIST, WHILE, IF, VARDEC, FUNCDEC, BLOCK, PARAM
	} type;
	union{
		struct{
			enum datatype{
				INVALID_DATATYPE, INT, DOUBLE, NONE
			} type;
			union{
				int _int;
				double _double;
			};
		} _literal;

		struct{
			char* name;
		} _ident;

		struct{
			char* name;
			struct ast* expr;
		} _assign;

		struct{
			enum binop{
				ADD, SUB, MUL, DIV, MOD, LT, LE, EQ, NE, GT, GE, AND, OR
			} op;
			struct ast* lhs;
			struct ast* rhs;
		} _binary;

		struct{
			enum unop{
				CHS, NOT
			} op;
			struct ast* operand;
		} _unary;

		struct{
			char* name;
			struct ast* params;
		} _call;

		struct{
			struct ast* item;
			struct ast* next;
		} _list;

		struct{
			struct ast* condition;
			struct ast* thenstmt;
			struct ast* elsestmt;
		} _if;

		struct{
			struct ast* condition;
			struct ast* statement;
		} _while;
		

		struct{
			char* name;
			enum datatype declared_type;
			enum modifier{
				NO_MODIFIER = 0, READONLY = 1
			} modifier;
			struct ast* init;
		} _vardec;


		struct{
			struct ast* parent;
			char* name;
			struct ast* params;
			struct ast* variables;
			struct ast* functions;
			enum datatype declared_type;
			int is_native;
			struct ast* body;
		} _funcdec;

		struct{
			char* name;
			enum datatype type;
		} _param;
	};


};


struct lookup_result{
	int offset;
	int level;
	struct ast* ast;
};

int list_length(struct ast* list);


struct ast* new_call(char* name, struct ast* params);
struct ast* new_unop(enum unop op, struct ast* operand);
struct ast* new_int_literal(int value);
struct ast* new_double_literal(double value);
struct ast* new_ident(char* value);
struct ast* new_assign(char* name, struct ast* expr);
struct ast* new_binop(enum binop op, struct ast* lhs, struct ast* rhs);
struct ast* new_list(struct ast* item, struct ast* next);
struct ast* new_while(struct ast* condition, struct ast* statement);
struct ast* new_if(struct ast* condition, struct ast* thenstmt, struct ast* elsestmt);
struct ast* new_vardec(char* name, enum datatype type, enum modifier mod, struct ast* init);
struct ast* new_funcdec(struct ast* parent, char* name, struct ast* params, struct ast* variables, struct ast* functions, enum datatype type, int native, struct ast* body);
struct ast* new_param(char* name, enum datatype type);



char* modifier_name(enum modifier m);

enum datatype typename2type(char* name);
char* type_name(enum datatype t);

struct ast* default_value_for(enum datatype type);

enum datatype resolve_decalaration_type(struct ast* declaration, struct ast* function);
enum datatype resolve_datatype(struct ast* expr, struct ast* function);

struct lookup_result lookup_id(struct ast* function, char* id);

void semantic_check(struct ast* ast);

#endif
