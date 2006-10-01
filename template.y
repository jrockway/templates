%{
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "directives.h"
  
tree_t *root;
%}

%union {
   struct operation_t *op;
   struct tree_t *tree;
   char *string;
};

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

commands:		      {  }
	| commands command    { $1; add_op(root, $2) }

command: literal              { $$ = $1; }
       | filter 	      { $$ = $1; }
       | IDENTIFIER           { $$ = bare_identifier($1) }
       ;
filter: IDENTIFIER FILTER_OP IDENTIFIER /* plain [% something | filter %] */
	{ $$ = identifier_filter($1, $3); }
      ;

literal: DATA		       { $$ = optree(op_string(OP_ECHO, $1));};
	
%%

int main (int argc, char **argv)
{
  operation_t *start = malloc(sizeof(operation_t));
  argument_t *filename = malloc(sizeof(argument_t));

  if(start == NULL || filename == NULL){
     fprintf(stderr, "Couldn't malloc memory for the parse tree.\n");
     exit(1);
  }
  filename->type = T_STRING;
  filename->data.STRING = "standard input";
  start->opcode = OP_START;
  start->arguments = filename;
  root = optree(start);
  root->last = root; /* point to itself to start */
  
  yydebug = 0;	
  printf("Starting...\n");
  yyparse();
  dumptree(root, 0);	
  return 0;	
}

int yyerror(char *error)
{
  fprintf(stderr, "error: %s\n*** Stop.\n", error);
}
