all: template

y.tab.h: template.y
	bison -t -d template.y --verbose

template.tab.c: template.y tree.h
	bison -t -d template.y --verbose

lex.yy.c: template.l y.tab.h
	flex -d -f -v template.l

tree.o: tree.c tree.h
	cc -o tree.o tree.c

template: lex.yy.c template.tab.c tree.o
	cc -o template tree.o lex.yy.c template.tab.c -lfl

clean:
	rm -f template
	rm -f template.exe
	rm -f lex.yy.c
	rm -f template.tab.h
	rm -f template.output
	rm -f y.tab.h
	rm -f template.tab.c
	rm -f tree.o
