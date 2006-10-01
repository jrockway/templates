#include "tree.h"
#include "directives.h"

struct tree_t *identifier_filter(char *identifier, char *filter)
{
  struct argument_t *f = malloc(sizeof(struct argument_t));
  f->data.STRING = strdup(identifier);
  f->type = T_STRING;
  struct argument_t *d = malloc(sizeof(struct argument_t));
  d->data.STRING = strdup(filter);
  d->type = T_STRING;
  f->next = d;
  struct operation_t *o = malloc(sizeof(struct operation_t));
  o->opcode = OP_FILTER;
  o->arguments = f;
  dumptree(optree(o), 0); /* print tree */
  return optree(o);
}

struct tree_t *bare_identifier(char *identifier)
{
  struct argument_t *f = malloc(sizeof(struct argument_t));
  f->data.STRING = identifier;
  f->type = T_STRING;
  struct operation_t *o = malloc(sizeof(struct operation_t));
  o->opcode = OP_GETECHO;
  o->arguments = f;
  dumptree(optree(o), 0); /* print */
  return optree(o);
}

