#include "ast.h"


int list_length(struct ast* list){
	return list == NULL?0:1+list_length(list->_list.next);
}


struct ast* new_call(char* name, struct ast* params){
	struct ast* r = new(struct ast);
	r->type = CALL;
	r->_call.name = name;
	r->_call.params = params;
	return r;
}

struct ast* new_unop(enum unop op, struct ast* operand){
	struct ast* r = new(struct ast);
	r->type = UNARY;
	r->_unary.op = op;
	r->_unary.operand = operand;
	return r;
}


struct ast* new_int_literal(int value){
	struct ast* r = new(struct ast);
	r->type = LITERAL;
	r->_literal.type = INT;
	r->_literal._int = value;
	return r;
}

struct ast* new_double_literal(double value){
	struct ast* r = new(struct ast);
	r->type = LITERAL;
	r->_literal.type = DOUBLE;
	r->_literal._double = value;
	return r;
}

struct ast* new_ident(char* value){
	struct ast* r = new(struct ast);
	r->type = IDENT;
	r->_ident.name = value;
	return r;
}



struct ast* new_assign(char* name, struct ast* expr){
	struct ast* r = new(struct ast);
	r->type = ASSIGN;
	r->_assign.name = name;
	r->_assign.expr = expr;
	return r;
}

struct ast* new_binop(enum binop op, struct ast* lhs, struct ast* rhs){
	struct ast* r = new(struct ast);
	r->type = BINARY;
	r->_binary.op = op;
	r->_binary.lhs = lhs;
	r->_binary.rhs = rhs;
	return r;
}

struct ast* new_while(struct ast* condition, struct ast* statement){
	struct ast* r = new(struct ast);
	r->type = WHILE;
	r->_while.condition = condition;
	r->_while.statement = statement;
	return r;
}

struct ast* new_if(struct ast* condition, struct ast* thenstmt, struct ast* elsestmt){
	struct ast* r = new(struct ast);
	r->type = IF;
	r->_if.condition = condition;
	r->_if.thenstmt = thenstmt;
	r->_if.elsestmt = elsestmt;
	return r;
}

struct ast* new_vardec(char* name, enum datatype type, enum modifier mod, struct ast* init){
	struct ast* r = new(struct ast);
	r->type = VARDEC;
	r->_vardec.name = name;
	r->_vardec.modifier =  mod;
	r->_vardec.init = init;
	r->_vardec.declared_type = type;
	return r;
}


struct ast* new_list(struct ast* item, struct ast* next){
	struct ast* r = new(struct ast);
	r->type = LIST;
	r->_list.item = item;
	r->_list.next = next;
	return r;
}

struct ast* new_funcdec(struct ast* parent, char* name, struct ast* params, struct ast* variables, struct ast* functions, enum datatype type, int native, struct ast* body){
	struct ast* r = new(struct ast);
	r->type = FUNCDEC;
	r->_funcdec.name = name;
	r->_funcdec.params = params;
	r->_funcdec.variables =  variables;
	r->_funcdec.functions = functions;
	r->_funcdec.declared_type = type;
	r->_funcdec.is_native = native;
	r->_funcdec.body = body;
	r->_funcdec.parent = parent;
	return r;
}

struct ast* new_param(char* name, enum datatype type){
	struct ast* r = new(struct ast);
	r->type = PARAM;
	r->_param.name = name;
	r->_param.type = type;
	return r;
}









struct ast* default_value_for(enum datatype type){
	switch(type){
		case INT: return new_int_literal(0);
		case DOUBLE: return new_double_literal(0);
		case NONE: return NULL;
		case INVALID_DATATYPE: return NULL;
	}
}

enum datatype typename2type(char* name){
	if(strcmp(name, "Int") == 0){
		return INT;
	}else if(strcmp(name, "Double") == 0){
		return DOUBLE;
	}else if(strcmp(name, "None") == 0){
		return NONE;
	}else{
		return INVALID_DATATYPE;
	}
}


void ast2xml(struct ast* ast){
	delay(2000);
	if(ast == NULL){
		display_clear();
		display_cursor(1,1);
		display_printf("NULL");
		return;
	}
	switch(ast->type){
		case UNARY:

		break;
		case BINARY:

		break;
		case IDENT:

		break;
		case LITERAL:
			switch(ast->_literal.type){
				case DOUBLE:

				break;
				case INT:
					display_clear();
					display_cursor(1,1);
					display_printf("int %d",ast->_literal._int);
				break;
				default:

				break;
			}
		break;
		case ASSIGN:

		break;
		case CALL:
			display_clear();
			display_cursor(1,1);
			display_printf("call %s",ast->_call.name);
			ast2xml(ast->_call.params);
		break;
		case IF:

		break;
		case WHILE:

		break;
		case LIST:
			display_clear();
			display_cursor(1,1);
			display_printf("list begin");
			struct ast* list =  ast;
			while(list != NULL){
				ast2xml(list->_list.item);
				list = list->_list.next;
			}
			display_clear();
			display_cursor(1,1);
			display_printf("list end");
		break;
		case VARDEC:

		break;
		case FUNCDEC:
			//char*  parent_name = ast->_funcdec.parent==NULL?"":ast->_funcdec.parent->_funcdec.name;
			//fprintf(file,"<Funcdec name=\"%s\" parent=\"%s\" declared_type=\"%s\">\n",ast->_funcdec.name,parent_name , type_name(ast->_funcdec.declared_type));

			display_clear();
			display_cursor(1,1);
			display_printf("fdec begin %s", ast->_funcdec.name);

			ast2xml(ast->_funcdec.params);





			display_clear();
			display_cursor(1,1);
			display_printf("vars begin");
			ast2xml(ast->_funcdec.variables);
			display_clear();
			display_cursor(1,1);
			display_printf("vars end");

			delay(2000);

			display_clear();
			display_cursor(1,1);
			display_printf("func begin");
			ast2xml(ast->_funcdec.functions);
			display_clear();
			display_cursor(1,1);
			display_printf("func end");

			delay(2000);

			display_clear();
			display_cursor(1,1);
			display_printf("body begin");

			if(!ast->_funcdec.is_native){
				ast2xml(ast->_funcdec.body);
			}else{
				delay(2000);
				display_clear();
				display_cursor(1,1);
				display_printf("native");
			}
			delay(2000);
			display_clear();
			display_cursor(1,1);
			display_printf("body end");

			delay(2000);
			display_clear();
			display_cursor(1,1);
			display_printf("fdec end");

		break;
		case PARAM:
			display_clear();
			display_cursor(1,1);
			display_printf("param %s", ast->_param.name);
		break;
		default:
		break;
	}
	delay(2000);
}



enum datatype resolve_decalaration_type(struct ast* declaration, struct ast* function){
	switch(declaration->type){
		case VARDEC:
		{
			if(declaration->_vardec.declared_type == INVALID_DATATYPE){
				return resolve_datatype(declaration->_vardec.init, function);
			}else{
				return declaration->_vardec.declared_type;
			}
		}
		break;
		case PARAM:
			return declaration->_param.type;
		break;
		case FUNCDEC:
			return declaration->_funcdec.declared_type;
		break;
		default: return INVALID_DATATYPE;
	}
}

enum datatype resolve_datatype(struct ast* expr, struct ast* function){
	if(expr == NULL){
		return NONE;
	}
	switch(expr->type){
		case ASSIGN:
			return resolve_datatype(expr->_assign.expr, function);
		break;
		case LITERAL:
			return expr->_literal.type;
		break;
		case IDENT:
		{
			struct lookup_result l = lookup_id(function, expr->_ident.name);
			if(l.ast != NULL && (l.ast->type == VARDEC || l.ast->type == PARAM)){
				return resolve_decalaration_type(l.ast,function);
			}else{
				return INVALID_DATATYPE;
			}
		}
		case CALL:
		{
			struct lookup_result l = lookup_id(function, expr->_ident.name);
			if(l.ast != NULL && l.ast->type == FUNCDEC){
				return resolve_decalaration_type(l.ast,function);
			}else{
				return INVALID_DATATYPE;
			}
		}
		break;
		case BINARY:
		{
			enum datatype lhs_type = resolve_datatype(expr->_binary.lhs, function);
			enum datatype rhs_type = resolve_datatype(expr->_binary.rhs, function);
			switch(expr->_binary.op){
				case ADD:
				case SUB:
				case DIV:
				case MUL:
					switch(lhs_type){
						case DOUBLE:
							switch(rhs_type){
								case DOUBLE: return DOUBLE;
								case INT: return DOUBLE;
								default: return INVALID_DATATYPE;
							}
						break;
						case INT:
							switch(rhs_type){
								case DOUBLE: return DOUBLE;
								case INT: return INT;
								default: return INVALID_DATATYPE;
							}
						break;
						default: return INVALID_DATATYPE;
						break;
					}
				break;
				case LE:
				case LT:
				case EQ:
				case NE:
				case GT:
				case GE:
					switch(lhs_type){
						case DOUBLE:
							switch(rhs_type){
								case DOUBLE: return INT;
								case INT: return INT;
								default: return INVALID_DATATYPE;
							}
						break;
						case INT:
							switch(rhs_type){
								case DOUBLE: return INT;
								case INT: return INT;
								default: return INVALID_DATATYPE;
							}
						break;
						default: return INVALID_DATATYPE;
						break;
					}
				break;
				case MOD:
					switch(lhs_type){
						case DOUBLE:
							switch(rhs_type){
								case DOUBLE: return INVALID_DATATYPE;
								case INT: return INVALID_DATATYPE;
								default: return INVALID_DATATYPE;
							}
						break;
						case INT:
							switch(rhs_type){
								case DOUBLE: return INVALID_DATATYPE;
								case INT: return INT;
								default: return INVALID_DATATYPE;
							}
						break;
						default: return INVALID_DATATYPE;
						break;
					}
				break;
				default: return INVALID_DATATYPE;
			}
		}
		break;
		case UNARY:
		{
			enum datatype operand = resolve_datatype(expr->_unary.operand, function);
			switch(expr->_unary.op){
				case CHS:
					switch(operand){
						case INT: return INT;
						case DOUBLE: return DOUBLE;
						default: return INVALID_DATATYPE;
					}
				break;
				case NOT:
					switch(operand){
						case INT: return INT;
						case DOUBLE: return INVALID_DATATYPE;
						default: return INVALID_DATATYPE;
					}
				break;
				default: return INVALID_DATATYPE;
			}
		}
		break;
		case LIST:
		{
			struct ast* last = expr;
			while(last->_list.next != NULL){
				last = last->_list.next;
			}
			return resolve_datatype(last->_list.item, function);
		}
		break;
		case IF:
		{
			enum datatype then_type = resolve_datatype(expr->_if.thenstmt, function);
			enum datatype else_type = then_type;
			if(expr->_if.elsestmt != NULL){
				else_type = resolve_datatype(expr->_if.elsestmt, function);
			}
			switch(then_type){
				case DOUBLE:
					switch(else_type){
						case DOUBLE: return DOUBLE;
						case INT: return DOUBLE;
						case NONE: return NONE;
						default: return INVALID_DATATYPE;
					}
				break;
				case INT:
					switch(else_type){
						case DOUBLE: return DOUBLE;
						case INT: return INT;
						case NONE: return NONE;
						default: return INVALID_DATATYPE;
					}
				break;
				case NONE:
					return NONE;
				break;
				default: return INVALID_DATATYPE;
				break;
			}
		}
		break;
		case WHILE:
			return NONE;
		break;
		default: return INVALID_DATATYPE;
	}
}


struct lookup_result lookup_id(struct ast* function, char* id){
	struct lookup_result loc = {0,0,NULL};

	while(function != NULL){

		struct ast* params = function->_funcdec.params;
		while(params != NULL){
			struct ast* param =params->_list.item;
			if(strcmp(param->_param.name, id) == 0){
				loc.ast = param;
				return loc;
			}else{
				loc.offset++;
				params = params->_list.next;
			}
		}

		struct ast* variables = function->_funcdec.variables;
		while(variables != NULL){
			struct ast* var = variables->_list.item;
			if(strcmp(var->_vardec.name, id) == 0){
				loc.ast = var;
				return loc;
			}else{
				loc.offset++;
				variables = variables->_list.next;
			}
		}

		struct ast* functions = function->_funcdec.functions;
		while(functions != NULL){
			struct ast* func = functions->_list.item;
			if(strcmp(func->_funcdec.name, id) == 0){
				loc.ast = func;
				return loc;
			}else{
				loc.offset++;
				functions = functions->_list.next;
			}
		}

		loc.offset = 0;
		loc.level++;
		function = function->_funcdec.parent;

	}
	loc.offset = -1;
	loc.level = -1;
	loc.ast = NULL;
	return loc;

}




