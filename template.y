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
   int *xop;
};

%error-verbose
%token <string> DIRECTIVE "inline directive"; 
%token <string> WRAP "WRAP directive";
%token <string> IDENTIFIER "identifier";
%token <string> DATA "text";
%token <string> END "END tag";
%token <string> IF "IF statement";
%token <string> ELSE "ELSE statement";
%token <string> ELSIF "ELSIF statement";
%token <string> LOOP "LOOP statement";
%token <string> SET "SET statement";
%token <string> E "%]";
%token <string> STRING "quoted string literal";

%token LT "less than operator";
%token GT "greater than operator";
%token LTEQ "less than / eqal to operator";
%token GTEQ "greater than / equal to operator";
%token EQ "equals operator";
%token AND "logical and operator";
%token OR "logical or operator";
%token NOT "logical not operator";
%token ADD "addition operator";
%token SUB "subtraction operator";
%token INC "increment operator";
%token DEC "decrement operator";
%token ASSIGN "assignment";
%token COMMAND "command"; /* indicates start of command */
%token FILTER_OP "|";
%token FILE_END 0 "end of file"

%left LT GT EQ LTEQ GTEQ;
%left DEC INC;
%left ADD SUB;
%right AND OR;
%right ASSIGN;

%type <tree> filter 
%type <tree> block
%type <tree> else
%type <tree> commands
%type <tree> expr
%type <tree> string
%type <string> error
%%

commands:		      {  $$ = NULL }
	| commands block      {  if($1 != NULL){ add_op($1, $2); 
                                        result = $1; $$ = $1;} 
				 else { result = $2; $$ = $2;} }
        ;

expr:     IDENTIFIER                  { $$ = identifier($1); }
        | string		      { $$ = $1}
        | INC IDENTIFIER              { $$ = NULL; }
        | DEC IDENTIFIER 	      { $$ = NULL; }
        | expr ADD expr		      { $$ = expr($1, X_ADD, $3) }
	| expr SUB expr		      { $$ = expr($1, X_SUB, $3) }
        | expr LT expr		      { $$ = expr($1, X_LT, $3)  }
	| expr GT expr		      { $$ = expr($1, X_GT, $3)  }
	| expr EQ expr		      { $$ = expr($1, X_EQ, $3)  }
	| expr LTEQ expr	      { $$ = expr($1, X_LTEQ, $3)}
	| expr GTEQ expr	      { $$ = expr($1, X_GTEQ, $3)}
	| expr AND expr		      { $$ = expr($1, X_AND, $3) }
	| expr OR expr		      { $$ = expr($1, X_OR, $3)  }
	;

string:   string STRING               { $$ = string($1, $2)   }
	| STRING 		      { $$ = string(NULL, $1) }
        ;


block:   DATA			      { $$ = optree(op_string(OP_ECHO, $1));};
       | filter E	      	      { $$ = echo_tree($1); }
       | IDENTIFIER E          	      { $$ = echo_tree(identifier($1)) }
       | DIRECTIVE IDENTIFIER E	      { $$ = identifier_directive($1, $2) }
       | DIRECTIVE E           	      { $$ = echo_tree(identifier($1)) /* ? */ }
       | IDENTIFIER ASSIGN expr E     { $$ = set($1, $3); }
       | SET IDENTIFIER ASSIGN expr E { $$ = set($2, $4); }
       | LOOP expr E commands END E   { $$ = loop($2, $4) }
       | LOOP IDENTIFIER ASSIGN IDENTIFIER E commands END E
       	      		 	      { $$ = assign_loop($2, $4, $6); }
       | IF expr E commands END E     { $$ = if_then($2, $4, NULL); }
       | IF expr E commands else END E
       	    	   	      	      { $$ = if_then($2, $4, $5);}
       | WRAP IDENTIFIER E commands END E
       	 	 	   	      { $$ = NULL; printf("a wrap!\n") }
       ;

else:    ELSIF expr E commands        { $$ = if_then($2, $4, NULL); } 
       | ELSIF expr E commands else   { $$ = if_then($2, $4, $5); } /* shift/reduce */
       | ELSE E commands              { $$ = $3; }
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
  filename->data.string = strdup("standard input");
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
  dumptree(result, 0);
  exit(1);
}
