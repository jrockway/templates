%{
#include <stdio.h>
#include "tree.h"
t_tree root; /* parse tree */
root.operation = "ROOT";
%}

%%
%token LIT_OPEN;
%token DIRECTIVE;
%token IDENTIFIER;
%token DATA;
%token END_TOK;

program: | program statement;
statement: block 
	
	 ;

block: command literal end { printf("BLOCK\n"); }
     | command { printf("FAKE BLOCK\n"); }
     ;	    
command: filter 
       | IDENTIFIER { $$ = printf("IDENTIFIER\n"); } 
       | DIRECTIVE { $$ = printf("DIRECTIVE\n"); }
       | DIRECTIVE IDENTIFIER { $$ = printf("DIRECTIVE ON IDENTIFIER\n"); }
       ;
filter: IDENTIFIER '|' DATA { printf("Hey, a filter!\n") }
      ;
end: DIRECTIVE END_TOK { $$ = printf("END BLOCK\n"); }
   ;

literal: LIT_OPEN | DATA { $$ = $1 };

%%

int main (int argc, char **argv)
{
  yydebug = 1;
  printf("Starting...\n");
  yyparse();
}

int yyerror(char *error)
{
  fprintf(stderr, "error: %s\n*** Stop.\n", error);
}
