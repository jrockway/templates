#include "tree.h"
#include "directives.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* non-block directives that accept an identifier:
   INCLUDE filtename
*/
tree_t *identifier_directive(char *directive, char *identifier)
{
  argument_t *idf = malloc(sizeof(argument_t));
  idf->data.STRING = strdup(identifier);
  idf->type = T_STRING;

  operation_t *op = malloc(sizeof(argument_t));
  op->opcode = OP_INCLUDE;
  op->arguments = idf;

  if(strcmp(directive, "INCLUDE") != 0)
    {
      printf("error: unknown directive %s\n", directive);
      exit(1);
    }
  
  return optree(op);
}


tree_t *identifier_filter(char *identifier, char *filter)
{
  argument_t *fname = malloc(sizeof(argument_t));
  fname->data.STRING = strdup(filter);
  fname->type = T_STRING;

  argument_t *iname = malloc(sizeof(argument_t));
  iname->data.STRING = strdup(identifier);
  iname->type = T_STRING;
  
  operation_t *get = malloc(sizeof(argument_t));
  get->opcode = OP_GET;
  get->arguments = iname;

  operation_t *fo = malloc(sizeof(operation_t));
  fo->opcode = OP_FILTER;
  fo->arguments = fname;

  tree_t *ftree = optree(fo);
  ftree->child  = optree(get);
  return ftree;
}

tree_t *chained_filter(tree_t *child_filter, char *filter)
{
  argument_t *fname = malloc(sizeof(argument_t));
  fname->data.STRING = strdup(filter);
  fname->type = T_STRING;

  operation_t *fo = malloc(sizeof(operation_t));
  fo->opcode = OP_FILTER;
  fo->arguments = fname;

  tree_t *ftree = optree(fo);
  ftree->child  = child_filter;
  return ftree;
}


tree_t *bare_identifier(char *identifier)
{
  argument_t *f = malloc(sizeof(argument_t));
  f->data.STRING = identifier;
  f->type = T_STRING;

  operation_t *get = malloc(sizeof(operation_t));
  get->opcode = OP_GET;
  get->arguments = f;
  
  operation_t *echo = malloc(sizeof(operation_t));
  echo->opcode = OP_ECHO;
  echo->arguments = NULL;
  
  tree_t *echotree = optree(echo);
  echotree->child = optree(get);
  return echotree;
}

tree_t *echo_tree(tree_t *tree)
{
  operation_t *op = malloc(sizeof(operation_t));
  op->opcode = OP_ECHO;
  op->arguments = NULL;

  tree_t *new_tree = optree(op);
  new_tree->child = tree;
  
  return new_tree;
}

