all: build

build:
	yacc -d -Wcounterexamples parser.y
	flex lex.l
	gcc lex.yy.c y.tab.c -o lex

clean:
	rm -f lex.yy.c lex y.tab.c y.tab.h

.PHONY:	clean