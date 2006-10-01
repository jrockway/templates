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

/* operation */
struct operation_t {
  int opcode;
  struct argument_t *arguments;  /* linked list of args */
};

/* opcodes */
/* make sure you keep this sync'd with tree.c's dumptree function */
#define OP_GET 1
#define OP_SET 2
#define OP_LITECHO 3
#define OP_GETECHO 4
#define OP_LOOP 5
#define OP_TEST 6
#define OP_INCLUDE 7
#define OP_WRAP 8
#define OP_FILTER 9

/* optree */
struct tree_t {
  struct operation_t *op;         
  struct tree_t *child;          /* linked list of children */
  struct tree_t *peer;           /* continuation of this linked list */
  struct tree_t *last;           /* end of peer list; only valid for root */
};

typedef struct tree_t tree_t;
typedef struct operation_t operation_t;
typedef struct argument_t argument_t;

/* print out the parse tree */
void dumptree(tree_t *, int);
void add_op(tree_t *, tree_t *);
operation_t *op_string(int, char *);
tree_t *optree(operation_t *);

#endif
