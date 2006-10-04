%{
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "directives.h"
#include <string.h>
  
tree_t *result;
%}

%union {
   struct operation_t *op;
   struct tree_t *tree;
   char *string;
};
%expect 1
%error-verbose
%token <string> DIRECTIVE "inline directive"; 
%token <string> BDIRECTIVE "block directive";
%token <string> IDENTIFIER "identifier";
%token <string> OPERATOR "operator";
%token <string> DATA "text";
%token <string> END "[% END %] tag";
%token <string> IF "if statement";
%token <string> ELSE "else statement";
%token <string> ELSIF "elsif statement";
%token <string> LOOP "loop statement";
%token <string> SET "set statement";
%token <string> E "%]";
%token ASSIGN;
%token COMMAND "command"; /* indicates start of command */
%token FILTER_OP "|";
%token FILE_END 0 "end of file"
%type <tree> filter 
%type <tree> block
%type <tree> else
%type <tree> commands
%type <tree> expr
%type <string> error

%%

commands:		      {  $$ = NULL }
	| commands block      {  if($1 != NULL){ 
	  	   	      	    add_op($1, $2);
				    result = $1; $$ = $1;
				} else { result = $2; $$ = $2;} }
        ;

expr:    IDENTIFIER                    { $$ = identifier($1); }
       | expr OPERATOR expr            { $$ = expr($1, $2, $3) }

block:   DATA			      { $$ = optree(op_string(OP_ECHO, $1));};
       | filter E	      	      { $$ = echo_tree($1); }
       | IDENTIFIER E          	      { $$ = echo_tree(identifier($1)) }
       | DIRECTIVE IDENTIFIER E	      { $$ = identifier_directive($1, $2) }
       | DIRECTIVE E           	      { $$ = echo_tree(identifier($1)) /* ? */ }
       | IDENTIFIER ASSIGN expr E     { $$ = NULL; printf("SET\n"); }
       | SET IDENTIFIER ASSIGN expr E { $$ = NULL; printf("REALSET\n"); }
       | LOOP expr E commands END E   { $$ = loop($2, $4) }
       | LOOP IDENTIFIER ASSIGN IDENTIFIER E commands END E
       	      		 	      { $$ = assign_loop($2, $4, $6); }
       | IF expr E commands END E     { $$ = if_then($2, $4, NULL); }
       | IF expr E commands else
       	    	   	      	      { $$ = if_then($2, $4, $5);}
       | BDIRECTIVE IDENTIFIER E commands END E
       	 	 	   	      { $$ = NULL; printf("a block!\n") }
       | BDIRECTIVE E block END E     { $$ = NULL; printf("a block!\n") }
       ;
else:    ELSIF expr E commands END E  { $$ = if_then($2, $4, NULL); } 
       | ELSIF expr E commands else END E  
       	       	      	       	      { $$ = if_then($2, $4, $5); }
       | ELSE E commands END E        { $$ = $3; }
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
  argument_t *filename = argument();

  if(start == NULL || filename == NULL){
     fprintf(stderr, "Couldn't malloc memory for the parse tree.\n");
     exit(1);
  }
  filename->type = T_STRING;
  filename->data.STRING = strdup("standard input");
  start->opcode = OP_START;
  start->arguments = filename;
  tree_t *root = optree(start);
  root->last = root; /* point to itself to start */
  
  yydebug = argc-1;	
  printf("Starting...\n");
  yyparse();
  add_op(root, result);
  dumptree(root, 0);
  destroy(root);	
  return 0;	
}

int yyerror(char *error)
{
  fprintf(stderr, "fatal error: %s\n", error);
  exit(1);
}
