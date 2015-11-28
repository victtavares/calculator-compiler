typedef struct expression *expressionTree;

typedef enum {PlusOp, MinusOp, TimesOp, DivideOp} optype;

struct expression {
  enum {operatorExp, constantExp, variableExp} kind; 
  union {
    struct {
      optype op; 
      expressionTree left; 
      expressionTree right;
    } oper;
    double constantval;
    char *variable;
  } u; 
};

expressionTree operatorExpression(optype op, expressionTree left, expressionTree right);
expressionTree ConstantExpression(double constantval);

