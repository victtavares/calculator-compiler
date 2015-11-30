%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "Structure.h"


/* prototypes */
nodeType *createOpr(int oper, int nops, ...);
nodeType *createIdentifier(char identifier[255]);
nodeType *createConstant(int value);
void freeNode(nodeType *p);
int cgen(nodeType *p);
int yylex(void);

void yyerror(char *s);
FILE *file;
char INPUT[64];
char OUTPUT[64];
int sym[26];                    /* symbol table */
%}

%union {
    int iValue;                 /* integer value */
    char sIndex[255];                /* symbol table index */
    nodeType *nPtr;             /* node pointer */
};

%token <iValue> INTEGER
%token <sIndex> VARIABLE
%token PLUS MINUS TIMES DIVIDE 
%token GREAT_EQUAL LESS_EQUAL TWO_EQUAL NOT_EQUAL LESS_THAN GREATER_THAN 
%token OPEN_PAREN CLOSE_PAREN EQUAL END_LINE
%token AND DO ELSE WHILE THEN END FOR IF VAR OR NOT
%token PRINT
%token EQUALVAR

%nonassoc IFX
%nonassoc ELSE

%left AND OR
%left GREAT_EQUAL LESS_EQUAL TWO_EQUAL NOT_EQUAL GREATER_THAN LESS_THAN
%left PLUS MINUS
%left TIMES DIVIDE
%right NOT
%right UMINUS

 %type <nPtr> command expr functionCall assignVariable

%%
block:
        block command     { cgen($2); }
        |                 
        ;


command:
        END_LINE                           { $$ = createOpr(END_LINE, 2, NULL, NULL); }
        | expr END_LINE                    { $$ = $1; }
        | VARIABLE EQUAL expr END_LINE     { $$ = createOpr(EQUAL, 2, createIdentifier($1), $3); }
        | functionCall   END_LINE
        | assignVariable END_LINE          { $$ = $1; }
        | WHILE expr DO command END         { $$ = createOpr(WHILE, 2, $2, $4); }
        | IF expr THEN command END  %prec IFX   { $$ = createOpr(IF, 2, $2, $4); }
        | IF expr THEN command ELSE command END { $$ = createOpr(IF, 3, $2, $4, $6);  }
        ;

functionCall:
        PRINT OPEN_PAREN expr CLOSE_PAREN { $$ = createOpr(PRINT, 1, $3); }
        ;

assignVariable:
        VAR VARIABLE %prec EQUALVAR                 { $$ = createOpr(EQUALVAR, 2, createIdentifier($2), createConstant(0)); }
        | VAR VARIABLE EQUAL expr %prec EQUALVAR    { $$ = createOpr(EQUALVAR, 2, createIdentifier($2), $4); }
         

expr:
          INTEGER                       { $$ = createConstant($1); }
        | VARIABLE                      { $$ = createIdentifier($1); }
        | MINUS expr %prec UMINUS       { $$ = createOpr(UMINUS, 1, $2); }
        | expr PLUS expr                { $$ = createOpr(PLUS, 2, $1, $3); }
        | expr MINUS expr               { $$ = createOpr(MINUS, 2, $1, $3); }
        | expr TIMES expr               { $$ = createOpr(TIMES, 2, $1, $3); }
        | expr DIVIDE expr              { $$ = createOpr(DIVIDE, 2, $1, $3); }
        | expr LESS_THAN expr           { $$ = createOpr(LESS_THAN, 2, $1, $3); }
        | expr LESS_EQUAL expr          { $$ = createOpr(LESS_EQUAL, 2, $1, $3); }
        | expr GREATER_THAN expr        { $$ = createOpr(GREATER_THAN, 2, $1, $3); }
        | expr GREAT_EQUAL expr         { $$ = createOpr(GREAT_EQUAL, 2, $1, $3); }
        | expr TWO_EQUAL expr           { $$ = createOpr(TWO_EQUAL, 2, $1, $3); }
        | expr NOT_EQUAL expr           { $$ = createOpr(NOT_EQUAL, 2, $1, $3); }
        | expr AND expr                 { $$ = createOpr(AND, 2, $1, $3); }
        | expr OR expr                  { $$ = createOpr(OR, 2, $1, $3); }
        | OPEN_PAREN expr CLOSE_PAREN   { $$ = $2; }
        | NOT expr                      { $$ = createOpr(UMINUS, 1, $2); }
        ;

%%

nodeType *createConstant(int value) {
    nodeType *p;

    /* allocate node and shows memory error if necessary*/
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* put the data on the node*/
    p->type = typeConstant;
    p->constant.value = value;

    return p;
}

nodeType *createIdentifier(char identifier[255]) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* put the data on the node*/
    p->type = typeIdentifier;
    strcpy(p->identifier.value, identifier);
    //p->identifier.value = identifier;

    return p;
}

nodeType *createOpr(int operator, int numberOfOperands, ...) {
    va_list ap;
    nodeType *p;
    int i;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");
    if ((p->opr.operands = malloc(numberOfOperands * sizeof(nodeType *))) == NULL)
        yyerror("out of memory");

    /* put the data on the node*/
    p->type = typeOpr;
    p->opr.operator = operator;
    p->opr.numberOfOperands = numberOfOperands;
    va_start(ap, numberOfOperands);
    for (i = 0; i < numberOfOperands; i++)
        p->opr.operands[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}


void yyerror(char *s) {
    printf("Error on parsing!\n");
    fprintf(stdout, "%s\n", s);
}

void exitFunction() {
   fprintf(file,"\n\tli $v0, 10\n");  
   fprintf(file,"\tsyscall\n");
   fclose(file);
   fclose(stdin);
}

void openFile() {
    file = fopen(OUTPUT,"w");
}

int main(int argc, char **argv) {
  
     // testing arguement 
     if (argc != 3){
        printf ("\nUso: ./Compiler <INPUT> <OUTPUT>\n");
        return (1);
    }else{
        sprintf(INPUT, "%s", argv[1]);
        sprintf(OUTPUT, "%s", argv[2]);
    }
    openFile();
    freopen(INPUT, "r", stdin);
    yyparse();
    exitFunction();
    return 0;


}
