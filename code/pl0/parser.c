#include "parser.h"


int token_is(enum token_type check){
    struct token* t = get_token();
    if(t->type == check){
      return 1;
    }else{
      return 0;
    }
}

int expect(enum token_type expected){
    struct token* t = get_token();
    if(token_is(expected)){
      return 1;
    }else{
      //printf("Expected %s but found %s on line %d column %d.\n",token_name(expected),token_name(t->type), t->line, t->column);
      exit(-1);
      return 0;
    }
}



void parser_error(){
    struct token* t = get_token();
    //fprintf(stderr, "error in line %d on column %d.\n",t->line, t->column);
}

struct ast* parse_all(){
	next_token();
	return function_dec(NULL);
}

struct ast* all_expr(){
	return control_expr();
}

struct ast* constdec(){
	struct ast* expr = NULL;

	if(expect(T_IDENT)){
		char* ident = new_strcpy(get_token()->text);
		next_token();

		enum datatype type = INVALID_DATATYPE;
		//optional type annotation
		if(token_is(T_COLON)){
			type = typename2type(next_token()->text);
			if(type == INVALID_DATATYPE){
				struct token* t = get_token();
				//fprintf(stdout,"Error: %s on line %d column %d is not a valid datatype.\n",t->text,t->line,t->column);
			}
			next_token();
		}

		if(expect(T_BECOMES)){
			next_token();
			struct ast* init = all_expr();
			expr = new_vardec(ident,type,READONLY,init);
		}
	}

	return expr;
}

struct ast* vardec(){
	struct ast* expr = NULL;

	if(expect(T_IDENT)){
		char* ident = new_strcpy(get_token()->text);
		int line = get_token()->line;
		int column = get_token()->column;

		next_token();

		enum datatype type = INVALID_DATATYPE;
		//optional type annotation
		if(token_is(T_COLON)){
			type = typename2type(next_token()->text);
			if(type == INVALID_DATATYPE){
				struct token* t = get_token();
				//fprintf(stdout,"%s on line %d column %d is not a valid datatype.\n",t->text,t->line,t->column);
			}
			next_token();
		}

		if(token_is(T_BECOMES)){
			next_token();
			struct ast* init = all_expr();
			expr = new_vardec(ident,type,NO_MODIFIER,init);
		}else{
			if(type != INVALID_DATATYPE){
				expr = new_vardec(ident,type,NO_MODIFIER,default_value_for(type));
			}else{
				//fprintf(stdout,"You need to either supply a datatype or an initial value for variable %s on line %d column %d.\n",ident,line,column);
			}
		}
	}

	return expr;
}



struct ast* param(){
	struct ast* expr = NULL;
	if(expect(T_IDENT)){
		char* paramname = new_strcpy(get_token()->text);
		next_token();
		if(expect(T_COLON)){
			next_token();
			if(expect(T_IDENT)){
				enum datatype type = typename2type(get_token()->text);
				if(type != INVALID_DATATYPE){
					expr = new_param(paramname,type);
				}
				next_token();
			}
		}
	}
	return expr;
}

struct ast* function_dec(struct ast* parent){
	struct ast* expr = NULL;

	if(expect(T_FUNCTION)){
		next_token();

		if(expect(T_IDENT)){
			char* funcname = new_strcpy(get_token()->text);
			next_token();

			if(expect(T_LPAR)){
				next_token();

				struct ast* paramlist = NULL;

				if(!token_is(T_RPAR)){
					struct ast* last = paramlist = new_list(param(),NULL);
					while(token_is(T_COMMA)){
						next_token();
						last = last->_list.next = new_list(param(),NULL);
					}
				}

				if(expect(T_RPAR)){
					next_token();
				}

				enum datatype type = INVALID_DATATYPE;

				if(expect(T_COLON)){
					next_token();
					if(expect(T_IDENT)){
						type = typename2type(get_token()->text);
						next_token();
					}
				}



				if(expect(T_BECOMES)){
					struct ast* body = NULL;
					struct ast* varlist = NULL;
					struct ast* functions = NULL;
					next_token();
					if(token_is(T_NATIVE)){
						next_token();
						expr = new_funcdec(parent,funcname,paramlist,varlist,functions,type,1,body);
					}else{

						struct ast* last = NULL;

						if(token_is(T_CONSTS)){
							next_token();
							varlist = new_list(constdec(), NULL);
							last = varlist;

							while(token_is(T_COMMA)){
								next_token();
								last = last->_list.next = new_list(constdec(), NULL);
							}
							if(expect(T_SEMICOLON)){
								next_token();
							}
						}


						if(token_is(T_VARS)){
							next_token();
							if(varlist == NULL){
								varlist = new_list(vardec(), NULL);
								last = varlist;
							}else{
								last = last->_list.next = new_list(vardec(), NULL);
							}

							while(token_is(T_COMMA)){
								next_token();
								last = last->_list.next = new_list(vardec(), NULL);
							}
							if(expect(T_SEMICOLON)){
								next_token();
							}
						}

						expr = new_funcdec(parent,funcname,paramlist,varlist,functions,type,0,body);

						if(token_is(T_FUNCTION)){
							struct ast* last = functions = new_list(function_dec(expr),NULL);
							while(token_is(T_FUNCTION)){
								last = last->_list.next = new_list(function_dec(expr),NULL);
							}
						}

						body = all_expr();
						expr->_funcdec.functions = functions;
						expr->_funcdec.body = body;
					}




				}


			}
		}
	}

	return expr;
}

struct ast* control_expr(){
	struct ast* expr = NULL;

	if(token_is(T_IF)){
		next_token();
		struct ast* cond = all_expr();
		if(expect(T_THEN)){
			next_token();
			struct ast* then = all_expr();
			struct ast* els = NULL;
			if(token_is(T_ELSE)){
				next_token();
				els = all_expr();
			}
			expr = new_if(cond,then,els);
		}
	}else if(token_is(T_WHILE)){
		next_token();
		struct ast* cond = all_expr();
		if(expect(T_DO)){
			next_token();
			expr = new_while(cond,all_expr());
		}
	}else{
		expr = equality_expr();
	}

	return expr;
}

struct ast* equality_expr(){
  struct ast* expr = NULL;

  expr = additive_expr();

  while(token_is(T_GT) ||
		token_is(T_GE) ||
		token_is(T_EQ) ||
		token_is(T_NE) ||
		token_is(T_LE) ||
		token_is(T_LT)){
	enum binop op = (token_is(T_GT)?GT:
					 token_is(T_GE)?GE:
					 token_is(T_EQ)?EQ:
					 token_is(T_NE)?NE:
					 token_is(T_LE)?LE:LT);
	next_token();
	struct ast* rhs = additive_expr();
	expr = new_binop(op,expr,rhs);
  }


  return expr;
}

struct ast* additive_expr(){
  struct ast* expr = NULL;

  expr = multiplicative_expr();

  while(token_is(T_PLUS) || token_is(T_MINUS)){
	enum binop op = (token_is(T_PLUS)?ADD:SUB);
	next_token();
	struct ast* rhs = multiplicative_expr();
	expr = new_binop(op,expr,rhs);
  }

  return expr;
}

struct ast* multiplicative_expr(){
  struct ast* expr = NULL;

  expr = factor_expr();

  while(token_is(T_MUL) || token_is(T_DIV) || token_is(T_MOD)){
    enum binop op = (token_is(T_MUL)?MUL:
    				 token_is(T_DIV)?DIV:MOD);

    next_token();
    struct ast* rhs = factor_expr();
    expr = new_binop(op,expr,rhs);
  }

  return expr;
}


struct ast* factor_expr(){
	struct ast* expr = NULL;

	if(token_is(T_NOT)){
		next_token();
		expr = new_unop(NOT,factor_expr());
	}else if(token_is(T_PLUS)){
		next_token();
		expr = factor_expr();
	}else if(token_is(T_MINUS)){
		next_token();
		expr = new_unop(CHS,factor_expr());
	}else if(token_is(T_IDENT)){
		char* ident_text = new_strcpy(get_token()->text);
		next_token();
		if(token_is(T_BECOMES)){
				next_token();
				struct ast* assign_expr = all_expr();
				expr = new_assign(ident_text,assign_expr);
		}else if(token_is(T_LPAR)){
			next_token();
			struct ast* params = NULL;

			if(!token_is(T_RPAR)){
				params = new_list(all_expr(), NULL);
				struct ast* last = params;
				while(token_is(T_COMMA)){
					next_token();
					last = last->_list.next = new_list(all_expr(), NULL);
				}
			}

			if(expect(T_RPAR)){
				next_token();
				expr = new_call(ident_text, params);
			}
		}else{
			expr = new_ident(ident_text);
		}
	}else if(token_is(T_INT)){
		expr = new_int_literal(atoi(get_token()->text));
		next_token();
	}else if(token_is(T_DOUBLE)){
		expr = new_double_literal(atof(get_token()->text));
		next_token();
	}else if(token_is(T_LPAR)){
		next_token();
		expr = all_expr();
		expect(T_RPAR);
		next_token();
	}else if(token_is(T_BEGIN)){
		next_token();
		struct ast* list = new_list(all_expr(),NULL);
		struct ast* last = list;
		while(token_is(T_SEMICOLON)){
			next_token();
			last = last->_list.next = new_list(all_expr(), NULL);
		}
		if(expect(T_END)){
			next_token();
		}
		expr = list;
	}
	
	return expr;
}



/*
int main(int argc, char* argv[]){
    FILE *in = fopen(argv[1],"r");
    set_scanner_input(in);

    //print_tokens(stdout);
    //print_bot(stdout);

    struct ast* ast = parse_all();
    //ast2xml(stdout,ast,0);

    union entry result;
    result = run(ast,16,100);
    //printf("result = int: %d, double %f\n",result._int,result._double);


    /*struct lookup_result a;
    a = lookup_id(ast,argv[2]);
    printf("%s: level %d offset %d\n",argv[2],a.level,a.offset);
    if(a.ast->type == FUNCDEC){
        struct lookup_result b;
        b = lookup_id(a.ast,argv[3]);
        printf("%s: from %s level %d offset %d\n",argv[3],a.ast->_funcdec.name,b.level,b.offset);
    }




    //printf("%s\n",type_name(resolve_datatype(ast)));
}
*/
