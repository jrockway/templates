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
%token <string> BDIRECTIVE;
%token <string> E;
%token COMMAND; /* indicates start of command */
%token FILTER_OP;
%type <tree> filter 
%type <tree> block
%type <tree> commands
%%

commands:		      {  }
	| commands block      { $1; if($2 != NULL) add_op(root, $2) }
        ;

block:   DATA			      { $$ = optree(op_string(OP_ECHO, $1));};
       | filter E	      	      { $$ = echo_tree($1); }
       | IDENTIFIER E          	      { $$ = bare_identifier($1) }
       | DIRECTIVE IDENTIFIER E	      { $$ = identifier_directive($1, $2) }
       | DIRECTIVE E           	      { $$ = bare_identifier($1) /* ? */ }
       | BDIRECTIVE IDENTIFIER E block END_TOK E
       	 	 	   	      { $$ = NULL; printf("a block!\n") }
       | BDIRECTIVE E block END_TOK E { $$ = NULL; printf("a block!\n") }
       ;

filter: IDENTIFIER FILTER_OP IDENTIFIER 
			      /* plain [% something | filter %] */
		   	      { $$ = identifier_filter($1, $3); }
      | filter FILTER_OP IDENTIFIER /* chained filters */
               		      { $$ = chained_filter($1, $3); }
      ;


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
  
  yydebug = 1;	
  printf("Starting...\n");
  yyparse();
  dumptree(root, 0);	
  return 0;	
}

int yyerror(char *error)
{
  fprintf(stderr, "error: %s\n*** Stop.\n", error);
}
