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
  int size;
  void *data;
  struct argument_t *next;
};

/* operation */
struct operation_t {
  char *name;
  int opcode;
  struct argument_t *arguments;  /* linked list of args */
};
  
/* optree */
struct tree_t {
  struct operation_t op;         
  struct tree_t *child;          /* linked list of children */
  struct tree_t *peer;           /* continuation of this linked list */
};

/* print out the parse tree */
void dumptree(struct tree_t *tree, int level);
