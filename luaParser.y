%{
#include <stdio.h>
#include <stdlib.h>
#define YYSTYPE double
int yylex(void);
int yyerror(char *s);
%}

%token NUMBER
%token PLUS MINUS TIMES DIVIDE POWER MOD
%token OPEN_PARENTHESES CLOSE_PARENTHESES
%token END

%left PLUS MINUS
%left TIMES DIVIDE
%left NEG
%right POWER

%start Input

%%

Input:| Input Line;

Line:END| Expression END { printf("Result: %f\n", $1); };

Expression:
     NUMBER { $$=$1; printf("Value on Number: %f\n", $1);}
| Expression PLUS Expression { $$=$1+$3; printf("Value on Plus: %f + %f\n", $1,$3); }
| Expression MINUS Expression { $$=$1-$3; }
| Expression TIMES Expression { $$=$1*$3; }
| Expression DIVIDE Expression { $$=$1/$3; }
| MINUS Expression %prec NEG { $$=-$2; }
| Expression POWER Expression { $$=pow($1,$3); }
| OPEN_PARENTHESES Expression CLOSE_PARENTHESES { $$=$2; }
;

%%

int yyerror(char *s) {
  printf("%s\n", s);
  return 0;
}

int main() {
  yyparse();
   return 0;
}
