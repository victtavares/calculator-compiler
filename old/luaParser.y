%{
#include "AST.h"
#include <stdio.h>
#include <stdlib.h>
#include  <math.h>


int yylex(void);
int yyerror(char *s);
expressionTree root;
%}

%union{
  double number_value;
  char *string_value;
  expressionTree expression_tree;
}

%token<number_value> NUMBER
%token PLUS MINUS TIMES DIVIDE POWER MOD
%token OPEN_PARENTHESES CLOSE_PARENTHESES

%type <expression_tree> Expression

%left PLUS MINUS
%left TIMES DIVIDE
%left NEG
%right POWER

%start Input

%%

Input: Expression                            {root = $1;}


Expression:
     NUMBER                                     { $$ = ConstantExpression($1); }
| Expression PLUS Expression                    { $$ = OperatorExpression(PlusOp,$1,$3); }
| Expression MINUS Expression                   { $$ = OperatorExpression(MinusOp,$1,$3); }
| Expression TIMES Expression                   { $$ = OperatorExpression(TimesOp,$1,$3); }
| Expression DIVIDE Expression                  { $$ = OperatorExpression(DivideOp,$1,$3); }
;

%%

int yyerror(char *s) {
  printf("error on %s\n", s);
  return 0;
}

int main() {
  yyparse();
   return 0;
}