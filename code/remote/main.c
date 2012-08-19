#include <argp.h>
#include <stdlib.h>

#include "ast.h"
#include "parser.h"
#include "scanner.h"
#include "interpreter.h"
#include "semcheck.h"




 
const char *argp_program_version = "FunCompiler 1.0.0";
const char *argp_program_bug_address = "<kevin.walter.private@googlemail.com>";

/* Program documentation. */
static char doc[] = "A Simple Compiler for the Fun Programming Language.";

/* A description of the arguments we accept. */
static char args_doc[] = "SOURCE";

/* The options we understand. */
static struct argp_option options[] = {
	{"xml",    	'x', "FILE", OPTION_ARG_OPTIONAL, 	"Outputs the AST as XML (default stdout)",	2},
	{"tokens", 	't', "FILE", OPTION_ARG_OPTIONAL, 	"Outputs the Tokens (default stdout)",		2},
	{"stacksize", 	's', "SIZE", 0, 	"Sets the evaluation stack size of the interpreter.",		2},
	{"memsize", 	'm', "SIZE", 0, 	"Sets the memory size of the interpreter.",		2},
	{"semcheck", 	'c', NULL, OPTION_ARG_OPTIONAL, 	"Performs the Semantic Check",	2},
	{"interpret", 	'i', NULL, OPTION_ARG_OPTIONAL, 	"Interprets the AST",	2},
	{ 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
	int xml;
	char* xmlfilename;

	int tokens;
	char* tokensfilename;


	int stacksize;
	int memsize;

	int semcheck;
	int interpret;

	char* source;
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
	  know is a pointer to our arguments structure. */
	struct arguments *arguments = (struct arguments*)state->input;

	switch (key)
	{
		case 'x':
			arguments->xml = 1;
			if(arg != NULL){
				arguments->xmlfilename = arg;
			}
			break;
		case 't':
			arguments->tokens = 1;
			if(arg != NULL){
				arguments->tokensfilename = arg;
			}
			break;
		case 'm':
			arguments->memsize = atoi(arg);
			break;
		case 's':
			arguments->stacksize = atoi(arg);
			break;
		case 'i':
			arguments->interpret = 1;
			break;
		case 'c':
			arguments->semcheck = 1;
			break;
		case ARGP_KEY_ARG:
			if(state->arg_num >= 1){
				argp_usage(state);
			}
			arguments->source = arg;
			break;
		case ARGP_KEY_END:
			if (state->arg_num < 1){
				argp_usage(state);
			}
			break;

		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };

int main (int argc, char **argv)
{
	struct arguments arguments;

	/* Default values. */
	arguments.xml = 0;
	arguments.tokens = 0;
	arguments.interpret = 0;
	arguments.semcheck = 0;
	arguments.memsize = 128;
	arguments.stacksize = 16;
	arguments.source = NULL;
	arguments.xmlfilename = NULL;
	arguments.tokensfilename = NULL;

     
	/* Parse our arguments; every option seen by parse_opt will
	  be reflected in arguments. */
	argp_parse (&argp, argc, argv, 0, 0, &arguments);



	//fprintf(stdout,"source=%s xml=%s tokens=%s\n",arguments.source, arguments.xmlfilename, arguments.tokensfilename);

	
	FILE *in = fopen(arguments.source,"r");
	set_scanner_input(in);
	if(arguments.tokens){
		FILE* tokenfile = stdout;
		if(arguments.tokensfilename != NULL){
			tokenfile = fopen(arguments.tokensfilename,"w");
		}
		set_token_output(tokenfile);
	}
	struct ast* program = parse_all();

	if(arguments.xml){
		FILE* xmlfile = stdout;
		if(arguments.xmlfilename != NULL){
			xmlfile=  fopen(arguments.xmlfilename,"w");
		}
		ast2xml(xmlfile,program,0);
	}

	if(arguments.semcheck){
		semcheck(program,program);
	}

	if(arguments.interpret){
		run(program,arguments.stacksize,arguments.memsize);
	}

	exit(0);
}
