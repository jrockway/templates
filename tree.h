#ifndef __template_tree_h
#define __template_tree_h 1


/* type of argument */
typedef int type_t;
#define T_NULL   0
#define T_STRING 1
#define T_INT    2
#define T_DOUBLE 3
#define T_VOID   4

/* argument to op */
struct argument_t {
  type_t type;
  union {
    char  *STRING;
    char  *SYMBOL;
    int    INT;
    double DOUBLE;
  } data;
  struct argument_t *next;
};
typedef struct argument_t argument_t;

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
#define OP_ECHO 3       /* echo some literal text (child or arg) */
#define OP_LOOP 5       /* loop over child until alter is false */ 
#define OP_BRANCH 6     /* test and execute child if true, alter otherwise */
#define OP_INCLUDE 7    /* include argument (filename) into the optree */
#define OP_WRAP 8       /* wrap this optree with argument (filename) */
#define OP_FILTER 9     /* run result of child thru named "filter" */ 
#define OP_START 10     /* root opcode */
#define OP_BAIL 11      /* exit with argument as the error */

/* optree */
struct tree_t {
  operation_t *op;         
  struct tree_t *child;          /* linked list of children */
  struct tree_t *alter;          /* alternate code path (if's else) */
  struct tree_t *peer;           /* continuation of this linked list */
  struct tree_t *last;           /* end of peer list; only valid for root */
};

typedef struct tree_t tree_t;
/* print out the parse tree */
void dumptree(tree_t *, int);
void add_op(tree_t *, tree_t *);
operation_t *op_string(int, char *);
tree_t *optree(operation_t *);

#endif
