#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inline void add_op(tree_t *tree, tree_t *new)
{
  //  printf("adding an operation to the tree: t0x%x <- t0x%x\n", tree, new);
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

inline argument_t *argument()
{
  argument_t *a = malloc(sizeof(argument_t));
  a->type = T_NULL;
  a->data.INT = 0;
  a->next = NULL;
  return a;
}


inline tree_t *optree(operation_t *op)
{
  tree_t *t = malloc(sizeof( tree_t));
  t->op = op;
  t->last = t;
  t->child = NULL;
  t->peer = NULL;
  t->alter = NULL;
  t->cond = NULL;
  return t;
}

void _destroy_op(operation_t *op)
{
  op->opcode = 0;
  argument_t *a = op->arguments;
  while(a != NULL)
    {
      argument_t *next = a->next;
      if( a->data.STRING != NULL && 
	 (a->type == T_STRING || a->type == T_SYMBOL))
	{
	  free(a->data.STRING);
	}
      
      free(a);
      a = next;
    }
  
  free(op);
  
}

void destroy(tree_t *tree)
{
  if (tree == NULL)  return;
  destroy(tree->cond);
  destroy(tree->child);
  destroy(tree->alter);
  destroy(tree->peer);
  _destroy_op(tree->op);
  free(tree);
}

void dumptree (tree_t *tree, int level)
{
  if (level > 50) return;
  
  /* make sure you keep this sync'd with tree.h's defines */
  const char *operations[13] = {"NOP","GET","SET","ECHO","!!",
				"LOOP","BRANCH","INCLUDE","WRAP","FILTER",
				"START", "BAIL", "EXPR"};
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
      printf("%sOP: %s (%d) at t0x%x\n", 
	     indent, operations[op->opcode], op->opcode, tree);
      
      /* print arguments */
      argument = op->arguments;
      while(argument != NULL){
	switch (argument->type)
	  {
	  case T_STRING:
	    printf("%s ARG: <STRING> ``%s''\n", indent, argument->data.STRING);
	    break;
	  case T_SYMBOL:
	    printf("%s ARG: <SYMBOL> ``%s''\n", indent, argument->data.STRING);
	    break;
	  case T_INT:
	    printf("%s ARG: <INT>%d\n", indent, argument->data.INT);
	    break;
	  case T_DOUBLE:
	    printf("%s ARG: <DOUBLE>%f\n", indent, argument->data.DOUBLE);
	    break;
	  case T_VOID:
	    printf("%s ARG: <VOID>\n", indent);
	    break;
	  default:
	    printf("%s ARG: [NULL]\n", indent);
	  }
	argument = argument->next;
      }
    }
    /* no opcode !? */
    else {
      printf("%sOP: NULL (NULL)\n", indent);
    }

    /* special opcodes */
    switch(op->opcode)
      {
      case OP_BRANCH:
	/* branch */
	printf("%s COND: t0x%x\n", indent, tree->R_COND);
	dumptree(tree->R_COND, level + 1);
	printf("%s TRUE: t0x%x\n", indent, tree->R_TRUE);
	dumptree(tree->R_TRUE, level + 2);
	printf("%s FALSE: t0x%x\n", indent, tree->R_FALSE);
	dumptree(tree->R_FALSE, level + 2);
	break;     
      case OP_LOOP:
	/* loop */
	printf("%s COND: t0x%x\n", indent, tree->R_COND);
	dumptree(tree->R_COND, level + 1);
	printf("%s ACTION: t0x%x\n", indent, tree->R_TRUE);
	dumptree(tree->R_ACTION, level + 2);
	break;
      case OP_EXPR:
	/* expression */
	printf("%s LHS: t0x%x\n", indent, tree->R_LHS);
	dumptree(tree->R_LHS, level + 2);
	printf("%s RHS: t0x%x\n", indent, tree->R_RHS);
	dumptree(tree->R_RHS, level + 2);
	break;

      default:
	/* everything else */
	dumptree(tree->child, level + 1);
	break;
      }
    
    /* iterate to next peer */
    tree = tree->peer;
  }
  
  free(indent);
}
