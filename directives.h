#ifndef __template_directives_h
#define __template_directives_h 1
#include "tree.h"
tree_t *identifier_filter(char *identifier, char *filter);
tree_t *chained_filter(tree_t *child_filter, char *filter);
tree_t *bare_identifier(char *identifier);
tree_t *echo_tree(tree_t *tree);
#endif
