#include "interpreter.h"


struct native_function* find_native_function(char* name, struct native_function* list){
	struct native_function* f = list;
	while(f != NULL){
		if(strcmp(f->name,name) == 0){
			return f;
		}
		f = f->next;
	}
	return f;
}

struct native_function* new_native_function(char* name, void (func)(struct stack*, struct memory*), struct native_function* next){
	struct native_function* f = new(struct native_function);
	f->name = name;
	f->func = func;
	f->next = next;
	return f;
}








union entry run(struct ast* function, int stacksize, int memorysize){
	struct memory* mem = new_memory(memorysize);
	struct stack* evalstack = new_stack(stacksize);

	struct native_function* pint = new_native_function("print_int",print_int,NULL);
	struct native_function* pdouble = new_native_function("print_double",print_double,pint);
	struct native_function* native_functions = new_native_function("debug",debug,pdouble);


	struct ast* call = new_call("main",NULL);
	struct ast* bootstrap_function = new_funcdec(NULL,"environment",NULL,NULL,new_list(function,NULL),NONE,0,NULL);
	function->_funcdec.parent = bootstrap_function;

	interpret(call , bootstrap_function, evalstack, mem, native_functions);
	return pop(evalstack);
}

void interpret(struct ast* ast, struct ast* context, struct stack* evalstack, struct memory* mem, struct native_function* native_functions){
	switch(ast->type){
		case CALL:
		{

			struct lookup_result loc = lookup_id(context,ast->_call.name);

			int paramcount = list_length(loc.ast->_funcdec.params);
			int varcount = list_length(loc.ast->_funcdec.variables);


			struct ast* params = ast->_call.params;
			while(params != NULL){
				interpret(params->_list.item,context,evalstack,mem, native_functions);
				params = params->_list.next;
			}

			push_stack_frame(mem,varcount + paramcount,loc.level);
			//printf("%s push_stack_frame delta %d var_count %d paramcount %d tos %d\n", loc.ast->_funcdec.name , loc.level, varcount, paramcount,mem->tos);


			int i;
			for(i = paramcount-1; i >= 0; i--){
				write(mem,0,i,pop(evalstack));
			}
			//mem_dump(mem,stdout);

			if(!loc.ast->_funcdec.is_native){

				struct ast* vars = loc.ast->_funcdec.variables;
				int offset = paramcount;
				while(vars != NULL){
					interpret(vars->_list.item->_vardec.init,loc.ast,evalstack,mem, native_functions);
					write(mem,0,offset,pop(evalstack));
					offset++;
					vars = vars->_list.next;
				}




				interpret(loc.ast->_funcdec.body,loc.ast,evalstack,mem, native_functions);

			}else{
				struct native_function* f = find_native_function(ast->_call.name,native_functions);
				if(f != NULL){
					f->func(evalstack,mem);
				}else{
					//printf("native function %s not found.\n", ast->_call.name);
					//exit(-1);
				}
			}
			pop_stack_frame(mem);
			//printf("pop tos %d\n",mem->tos);
			//mem_dump(mem,stdout);

		}
		break;
		case IDENT:
		{
			struct lookup_result loc = lookup_id(context,ast->_ident.name);
			union entry e = read(mem,loc.level,loc.offset);
			push(evalstack,e);
		}
		break;
		case LITERAL:
		{
			switch(ast->_literal.type){
				case INT: push(evalstack, int_entry(ast->_literal._int));
				break;
				case DOUBLE: push(evalstack, double_entry(ast->_literal._double));
				break;
			}
		}
		break;
		case UNARY:
		{
			interpret(ast->_unary.operand, context, evalstack, mem, native_functions);
			union entry e = pop(evalstack);
			enum datatype type = resolve_datatype(ast->_unary.operand, context);
			switch(ast->_unary.op){
				case CHS:
					switch(type){
						case DOUBLE:
							push(evalstack, double_entry(-e._double));
						break;
						case INT:
							push(evalstack, int_entry(-e._int));
						break;
					}
				break;
				case NOT:
					switch(type){
						case DOUBLE:
							push(evalstack, double_entry(!e._double));
						break;
						case INT:
							push(evalstack, int_entry(!e._int));
						break;
					}
				break;
			}
		}
		break;
		case BINARY:
		{
			interpret(ast->_binary.lhs, context, evalstack, mem, native_functions);
			interpret(ast->_binary.rhs, context, evalstack, mem, native_functions);
			union entry rhs = pop(evalstack);
			union entry lhs = pop(evalstack);

			enum datatype rhs_type = resolve_datatype(ast->_binary.lhs, context);
			enum datatype lhs_type = resolve_datatype(ast->_binary.rhs, context);
			switch(ast->_binary.op){
				case ADD:
					switch(lhs_type){
							case DOUBLE:
								switch(rhs_type){
									case DOUBLE: push(evalstack,  double_entry(lhs._double + rhs._double));
									break;
									case INT: push(evalstack, double_entry(lhs._double + rhs._int));
									break;
								}
							break;
							case INT:
								switch(rhs_type){
									case DOUBLE: push(evalstack, double_entry(lhs._int + rhs._double));
									break;
									case INT: push(evalstack, int_entry(lhs._int + rhs._int));
									break;
								}
							break;
						}
				break;

				case SUB:
					switch(lhs_type){
							case DOUBLE:
								switch(rhs_type){
									case DOUBLE: push(evalstack, double_entry(lhs._double - rhs._double));
									break;
									case INT: push(evalstack, double_entry(lhs._double - rhs._int));
									break;
								}
							break;
							case INT:
								switch(rhs_type){
									case DOUBLE: push(evalstack, double_entry(lhs._int - rhs._double));
									break;
									case INT: push(evalstack, int_entry(lhs._int - rhs._int));
									break;
								}
							break;
						}
				break;

				case DIV:
					switch(lhs_type){
						case DOUBLE:
							switch(rhs_type){
								case DOUBLE: push(evalstack, double_entry(lhs._double / rhs._double));
								break;
								case INT: push(evalstack, double_entry(lhs._double / rhs._int));
								break;
							}
						break;
						case INT:
							switch(rhs_type){
								case DOUBLE: push(evalstack, double_entry(lhs._int / rhs._double));
								break;
								case INT: push(evalstack, int_entry(lhs._int / rhs._int));
								break;
							}
						break;
					}
				break;

				case MUL:
					switch(lhs_type){
						case DOUBLE:
							switch(rhs_type){
								case DOUBLE: push(evalstack, double_entry(lhs._double * rhs._double));
								break;
								case INT: push(evalstack, double_entry(lhs._double * rhs._int));
								break;
							}
						break;
						case INT:
							switch(rhs_type){
								case DOUBLE: push(evalstack, double_entry(lhs._int * rhs._double));
								break;
								case INT: push(evalstack, int_entry(lhs._int * rhs._int));
								break;
							}
						break;
					}
				break;

				case LE:
					switch(lhs_type){
						case DOUBLE:
							switch(rhs_type){
								case DOUBLE: push(evalstack, int_entry(lhs._double <= rhs._double));
								break;
								case INT: push(evalstack, int_entry(lhs._double <= rhs._int));
								break;
							}
						break;
						case INT:
							switch(rhs_type){
								case DOUBLE: push(evalstack, int_entry(lhs._int <= rhs._double));
								break;
								case INT: push(evalstack, int_entry(lhs._int <= rhs._int));
								break;
							}
						break;
					}
				break;

				case LT:
					switch(lhs_type){
						case DOUBLE:
							switch(rhs_type){
								case DOUBLE: push(evalstack, int_entry(lhs._double < rhs._double));
								break;
								case INT: push(evalstack, int_entry(lhs._double < rhs._int));
								break;
							}
						break;
						case INT:
							switch(rhs_type){
								case DOUBLE: push(evalstack, int_entry(lhs._int < rhs._double));
								break;
								case INT: push(evalstack, int_entry(lhs._int < rhs._int));
								break;
							}
						break;
					}
				break;

				case EQ:
					switch(lhs_type){
						case DOUBLE:
							switch(rhs_type){
								case DOUBLE: push(evalstack, int_entry(lhs._double == rhs._double));
								break;
								case INT: push(evalstack, int_entry(lhs._double == rhs._int));
								break;
							}
						break;
						case INT:
							switch(rhs_type){
								case DOUBLE: push(evalstack, int_entry(lhs._int == rhs._double));
								break;
								case INT: push(evalstack, int_entry(lhs._int == rhs._int));
								break;
							}
						break;
					}
				break;

				case NE:
					switch(lhs_type){
						case DOUBLE:
							switch(rhs_type){
								case DOUBLE: push(evalstack, int_entry(lhs._double != rhs._double));
								break;
								case INT: push(evalstack, int_entry(lhs._double != rhs._int));
								break;
							}
						break;
						case INT:
							switch(rhs_type){
								case DOUBLE: push(evalstack, int_entry(lhs._int != rhs._double));
								break;
								case INT: push(evalstack, int_entry(lhs._int != rhs._int));
								break;
							}
						break;
					}
				break;

				case GT:
					switch(lhs_type){
						case DOUBLE:
							switch(rhs_type){
								case DOUBLE: push(evalstack, int_entry(lhs._double > rhs._double));
								break;
								case INT: push(evalstack, int_entry(lhs._double > rhs._int));
								break;
							}
						break;
						case INT:
							switch(rhs_type){
								case DOUBLE: push(evalstack, int_entry(lhs._int > rhs._double));
								break;
								case INT: push(evalstack, int_entry(lhs._int > rhs._int));
								break;
							}
						break;
					}
				break;

				case GE:
					switch(lhs_type){
						case DOUBLE:
							switch(rhs_type){
								case DOUBLE: push(evalstack, int_entry(lhs._double >= rhs._double));
								break;
								case INT: push(evalstack, int_entry(lhs._double >= rhs._int));
								break;
							}
						break;
						case INT:
							switch(rhs_type){
								case DOUBLE: push(evalstack, int_entry(lhs._int >= rhs._double));
								break;
								case INT: push(evalstack, int_entry(lhs._int >= rhs._int));
								break;
							}
						break;
					}
				break;

				case MOD:
					push(evalstack, int_entry(lhs._int % rhs._int));
				break;
			}
		}
		break;
		case LIST:
		{
			struct ast* last = ast;
			int size = stack_size(evalstack);
			while(last->_list.next != NULL){
				interpret(last->_list.item,context,evalstack,mem, native_functions);
				//clean stack since we only use the last value of the list
				if(stack_size(evalstack) > size){
					pop(evalstack);
				}
				last = last->_list.next;
			}
			interpret(last->_list.item,context,evalstack,mem, native_functions);
		}
		break;
		case IF:
		{
			interpret(ast->_if.condition , context, evalstack, mem, native_functions);
			int cond = pop(evalstack)._int;
			if(cond){
				interpret(ast->_if.thenstmt , context, evalstack, mem, native_functions);
			}else{
				interpret(ast->_if.elsestmt , context, evalstack, mem, native_functions);
			}
		}
		break;
		case WHILE:
		{
			interpret(ast->_while.condition , context, evalstack, mem, native_functions);
			int cond = pop(evalstack)._int;
			while(cond){
				interpret(ast->_while.statement,context,evalstack,mem, native_functions);
				interpret(ast->_while.condition , context, evalstack, mem, native_functions);
				cond = pop(evalstack)._int;
			}
		}
		break;
		case ASSIGN:

		break;
	}
}
