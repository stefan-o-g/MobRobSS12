MAKEFLAGS += -r

CC = gcc
LEX = lex

CFLAGS = -g -lm

OBJS = parser.o lex_scanner.o semcheck.o interpreter.o token.o ast.o utils.o stack.o memory.o native_functions.o main.o 


PROG = "func"


all: $(PROG)

$(PROG) : scanner_source $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG)

%.o : %.c
	$(CC) -c $(CFLAGS) $*.c


scanner_source : lex_scanner.l
	$(LEX) -o lex_scanner.c lex_scanner.l


clean : 
	rm -f $(OBJS) lex_scanner.c


