
CC = gcc
# debug ccflags
CCFLAGS = -std=c99 -O0 -Wall -pedantic -g
# prod ccflags
#CCFLAGS = -std=c99 -O2 -Wall -pedantic

yall: lex.yy.c parser.tab.c main.c ast.o err.o utilz.o builtins.o globals.h
	${CC} -O2 -o $@ parser.tab.c lex.yy.c ast.o err.o utilz.o builtins.o main.c -lfl -lm

ast.o: ast.h builtintypes.h ast.c
	${CC} ${CCFLAGS} -c -o ast.o ast.c

err.o: err.h err.c
	${CC} ${CCFLAGS} -c -o err.o err.c

utilz.o: utilz.h utilz.c
	${CC} ${CCFLAGS} -c -o utilz.o utilz.c

builtins.o: builtins.h builtins.c
	${CC} ${CCFLAGS} -c -o builtins.o builtins.c

lex.yy.c: lexer.lex
	flex lexer.lex

parser.tab.c: parser.y globals.h
	bison -d parser.y

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c *.o yall