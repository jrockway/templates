all: template

y.tab.h: template.y tree.h
	bison -t -d template.y --verbose

template.tab.c: template.y tree.h
	bison -t -d template.y --verbose

lex.yy.c: template.l y.tab.h
	flex -d -v template.l

tree.o: tree.c tree.h
	cc -c tree.c -o tree.o -g3

directives.o: directives.c directives.h tree.h
	cc -c directives.c -o directives.o -g3

template: lex.yy.c template.tab.c tree.o directives.o
	cc -o template tree.o directives.o lex.yy.c template.tab.c -lfl -g3

clean:
	rm -f template
	rm -f template.exe
	rm -f template.exe.stackdump
	rm -f lex.yy.c
	rm -f template.tab.h
	rm -f template.output
	rm -f y.tab.h
	rm -f template.tab.c
	rm -f tree.o
	rm -f directives.o
