#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "ast.h"

struct symboltable{
	struct scope{
		struct scope* parent;
		struct symbol{
			char* id;
			struct ast* entry;
			struct symbol* next;
		};
	};
};


struct symbol* lookup_symbol(struct symboltable* symtab, char* id);
int insert(struct symboltable* symtab, struct ast* entry);


#endif
