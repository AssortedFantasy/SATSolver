
# Temporary makefile for testing things

all: bison flex
	mv parse.tab.h ../../include/
	mv lex.yy.c lex.yy.cc
	mv parse.tab.c parse.tab.cc

clean:
	rm -f parse.tab.* lex.* out

bison:
	bison -d parse.y

flex:
	flex parse.l