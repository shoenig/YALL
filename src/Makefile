
CC = gcc
# debug ccflags
CCFLAGS = -std=c99 -O0 -Wall -pedantic -g
# prod ccflags
#CCFLAGS = -std=c99 -O2 -Wall -pedantic


yall: lex.yy.c parser.tab.c main.c ast.o err.o utilz.o builtins.o symtable.o typedecoder.o list.o stringer.o ufunc.o functable.o globals.h
	${CC} -O2 -o $@ parser.tab.c lex.yy.c ast.o err.o utilz.o builtins.o symtable.o typedecoder.o list.o stringer.o ufunc.o functable.o main.c -lfl -lm
	mv yall ../

ast.o: ast.h builtintypes.h ast.c
	${CC} ${CCFLAGS} -c -o ast.o ast.c

err.o: err.h err.c
	${CC} ${CCFLAGS} -c -o err.o err.c

utilz.o: utilz.h utilz.c
	${CC} ${CCFLAGS} -c -o utilz.o utilz.c

builtins.o: builtins.h builtins.c
	${CC} ${CCFLAGS} -c -o builtins.o builtins.c

symtable.o: symbol.h symtable.h symtable.c builtintypes.h
	${CC} ${CCFLAGS} -c -o symtable.o symtable.c

typedecoder.o: typedecoder.h typedecoder.c
	${CC} ${CCFLAGS} -c -o typedecoder.o typedecoder.c

list.o: list.h list.c
	${CC} ${CCFLAGS} -c -o list.o list.c

stringer.o: stringer.h stringer.c
	${CC} ${CCFLAGS} -c -o stringer.o stringer.c

ufunc.o: ufunc.h ufunc.c
	${CC} ${CCFLAGS} -c -o ufunc.o ufunc.c

functable.o: functable.h functable.c
	${CC} ${CCFLAGS} -c -o functable.o functable.c

lex.yy.c: lexer.lex
	flex lexer.lex

parser.tab.c: parser.y globals.h
	bison -d parser.y

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c *.o yall