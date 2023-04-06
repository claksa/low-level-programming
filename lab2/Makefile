all: build

run:
	./main

build:
	yacc -d -Wcounterexamples parser.y
	flex lex.l
	gcc main.c decl.c lex.yy.c y.tab.c -o main

clean:
	rm -f lex.yy.c main y.tab.c y.tab.h

.PHONY:	clean