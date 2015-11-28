%{
   #include <stdlib.h>
   #include "luaParser.tab.h"
   #include "AST.h"
%}

whitespace      ([ \t\n\r\f])+
letter          [A-Za-z]
digit           [0-9]
id              ({letter}|_)({letter}|({digit}|_))*

realNumber      {digit}+(\.{digit}+)?([Ee][+-]?{digit}+)?
hexadecimal     0[xX][0-9A-Fa-f]+
number          {realNumber}|{hexadecimal}

commentLine         --[^\[].*
/* -- qualquer coisa exceção o ] e então o ]*/
commentMultiLine    --\[[^\]]*\]
/*  Aspas seguidas de zero ou mais (qualquer caracter que não é uma aspas ou backslash) ou (uma backslash seguida de qualquer coisa)*/
stringAspasSimples  '([^'\\]|\\.)*'
stringAspasDuplas   \"([^"\\]|\\.)*\"
/* falta string com ==[]=== */

%%

{commentLine}         printf("comment: %s\n", yytext);
{whitespace}          {/* Do Nothing */}
{number}              { yylval.number_value = atof(yytext);  return NUMBER; }
{commentMultiLine}    printf("comment multiline: %s\n", yytext);
{stringAspasSimples}  printf("stringAspasSimples: %s\n", yytext);
{stringAspasDuplas}   printf("stringAspasDuplas: %s\n", yytext);


and             printf("keyword: %s\n", yytext);
break           printf("keyword: %s\n", yytext);
do              printf("keyword: %s\n", yytext);
else            printf("keyword: %s\n", yytext);
elseif          printf("keyword: %s\n", yytext);
end             printf("keyword: %s\n", yytext);
false           printf("keyword: %s\n", yytext);
for             printf("keyword: %s\n", yytext);
function        printf("keyword: %s\n", yytext);
if              printf("keyword: %s\n", yytext);
in              printf("keyword: %s\n", yytext);
local           printf("keyword: %s\n", yytext);
nil             printf("keyword: %s\n", yytext);
not             printf("keyword: %s\n", yytext);
or              printf("keyword: %s\n", yytext);
repeat          printf("keyword: %s\n", yytext);
return          printf("keyword: %s\n", yytext);
then            printf("keyword: %s\n", yytext);
true            printf("keyword: %s\n", yytext);
until           printf("keyword: %s\n", yytext);
while           printf("keyword: %s\n", yytext);

"+"             return PLUS;
"-"             return MINUS;
"*"             return TIMES;
"/"             return DIVIDE;
"%"             return MOD;
"^"             return POWER;

"#"             printf("Item lexico: %s\n", yytext);
"=="            printf("Item lexico: %s\n", yytext);
"~="            printf("Item lexico: %s\n", yytext);
"<="            printf("Item lexico: %s\n", yytext);
">="            printf("Item lexico: %s\n", yytext);
"<"             printf("Item lexico: %s\n", yytext);
">"             printf("Item lexico: %s\n", yytext);
"="             printf("Item lexico: %s\n", yytext);
"("             return OPEN_PARENTHESES;
")"             return CLOSE_PARENTHESES;
"{"             printf("Item lexico: %s\n", yytext);
"}"             printf("Item lexico: %s\n", yytext);
"["             printf("Item lexico: %s\n", yytext);
"]"             printf("Item lexico: %s\n", yytext);
";"             printf("Item lexico: %s\n", yytext);
":"             printf("Item lexico: %s\n", yytext);
","             printf("Item lexico: %s\n", yytext);
"."             printf("Item lexico: %s\n", yytext);
".."            printf("Item lexico: %s\n", yytext);
"..."           printf("Item lexico: %s\n", yytext);


{id}            printf("id:  %s\n", yytext);


.               printf( "Unrecognized character: %s\n", yytext );


%%
/*
int main( argcounter, argv )
int argcounter;
char **argv;
{
    //to skip over program name
    ++argv, --argcounter;
    if ( argcounter > 0 )
    yyin = fopen( argv[0], "r" );
    else
    yyin = stdin;
    
    yylex();
}
*/