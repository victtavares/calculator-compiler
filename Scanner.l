
%{
#include <stdlib.h>
#include "Structure.h"
#include "y.tab.h"
void yyerror(char *);
%}

letter          [A-Za-z]
digit           [0-9]
id              ({letter}|_)({letter}|({digit}|_))*

numberNoZero    [1-9][0-9]*
zero            0
number          {numberNoZero}|{zero}
%%


{number}    {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }


[()<>=;{}.] {
                return *yytext;
             }
/* Math Tokens */
"+"             return PLUS;
"-"             return MINUS;
"*"             return TIMES;
"/"             return DIVIDE;

/* Comparison Tokens */
">="            return GREAT_EQUAL;
"<="            return LESS_EQUAL;
"=="            return TWO_EQUAL;
"!="            return NOT_EQUAL;


"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"print"         return PRINT;

[ \t\n]+        ;       /* ignore whitespace */

{id}        { 
                yylval.sIndex = *yytext - 'a';
                return VARIABLE;
            }

.               yyerror("Unknown character");
%%
int yywrap(void) {
    return 1;
}
