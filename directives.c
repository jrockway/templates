#include "tree.h"
#include "directives.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

tree_t *error(char *message, int line)
{
  argument_t *err = argument();
  err->data.string = strdup(message);
  err->type = T_STRING;
  
  argument_t *lin = argument();
  lin->data.number = line;
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
  idf->data.string = strdup(identifier);
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
  fname->data.string = strdup(filter);
  fname->type = T_STRING;

  argument_t *iname = argument();
  iname->data.string = strdup(identifier);
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
  fname->data.string = strdup(filter);
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
  f->data.string = identifier;
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

tree_t *expr(tree_t *left, int operator, tree_t *right)
{
  argument_t *o = argument();
  o->data.number = operator;
  o->type = T_INT;
  
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
  src->data.string = strdup(source); /* XXXXX actual operator */
  src->type = T_STRING;

  argument_t *snk = argument();
  snk->data.string = strdup(sink); /* XXXXX actual operator */
  snk->type = T_STRING;
  src->next = snk;
  
  operation_t *op = malloc(sizeof(operation_t));
  op->opcode    = OP_LOOP;
  op->arguments = src;
  
  tree_t *loop = optree(op);
  loop->R_ACTION  = action;
  
  return loop;
}

tree_t *set(char *symbol, tree_t *value){
  argument_t *symname = argument();
  symname->type = T_SYMBOL;
  symname->data.string = strdup(symbol);
  
  operation_t *op = malloc(sizeof(operation_t));
  op->opcode    = OP_SET;
  op->arguments = symname;
 
  tree_t *result = optree(op);
  result->R_EXPR = value;
  
  return result;
}

  
/* first must be a "get string" op! */
tree_t *string(tree_t *a, char *b)
{
  tree_t *result;
  
  if(a == NULL)
    {
      /* create a new string */
      argument_t *s = argument();
      s->type = T_STRING;
      s->data.string = strdup(b);
      
      operation_t *op = malloc(sizeof(operation_t));
      op->opcode = OP_GET;
      op->arguments = s;
      
      result = optree(op);
    }
  else
    {
      // blow up catch on fire, i hope this is really a T_STRING!
      char *start = a->op->arguments->data.string;
      int size = strlen(start) + strlen(b);
      
      char *str = malloc(size * sizeof(char));
      if (str == NULL) return NULL;
      strncpy(str, start, size);
      strncat(str, b, size-strlen(start));
      str[size-1] = '\000'; /* i wish strl* were portable */
      
      a->op->arguments->data.string = str;
      result = a;
    }
  
  return result;
}
