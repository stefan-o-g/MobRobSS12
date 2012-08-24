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




char* unop_name(enum unop op){
	switch(op){
		case CHS: return "CHS";
		case NOT: return "NOT";
	}
}

char* binop_name(enum binop op){
	switch(op){
		case ADD: return "ADD";
		case SUB: return "SUB";
		case MUL: return "MUL";
		case DIV: return "DIV";
		case MOD: return "MOD";
		case LT: return "LT";
		case LE: return "LE";
		case EQ: return "EQ";
		case NE: return "NE";
		case GT: return "GT";
		case GE: return "GE";
		case AND: return "AND";
		case OR: return "OR";
	}
}

char* type_name(enum datatype t){
	switch(t){
		case INT: return "INT";
		case DOUBLE: return "DOUBLE";
		case NONE: return "NONE";
		case INVALID_DATATYPE: return "INVALID_DATATYPE";
	}
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

char* modifier_name(enum modifier m){
	switch(m){
	case READONLY: return "READONLY";
	case NO_MODIFIER: return "NO_MODIFIER";
	default: return "UNKNOWN";
	}
}

void ast2xml(FILE* file, struct ast* ast, int indent){
	if(ast == NULL){
		//print_indent(file,indent);
		//fprintf(file,"<NULL/>\n");
		return;
	}
	switch(ast->type){
		case UNARY:
			print_indent(file,indent);
			fprintf(file,"<Unary op=\"%s\">\n",unop_name(ast->_unary.op));
			ast2xml(file,ast->_unary.operand, indent+1);
			print_indent(file,indent);
			fprintf(file,"</Unary>\n");
		break;
		case BINARY:
			print_indent(file,indent);
			fprintf(file,"<Binary op=\"%s\">\n",binop_name(ast->_binary.op));
				print_indent(file,indent+1);
				fprintf(file,"<lhs>\n");
					ast2xml(file,ast->_binary.lhs, indent+2);
				print_indent(file,indent+1);
				fprintf(file,"</lhs>\n");

				print_indent(file,indent+1);
				fprintf(file,"<rhs>\n");
					ast2xml(file,ast->_binary.rhs, indent+2);
				print_indent(file,indent+1);
				fprintf(file,"</rhs>\n");
			print_indent(file,indent);
			fprintf(file,"</Binary>\n");
		break;
		case IDENT:
			print_indent(file,indent);
			fprintf(file,"<Ident name=\"%s\" />\n",ast->_ident.name);
		break;
		case LITERAL:
			switch(ast->_literal.type){
				case DOUBLE:
					print_indent(file,indent);
					fprintf(file,"<Literal type=\"DOUBLE\" value=\"%lf\" />\n",ast->_literal._double);
				break;
				case INT:
					print_indent(file,indent);
					fprintf(file,"<Literal type=\"INT\" value=\"%d\" />\n",ast->_literal._int);
				break;
				default:
					print_indent(file,indent);
					fprintf(file,"<Literal type=\"UNKNOWN\" />\n");
				break;
			}
		break;
		case ASSIGN:
			print_indent(file,indent);
			fprintf(file,"<Assign ident=\"%s\">\n",ast->_assign.name);
				print_indent(file,indent+1);
				fprintf(file,"<rhs>\n");
					ast2xml(file, ast->_assign.expr, indent+2);
				print_indent(file,indent+1);
				fprintf(file,"</rhs>\n");
			print_indent(file,indent);
			fprintf(file,"</Assign>\n");
		break;
		case CALL:
			print_indent(file,indent);
			fprintf(file,"<Call name=\"%s\">\n",ast->_call.name);

			struct ast* param =  ast->_call.params;
			while(param != NULL){
				print_indent(file,indent+1);
				fprintf(file,"<param>\n");

				ast2xml(file,param->_list.item,indent+2);

				print_indent(file,indent+1);
				fprintf(file,"</param>\n");
				param = param->_list.next;
			}

			print_indent(file,indent);
			fprintf(file,"</Call>\n");
		break;
		case IF:
			print_indent(file,indent);
			fprintf(file,"<If>\n");

				print_indent(file,indent+1);
				fprintf(file,"<condition>\n");
				ast2xml(file, ast->_if.condition, indent+2);
				print_indent(file,indent+1);
				fprintf(file,"</condition>\n");


				print_indent(file,indent+1);
				fprintf(file,"<than>\n");
				ast2xml(file, ast->_if.thenstmt, indent+2);
				print_indent(file,indent+1);
				fprintf(file,"</than>\n");


				print_indent(file,indent+1);
				fprintf(file,"<else>\n");
				ast2xml(file, ast->_if.elsestmt, indent+2);
				print_indent(file,indent+1);
				fprintf(file,"</else>\n");


			print_indent(file,indent);
			fprintf(file,"</If>\n");
		break;
		case WHILE:
			print_indent(file,indent);
			fprintf(file,"<While>\n");

				print_indent(file,indent+1);
				fprintf(file,"<condition>\n");
				ast2xml(file, ast->_while.condition, indent+2);
				print_indent(file,indent+1);
				fprintf(file,"</condition>\n");


				print_indent(file,indent+1);
				fprintf(file,"<do>\n");
				ast2xml(file, ast->_while.statement, indent+2);
				print_indent(file,indent+1);
				fprintf(file,"</do>\n");



			print_indent(file,indent);
			fprintf(file,"</While>\n");
		break;
		case LIST:
			print_indent(file,indent);
			fprintf(file,"<List>\n");

			struct ast* list =  ast;
			while(list != NULL){
				ast2xml(file,list->_list.item,indent+1);
				list = list->_list.next;
			}

			print_indent(file,indent);
			fprintf(file,"</List>\n");
		break;
		case VARDEC:
			print_indent(file,indent);
			fprintf(file,"<Vardec name=\"%s\" modifier=\"%s\" declared_type=\"%s\">\n",ast->_vardec.name, modifier_name(ast->_vardec.modifier), type_name(ast->_vardec.declared_type));

			ast2xml(file, ast->_vardec.init, indent+1);

			print_indent(file,indent);
			fprintf(file,"</Vardec>\n");
		break;
		case FUNCDEC:
			print_indent(file,indent);
			char*  parent_name = ast->_funcdec.parent==NULL?"":ast->_funcdec.parent->_funcdec.name;
			fprintf(file,"<Funcdec name=\"%s\" parent=\"%s\" declared_type=\"%s\">\n",ast->_funcdec.name,parent_name , type_name(ast->_funcdec.declared_type));

			ast2xml(file,ast->_funcdec.params,indent+1);


			print_indent(file,indent+1);
			fprintf(file,"<Variables>\n");
			ast2xml(file, ast->_funcdec.variables, indent+2);
			print_indent(file,indent+1);
			fprintf(file,"</Variables>\n");



			print_indent(file,indent+1);
			fprintf(file,"<Functions>\n");
			ast2xml(file, ast->_funcdec.functions, indent+2);
			print_indent(file,indent+1);
			fprintf(file,"</Functions>\n");



			print_indent(file,indent+1);
			fprintf(file,"<Body>\n");

			if(!ast->_funcdec.is_native){
				ast2xml(file, ast->_funcdec.body, indent+2);
			}else{
				print_indent(file,indent+2);
				fprintf(file,"native\n");
			}
			print_indent(file,indent+1);
			fprintf(file,"</Body>\n");

			print_indent(file,indent);
			fprintf(file,"</Funcdec>\n");
		break;
		case PARAM:
			print_indent(file,indent);
			fprintf(file,"<Param name=\"%s\" type=\"%s\"/>\n",ast->_param.name, type_name(ast->_param.type));
		break;
		default:
		break;
	}
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




