#TODO: separate out the build process

yall: lex.yy.c parser.tab.c main.c ast.o err.o utilz.o
	gcc -O0 -g -o $@ parser.tab.c lex.yy.c ast.o err.o utilz.o main.c -lfl

ast.o: ast.h ast.c
	gcc -std=c99 -Wall -pedantic -O0 -g -c -o ast.o ast.c

err.o: err.h err.c
	gcc -std=c99 -Wall -pedantic -O0 -g -c -o err.o err.c

utilz.o: utilz.c
	gcc -std=c99 -Wall -pedantic -O0 -g -c -o utilz.o utilz.c

lex.yy.c: lexer.lex
	flex lexer.lex

parser.tab.c: parser.y
	bison -d parser.y

clean:
	rm parser.tab.c parser.tab.h lex.yy.c *.o