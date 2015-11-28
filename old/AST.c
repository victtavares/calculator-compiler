#include "AST.h"
#include <stdlib.h>
#include <stdio.h>


expressionTree operatorExpression(optype op, expressionTree left, expressionTree right) {
  expressionTree retval = (expressionTree) malloc(sizeof(struct expression));
  retval->kind = operatorExp;
  retval->u.oper.op = op;
  retval->u.oper.left = left;
  retval->u.oper.right = right;
  return retval;
}

expressionTree ConstantExpression(double constantval) {
  expressionTree retval = (expressionTree) malloc(sizeof(struct expression));
  retval->kind = constantExp; 
  retval->u.constantval = constantval; 
  return retval;
}
