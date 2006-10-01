#include "tree.h"
#include <stdio.h>

inline void add_op(struct tree_t *tree, struct tree_t *new)
{
  //  printf("adding an operation to the tree: %x <- %x\n", tree, new);
  if(tree == NULL){
    printf("NULL ADDITION TO TREE\n");
    return;
  }
  
  /* tack on "new" as the last peer of the root */
  if(tree->last != NULL) tree->last->peer = new;
  else tree->peer = new;
  tree->last = new;
}

inline struct operation_t *op_string(int opcode, char *string)
{
  struct argument_t  *a = malloc(sizeof(struct argument_t));
  struct operation_t *o = malloc(sizeof(struct operation_t));
  a->type = T_STRING;
  a->data.STRING = strdup(string); /* XXX: dup? */
  a->next = NULL;
  o->opcode = opcode;
  o->arguments = a;
  return o;
}

struct tree_t *optree(struct operation_t *op)
{
  struct tree_t *t = malloc(sizeof(struct tree_t));
  t->op = op;
  t->last = NULL;
  t->child = NULL;
  t->peer = NULL;
  return t;
}

void dumptree (struct tree_t *tree, int level)
{
  /* make sure you keep this sync'd with tree.h's defines */
  const char *operations[10] = {"NULL","GET","SET","LITECHO","GET+ECHO",
			       "LOOP","TEST","INCLUDE","WRAP","FILTER"};
  char *indent = malloc(2*level+1);
  int i;
  struct operation_t *op;
  struct argument_t  *argument;
  type_t type;

  for(i = 0; i < 2*level; i++) strncat(indent, "  ", 2*level-i);
  
  while(tree != NULL){
    /* print opcode */
    op = tree->op;
    if(op != NULL){
      printf("%sOP: %s (%d)\n", indent, operations[op->opcode], op->opcode);
      
      /* print arguments */
      argument = op->arguments;
      while(argument != NULL){
	if(argument->type == T_STRING){
	  printf("%s ARG: <STRING>%s\n", indent, argument->data.STRING);
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
