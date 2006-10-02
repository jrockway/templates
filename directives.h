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

#endif
