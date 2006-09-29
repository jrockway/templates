#include "tree.h"
#include <stdio.h>

void dumptree (struct tree_t *tree, int level)
{
  char *indent = malloc(2*level+1);
  int i;
  struct operation_t *op;
  struct argument_t  *argument;
  type_t type;

  for(i = 0; i < 2*level; i++) strncat(indent, "  ", 2*level-i);
  
  while(tree != NULL){
    /* print opcode */
    op = &tree->op;
    printf("%sOP: %s (%d)\n", indent, op->name, op->opcode);
    
    /* print arguments */
    argument = &op->arguments;
    while(argument != NULL){
      if(argument->type == T_STRING){
	printf("%s ARG: <STRING>%s\n", indent, (char *) argument->data);
      }
      else if(argument->type == T_INT){
	printf("%s ARG: <INT>%d\n", indent, *((int *)argument->data));
      }
      else if(argument->type == T_DOUBLE){
	printf("%s ARG: <DOUBLE>%f\n", indent, *((double *)argument->data));
      }
      else if(argument->type == T_VOID){
	printf("%s ARG: <VOID>\n", indent);
      }
      else {
	printf("%s ARG: [NULL]\n", indent);
      }   
      argument = argument->next;
    }
    
    dumptree(tree->child, level + 1);
    tree = tree->peer;
  }
  free(indent);
}
