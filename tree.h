#ifndef __template_tree_h
#define __template_tree_h 1


/* type of argument */
typedef int type_t;
#define T_NULL   0
#define T_STRING 1
#define T_SYMBOL 2
#define T_INT    3
#define T_OPERATOR 4
#define T_DOUBLE 5
#define T_VOID   6

/* argument to op */
struct argument_t {
  type_t type;
  union {
    char  *STRING;
    int    INT;
    double DOUBLE;
  } data;
  struct argument_t *next;
};
typedef struct argument_t argument_t;

/* if you add anything here, make sure tree.c:_destroy_op is still
   correct */
/* operation */
struct operation_t {
  int opcode;
  struct argument_t *arguments;  /* linked list of args */
};
typedef struct operation_t operation_t;

/* opcodes */
/* make sure you keep this sync'd with tree.c's dumptree function */
#define OP_NOP 0        /* no operation ? */
#define OP_GET 1        /* get something from the stash, return to parent */
#define OP_SET 2        /* set stash entry from child's return */
#define OP_ECHO 3       /* echo some text (child or arg if !child) */
#define OP_LOOP 5       /* loop over child until alter is false */ 
#define OP_BRANCH 6     /* test cond and execute R_TRUE if true, 
			   R_FALSE otherwise */
#define OP_INCLUDE 7    /* include argument (filename) into the optree */
#define OP_WRAP 8       /* wrap this optree with argument (filename) */
#define OP_FILTER 9     /* run result of child thru named "filter" */ 
#define OP_START 10     /* root opcode */
#define OP_BAIL 11      /* exit with argument as the error */
#define OP_EXPR 12      /* evaluate an expression. arg is operator,
			   R_LHS is lhs, R_RHS is rhs */
 
/* keep sync'd with dumptree */
/* condition opcodes for OP_EXPR's argument */
#define COP_EQUV 0      /* lhs == rhs */
#define COP_LESS 1      /* lhs <  rhs */
#define COP_MORE 2      /* lhs >  rhs */
#define COP_LTEQ 3      /* lhs <= rhs */
#define COP_GTEQ 4      /* lhs >= rhs */
#define COP_TRUE 5      /* lhs */
#define COP_NOT  6      /* !lhs */
#define COP_OR   7      /* lhs || rhs */
#define COP_AND  8      /* lhs && rhs */

/* if you add anything here, make sure to adjust destroy() and
   dumptree() */
/* optree */
struct tree_t {
  operation_t *op;         
  struct tree_t *child;          /* linked list of children */
  struct tree_t *alter;          /* alternate code path (if's else) */
  struct tree_t *peer;           /* continuation of this linked list */
  struct tree_t *last;           /* end of peer list */
  struct tree_t *cond;           /* condition for deciding on child/alter */
};

#define R_RHS child
#define R_LHS alter
#define R_TRUE child
#define R_FALSE alter
#define R_COND cond

typedef struct tree_t tree_t;
/* print out the parse tree */
void dumptree(tree_t *, int);
void destroy(tree_t *);
void add_op(tree_t *, tree_t *);
operation_t *op_string(int, char *);
argument_t *argument();
tree_t *optree(operation_t *);
tree_t *if_then(tree_t *, tree_t *, tree_t *);


#endif
