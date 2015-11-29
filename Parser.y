%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Structure.h"

/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(int i);
nodeType *con(int value);
void freeNode(nodeType *p);
int ex(nodeType *p);
int yylex(void);

void yyerror(char *s);
int sym[26];                    /* symbol table */
%}

%union {
    int iValue;                 /* integer value */
    char sIndex;                /* symbol table index */
    nodeType *nPtr;             /* node pointer */
};

%token <iValue> INTEGER
%token <sIndex> VARIABLE
%token PLUS MINUS TIMES DIVIDE PRINT
%token GREAT_EQUAL LESS_EQUAL TWO_EQUAL NOT_EQUAL LESS_THAN GREATER_THAN 
%token OPEN_PAREN CLOSE_PAREN EQUAL END_LINE
%token AND DO ELSE WHILE THEN END FOR IF VAR OR NOT
%token print

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
        block command     { ex($2); freeNode($2); }
        | 
        ;


command:
        END_LINE                           { $$ = opr(END_LINE, 2, NULL, NULL); }
        | expr END_LINE                    { $$ = $1; }
        | VARIABLE EQUAL expr END_LINE     { $$ = opr(EQUAL, 2, id($1), $3); }
        | functionCall   END_LINE
        | assignVariable END_LINE          { $$ = $1; }
        | WHILE expr DO command END         { $$ = opr(WHILE, 2, $2, $4); }
        | IF expr THEN command  %prec IFX   { $$ = opr(IF, 2, $2, $4); }
        | IF expr THEN command ELSE command { $$ = opr(IF, 3, $2, $4, $6);  }
        ;

functionCall:
        PRINT OPEN_PAREN expr CLOSE_PAREN { $$ = opr(PRINT, 1, $3); }
        ;

assignVariable:
        VAR VARIABLE              {{ $$ = id($2); }}
        | VAR VARIABLE EQUAL expr  { $$ = opr(EQUAL, 2, id($2), $4); }
         

expr:
          INTEGER                       { $$ = con($1); }
        | VARIABLE                      { $$ = id($1); }
        | MINUS expr %prec UMINUS       { $$ = opr(UMINUS, 1, $2); }
        | expr PLUS expr                { $$ = opr(PLUS, 2, $1, $3); }
        | expr MINUS expr               { $$ = opr(MINUS, 2, $1, $3); }
        | expr TIMES expr               { $$ = opr(TIMES, 2, $1, $3); }
        | expr DIVIDE expr              { $$ = opr(DIVIDE, 2, $1, $3); }
        | expr LESS_THAN expr           { $$ = opr(LESS_THAN, 2, $1, $3); }
        | expr LESS_EQUAL expr          { $$ = opr(LESS_EQUAL, 2, $1, $3); }
        | expr GREATER_THAN expr        { $$ = opr(GREATER_THAN, 2, $1, $3); }
        | expr GREAT_EQUAL expr         { $$ = opr(GREAT_EQUAL, 2, $1, $3); }
        | expr TWO_EQUAL expr           { $$ = opr(TWO_EQUAL, 2, $1, $3); }
        | expr NOT_EQUAL expr           { $$ = opr(NOT_EQUAL, 2, $1, $3); }
        | expr AND expr                 { $$ = opr(AND, 2, $1, $3); }
        | expr OR expr                  { $$ = opr(OR, 2, $1, $3); }
        | OPEN_PAREN expr CLOSE_PAREN   { $$ = $2; }
        | NOT expr                      { $$ = opr(UMINUS, 1, $2); }
        ;

%%

nodeType *con(int value) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.value = value;

    return p;
}

nodeType *id(int i) {
    nodeType *p;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeId;
    p->id.i = i;

    return p;
}

nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    int i;

    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");
    if ((p->opr.op = malloc(nops * sizeof(nodeType *))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}

void freeNode(nodeType *p) {
    int i;

    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
		free (p->opr.op);
    }
    free (p);
}

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}
