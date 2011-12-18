
CC = gcc
# debug ccflags
CCFLAGS = -std=c99 -O0 -Wall -pedantic -g
# prod ccflags
#CCFLAGS = -std=c99 -O2 -Wall -pedantic

yall: lex.yy.c parser.tab.c main.c ast.o err.o utilz.o
	${CC} -O2 -o $@ parser.tab.c lex.yy.c ast.o err.o utilz.o main.c -lfl

ast.o: ast.h ast.c
	${CC} ${CCFLAGS} -c -o ast.o ast.c

err.o: err.h err.c
	${CC} ${CCFLAGS} -c -o err.o err.c

utilz.o: utilz.c
	${CC} ${CCFLAGS} -c -o utilz.o utilz.c

lex.yy.c: lexer.lex
	flex lexer.lex

parser.tab.c: parser.y
	bison -d parser.y

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c *.o yall