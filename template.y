%{
#include <stdio.h>
#include "tree.h"
#include "directives.h"
  
tree_t *root;
 
%}
%union {
   operation_t *op;
   tree_t *tree;
   char *string;
};

%token <string> LIT_OPEN;
%token <string> DIRECTIVE;
%token <string> IDENTIFIER;
%token <string> DATA;
%token <string> END_TOK;
%token FILTER_OP;
%type <tree> filter 
%type <tree> literal
%type <tree> command
%type <tree> commands
%%

commands: /* empty */
	| commands command    { add_op($2, $1); root = $1; $$ = $1; }

command: literal              { $$ = $1; }
       | filter 	      { $$ = $1; }
       | IDENTIFIER           { $$ = bare_identifier($1) }
       ;
filter: IDENTIFIER FILTER_OP IDENTIFIER /* plain [% something | filter %] */
	{ $$ = identifier_filter($1, $3); }
      ;

literal:      /* literal [% */
	  LIT_OPEN { $$ = optree(op_string(OP_LITECHO, "[%")); };
              /* text to echo */
	| DATA     { $$ = optree(op_string(OP_LITECHO, $1));   };

%%

int main (int argc, char **argv)
{
  root = malloc(sizeof(struct tree_t));
  if(root == 0){
     fprintf(stderr, "Couldn't malloc memory for the parsetree.\n");
     exit(1);
  }
  yydebug = 0;
  printf("Starting...\n");
  yyparse();
  dumptree(root, 0);			
}

int yyerror(char *error)
{
  fprintf(stderr, "error: %s\n*** Stop.\n", error);
}
