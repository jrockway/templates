#ifndef __template_directives_h
#define __template_directives_h 1
#include "tree.h"
tree_t *error(char *, int);
tree_t *identifier_directive(char *directive, char *identifier);
tree_t *identifier_filter(char *identifier, char *filter);
tree_t *chained_filter(tree_t *child_filter, char *filter);
tree_t *identifier(char *identifier);
tree_t *echo_tree(tree_t *tree);
tree_t *expr(tree_t *left, char *operator, tree_t *right);
tree_t *if_then(tree_t *condition, tree_t *true, tree_t *false);
tree_t *loop(tree_t *condition, tree_t *action);
tree_t *assign_loop(char *sink, char *source, tree_t *action);

#endif
