#include "tree.h"
#include "directives.h"
#include <stdlib.h>
#include <string.h>

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

