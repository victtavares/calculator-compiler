
%{
#include <stdlib.h>
#include <string.h>
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

"+"             return PLUS;
"-"             return MINUS;
"*"             return TIMES;
"/"             return DIVIDE;


">="            return GREAT_EQUAL;
"<="            return LESS_EQUAL;
"=="            return TWO_EQUAL;
"~="            return NOT_EQUAL;
">"             return GREATER_THAN;
"<"             return LESS_THAN;


"("             return OPEN_PAREN;
")"             return CLOSE_PAREN;
"="             return EQUAL;

"and"           return AND;
"do"            return DO;
"else"          return ELSE;
"while"         return WHILE;
"then"          return THEN;
"end"           return END;
"for"           return FOR;
"if"            return IF;
"var"           return VAR;
"or"            return OR;
"not"           return NOT;


"print"         return PRINT;

[;\n]           return END_LINE;

[{}]         {
                return *yytext;
             }

{id}        { 
                strcpy(yylval.sIndex, yytext);
                return VARIABLE;
            }

[ \t]+        ;       /* ignoring whitespace */



.           yyerror("Caracter desconhecido!");
%%
int yywrap(void) {
    return 1;
}
