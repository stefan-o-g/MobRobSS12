#include "semcheck.h"

int implicitly_convertable(enum datatype source, enum datatype destination){
	switch(source){
		case DOUBLE:
			switch(destination){
				case DOUBLE: return 1;
				case INT: return 1;
				case NONE: return 1;
				default: return 0;
			}
		break;
		case INT:
			switch(destination){
				case DOUBLE: return 1;
				case INT: return 1;
				case NONE: return 1;
				default: return 0;
			}
		break;
		case NONE:
			if(destination == NONE){
				return 1;
			}else{
				return 0;
			}
		break;
		default: return 0;
		break;
	}
}

void semcheck(struct ast* ast, struct ast* context){
	switch(ast->type){
		case FUNCDEC:
		{
			struct ast* list_item = ast->_funcdec.variables;
			while(list_item != NULL){

				enum datatype vartype = resolve_decalaration_type(list_item->_list.item,context);
				enum datatype exprtype = resolve_datatype(list_item->_list.item->_vardec.init,context);

				if(!implicitly_convertable(exprtype,vartype)){
					printf("%s can not implicitly converted to %s.\n", type_name(exprtype), type_name(vartype));
					exit(-1);
				}

				list_item = list_item->_list.next;
			}

			list_item = ast->_funcdec.functions;
			while(list_item != NULL){
				semcheck(list_item->_list.item, list_item->_list.item);
				list_item = list_item->_list.next;
			}

			if(!ast->_funcdec.is_native){
				semcheck(ast->_funcdec.body, context);
			}
		}
		break;
		case IDENT:
		{
			struct lookup_result result = lookup_id(context,ast->_ident.name);
			if(result.ast == NULL){
				printf("ident %s not defined.\n",ast->_ident.name);
				exit(-1);
			}else{
				if(result.ast->type == FUNCDEC){
					printf("ident %s not a variable.\n",ast->_ident.name);
					exit(-1);
				}
			}
		}
		break;
		case BINARY:
		{
			semcheck(ast->_binary.lhs,context);
			semcheck(ast->_binary.rhs,context);
		}
		break;
		case UNARY:
		{
			semcheck(ast->_unary.operand,context);
		}
		break;
		case IF:
		{
			semcheck(ast->_if.condition,context);

			enum datatype cond_type = resolve_datatype(ast->_if.condition,context);
			if(cond_type != INT){
				printf("if condition needs to be of type Int.\n");
				exit(-1);
			}

			semcheck(ast->_if.thenstmt,context);
			semcheck(ast->_if.elsestmt,context);
		}
		break;
		case WHILE:
		{
			semcheck(ast->_while.condition,context);

			enum datatype cond_type = resolve_datatype(ast->_while.condition,context);
			if(cond_type != INT){
				printf("while condition needs to be of type Int.\n");
				exit(-1);
			}

			semcheck(ast->_while.statement,context);
		}
		break;
		case LIST:
		{
			struct ast* list_item = ast;
			while(list_item != NULL){
				semcheck(list_item->_list.item,context);
				list_item = list_item->_list.next;
			}
		}
		break;
		case ASSIGN:
		{
			struct lookup_result result = lookup_id(context,ast->_assign.name);
			if(result.ast == NULL){
				printf("ident %s not defined.\n",ast->_assign.name);
				exit(-1);
			}else{
				if(result.ast->type == FUNCDEC){
					printf("ident %s not a variable.\n",ast->_assign.name);
					exit(-1);
				}else if(result.ast->_vardec.modifier == READONLY){
					printf("ident %s is is a constant and can not be reassigned.\n",ast->_assign.name);
					exit(-1);
				}
			}
			semcheck(ast->_assign.expr,context);

			enum datatype expr_type = resolve_datatype(ast->_assign.expr,context);
			enum datatype var_type = resolve_decalaration_type(result.ast,context);
			if(!implicitly_convertable(expr_type,var_type)){
				printf("%s can not implicitly converted to %s.\n",type_name(expr_type),type_name(var_type));
				exit(-1);
			}

		}
		break;
		case CALL:
		{
			struct lookup_result result = lookup_id(context,ast->_call.name);
			if(result.ast == NULL){
				printf("function %s not defined.\n",ast->_call.name);
				exit(-1);
			}else if(result.ast->type != FUNCDEC){
				printf("%s not a function.\n",ast->_call.name);
				exit(-1);
			}

			struct ast* param_list = result.ast->_funcdec.params;
			struct ast* value_list = ast->_call.params;
			if(list_length(param_list) != list_length(value_list)){
				printf("functioncall %s parameter count mismatch.\n",ast->_call.name);
				exit(-1);
			}


			while(value_list != NULL){

				semcheck(value_list->_list.item,context);

				enum datatype param_type = resolve_decalaration_type(param_list->_list.item,context);
				enum datatype expr_type = resolve_datatype(value_list->_list.item,context);

				if(!implicitly_convertable(expr_type,param_type)){
					printf("%s can not implicitly converted to %s.\n",type_name(expr_type),type_name(param_type));
					exit(-1);
				}

				param_list = param_list->_list.next;
				value_list = value_list->_list.next;
			}
		}
	}
}



