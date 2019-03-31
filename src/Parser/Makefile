
# Temporary makefile for testing things

all: bison flex
	g++ parse.tab.c lex.yy.c -o out

clean:
	rm -f parse.tab.* lex.* out

bison:
	bison -d parse.y

flex:
	flex parse.l