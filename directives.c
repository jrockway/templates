#include "tree.h"
#include "directives.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

tree_t *error(char *message, int line)
{
  argument_t *err = argument();
  err->data.STRING = strdup(message);
  err->type = T_STRING;
  
  argument_t *lin = argument();
  lin->data.INT = line;
  lin->type = T_INT;
  err->next = lin;
  
  operation_t *op = malloc(sizeof(operation_t));
  op->opcode = OP_BAIL;
  op->arguments = err;
  
  return optree(op);
}



/* non-block directives that accept an identifier:
   INCLUDE filtename
*/
tree_t *identifier_directive(char *directive, char *identifier)
{
  argument_t *idf = argument();
  idf->data.STRING = strdup(identifier);
  idf->type = T_SYMBOL;

  operation_t *op = malloc(sizeof(operation_t));
  op->opcode = OP_INCLUDE;
  op->arguments = idf;

  if(strcmp(directive, "INCLUDE") != 0)
    {
      char errstr[100];
      snprintf(errstr, 100, "unknown directive %s", directive);
      return error(errstr, 0);
    }
  
  return optree(op);
}


tree_t *identifier_filter(char *identifier, char *filter)
{
  argument_t *fname = argument();
  fname->data.STRING = strdup(filter);
  fname->type = T_STRING;

  argument_t *iname = argument();
  iname->data.STRING = strdup(identifier);
  iname->type = T_SYMBOL;
  
  operation_t *get = malloc(sizeof(operation_t));
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
  argument_t *fname = argument();
  fname->data.STRING = strdup(filter);
  fname->type = T_STRING;

  operation_t *fo = malloc(sizeof(operation_t));
  fo->opcode = OP_FILTER;
  fo->arguments = fname;

  tree_t *ftree = optree(fo);
  ftree->child  = child_filter;
  return ftree;
}


tree_t *identifier(char *identifier)
{
  argument_t *f = argument();
  f->data.STRING = identifier;
  f->type = T_SYMBOL;

  operation_t *get = malloc(sizeof(operation_t));
  get->opcode = OP_GET;
  get->arguments = f;

  return optree(get);
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

tree_t *expr(tree_t *left, char *operator, tree_t *right)
{
  argument_t *o = argument();
  o->data.STRING = strdup(operator); /* XXXXX actual operator */
  o->type = T_STRING;
  
  operation_t *op = malloc(sizeof(operation_t));
  op->opcode    = OP_EXPR;
  op->arguments = o;
  
  tree_t *expr = optree(op);
  expr->R_RHS = right;
  expr->R_LHS = left;
  
  return expr; 
}


tree_t *if_then(tree_t *condition, tree_t *true, tree_t *false)
{
  operation_t *op = malloc(sizeof(operation_t));
  op->opcode    = OP_BRANCH;
  op->arguments = NULL;
  
  tree_t *br = optree(op);
  br->R_COND  = condition;
  br->R_TRUE  = true;
  br->R_FALSE = false;
  
  return br;
}

tree_t *loop(tree_t *condition, tree_t *action)
{
  operation_t *op = malloc(sizeof(operation_t));
  op->opcode    = OP_LOOP;
  op->arguments = NULL;
  
  tree_t *loop = optree(op);
  loop->R_COND  = condition;
  loop->R_ACTION  = action;

  return loop;
}
tree_t *assign_loop(char *sink, char *source, tree_t *action)
{
  argument_t *src = argument();
  src->data.STRING = strdup(source); /* XXXXX actual operator */
  src->type = T_STRING;

  argument_t *snk = argument();
  snk->data.STRING = strdup(sink); /* XXXXX actual operator */
  snk->type = T_STRING;
  src->next = snk;
  
  operation_t *op = malloc(sizeof(operation_t));
  op->opcode    = OP_LOOP;
  op->arguments = src;
  
  tree_t *loop = optree(op);
  loop->R_ACTION  = action;
  
  return loop;
}

