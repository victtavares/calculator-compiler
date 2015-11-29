
/* node with operator and operands */
typedef struct {
    int operator;                   /* operator */
    int numberOfOperands;         /* number of operands*/
    struct nodeTypeTag **operands;    /* operands */
} oprNodeType;

typedef struct {
    int value;                  /* value of constant */
} constantNodeType;

/* identifier structure */
typedef struct {
    int value;                      /* subscript to sym array */
} identifierNodeType;


typedef enum { typeOpr, typeIdentifier, typeConstant } nodeTypesEnum;

typedef struct nodeTypeTag {
    nodeTypesEnum type;              /* type of node */

    union {
        constantNodeType constant;        /* constants */
        identifierNodeType identifier;          /* identifiers */
        oprNodeType opr;        /* operators */
    };
} nodeType;

extern int sym[26];
