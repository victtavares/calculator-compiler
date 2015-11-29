
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


[{}] {
                return *yytext;
             }

"+"             return PLUS;
"-"             return MINUS;
"*"             return TIMES;
"/"             return DIVIDE;


">="            return GREAT_EQUAL;
"<="            return LESS_EQUAL;
"=="            return TWO_EQUAL;
"!="            return NOT_EQUAL;
">"             return GREATER_THAN;
"<"             return LESS_THAN;
"~="            return ALMOST_EQUAL;


"("             return OPEN_PAREN;
")"             return CLOSE_PAREN;
"="             return EQUAL;
";"             return DOT_COMMA;



"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;


"print"         return PRINT;

[ \t\n]+        ;       /* ignoring whitespace */

{id}        { 
                yylval.sIndex = *yytext - 'a';
                return VARIABLE;
            }

.           yyerror("Caracter desconhecido!");
%%
int yywrap(void) {
    return 1;
}
