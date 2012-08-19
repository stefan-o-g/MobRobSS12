/*** Definition section ***/

%{

#include <stdio.h>
#include "token.h"
#include "scanner.h"


int lineno = 1;
int columnno = 1;


FILE* token_out_file = NULL;

struct token last_token;

void set_token_output(FILE* file){
	token_out_file = file;
}

struct token* next_token(){
	
	int t = yylex();
	
	
	if(t == 0){
	 	last_token.type = T_EOF;
	 	last_token.line = -1;
	 	last_token.column = -1;
	 	last_token.text = "";
	}
	
	if(token_out_file != NULL){
		print_token_bot(token_out_file,&last_token);
	}
	
	
	return &last_token;
}

struct token* get_token(){
	return &last_token;
}




void update_column(){
	int length = strlen(yytext);
	columnno += length;
}

int token(enum token_type t){
	last_token.type = t;
	last_token.line = lineno;
	last_token.column = columnno;
	last_token.text = yytext;
	update_column();
	return t;
}








%}

%option noyywrap
%option yylineno


%%




"+"				{  return token(T_PLUS); }
"-"				{  return token(T_MINUS); }
"*"				{  return token(T_MUL); }
"/"				{  return token(T_DIV); }
"%"				{  return token(T_MOD); }

"="				{  return token(T_BECOMES); }

"("				{  return token(T_LPAR); }
")"				{  return token(T_RPAR); }

"<"				{  return token(T_LT);	}
">"				{  return token(T_GT);	}
"<="				{  return token(T_LE);	}
">="				{  return token(T_GE);	}
"!="				{  return token(T_NE);	}
"=="				{  return token(T_EQ);	}

";"				{  return token(T_SEMICOLON); }
"."				{  return token(T_DOT); }
","				{  return token(T_COMMA); }

"?"				{  return token(T_QUEST); }
"!"				{  return token(T_NOT); }

":"				{  return token(T_COLON); }

"while"				{  return token(T_WHILE); }
"do"				{  return token(T_DO); }

"vars"				{  return token(T_VARS); }
"consts"			{  return token(T_CONSTS); }

"if"				{  return token(T_IF);	}
"then"				{  return token(T_THEN); }
"else"				{  return token(T_ELSE); }

"{"				{  return token(T_BEGIN); }
"}"				{  return token(T_END); }


"func" 				{  return token(T_FUNCTION); }
"native"			{  return token(T_NATIVE); }

[0-9]+(\.[0-9]+)		{  return token(T_DOUBLE); }

[0-9]+				{  return token(T_INT); }

[A-Za-z_]([A-Za-z_0-9]{0,99})	{  return token(T_IDENT); }


[ \t]				{
					update_column();
				} /* skip whitespace */ 

\r\n|\r|\n			{ 
					lineno++;
					columnno = 1;
				}

.				{ return token(T_UNKNOWN); }  /* unknown token (error) */
%%


void set_scanner_input(FILE* file){
    yyset_in(file);
}


