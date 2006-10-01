#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inline void add_op(tree_t *tree, tree_t *new)
{
  //  printf("adding an operation to the tree: %x <- %x\n", tree, new);
  if(tree == NULL){
    printf("** ADDITION TO NULL TREE!!\n");
    return;
  }
  
  /* tack on "new" as the last peer of the root */
  if(tree->last != NULL) tree->last->peer = new;
  else tree->peer = new;
  tree->last = new;
}

inline operation_t *op_string(int opcode, char *string)
{
  argument_t  *a = malloc(sizeof( argument_t));
  operation_t *o = malloc(sizeof( operation_t));
  a->type = T_STRING;
  a->data.STRING = strdup(string); /* XXX: dup? */
  a->next = NULL;
  o->opcode = opcode;
  o->arguments = a;
  return o;
}

inline tree_t *optree(operation_t *op)
{
  tree_t *t = malloc(sizeof( tree_t));
  t->op = op;
  t->last = NULL;
  t->child = NULL;
  t->peer = NULL;
  t->alter = NULL;
  return t;
}

void dumptree (tree_t *tree, int level)
{
  /* make sure you keep this sync'd with tree.h's defines */
  const char *operations[12] = {"NOP","GET","SET","ECHO","!!",
				"LOOP","BRANCH","INCLUDE","WRAP","FILTER",
				"START", "BAIL"};
  char *indent = malloc(2*level+1);
  int i;
  operation_t *op;
  argument_t  *argument;
  type_t type;

  for(i = 0; i < 2*level; i++) indent[i] = ' ';
  indent[2*level] = '\0';
  
  while(tree != NULL){
    /* print opcode */
    op = tree->op;
    if(op != NULL){
      printf("%sOP: %s (%d) at 0x%x\n", 
	     indent, operations[op->opcode], op->opcode, tree);
      
      /* print arguments */
      argument = op->arguments;
      while(argument != NULL){
	if(argument->type == T_STRING){
	  printf("%s ARG: <STRING> ``%s''\n", indent, argument->data.STRING);
	}
	else if(argument->type == T_INT){
	  printf("%s ARG: <INT>%d\n", indent, argument->data.INT);
	}
	else if(argument->type == T_DOUBLE){
	  printf("%s ARG: <DOUBLE>%f\n", indent, argument->data.DOUBLE);
	}
	else if(argument->type == T_VOID){
	  printf("%s ARG: <VOID>\n", indent);
	}
	else {
	  printf("%s ARG: [NULL]\n", indent);
	}
	argument = argument->next;
      }
    }
    /* no opcode !? */
    else {
      printf("%sOP: NULL (NULL)\n", indent);
    }
    
    /* recurse to next child */
    dumptree(tree->child, level + 1);
    
    /* iterate to next peer */
    tree = tree->peer;
  }
  
  free(indent);
}
