#include <stdio.h>
#include <string.h>
#include "Structure.h"
#include "y.tab.h"

int cgen(nodeType *p);

int headerWasCreated = 0;

//keep count of the assigners
nodeType* treeToAssignValue[255];
char identifierToAssignValue[255][255];
int count = 0;

void generateMips(nodeType *tree) {

    // if (firstTime) {
    //     // printf(".text\n");
    //     // printf(".globl main\n\n");
    //     // printf("main:\n\n");
    //     firstTime = 0;
    // }

    cgen(tree);

}


void cgenAssignVariable(char *variable, nodeType *expression) {
    cgen(expression);
    printf("  sw $a0, %s\n", variable);
    printf("  lw $a0, %s\n", variable);
}


void generateHeader(nodeType *tree) {
    if (!headerWasCreated 
        && (tree->type != typeOpr || tree->opr.operator != EQUALVAR)
        && tree->opr.operator != END_LINE) {

        //creating the header text
        printf(".text\n");
        printf(".gobl main\n\n");
        printf("main:\n");
        
        headerWasCreated = 1;
        for (int i = 0; i < count; i++) {
            cgenAssignVariable(identifierToAssignValue[i], treeToAssignValue[i]);
        }




        

    }  
}

int cgen(nodeType *p) {

    if (!p) return 0;
    generateHeader(p);
    switch(p->type) {

    case typeConstant:  
        printf("  li $a0, %d\n", p->constant.value);
        break;
    case typeIdentifier:
        printf("  li $a0, %s\n", p->identifier.value);      
        break;

    case typeOpr:
        switch(p->opr.operator) {


        // case WHILE:     while(ex(p->opr.op[0])) ex(p->opr.op[1]); return 0;
        // case IF:        if (ex(p->opr.op[0]))
        //                     ex(p->opr.op[1]);
        //                 else if (p->opr.nops > 2)
        //                     ex(p->opr.op[2]);
        //                 return 0;
        case PRINT:     
            cgen(p->opr.operands[0]);
            printf("  li $vo, 1\n");
            printf("  syscall\n");
            break;
        // case DOT_COMMA:       //ex(p->opr.op[0]); return ex(p->opr.op[1]);
        // case EQUAL:       return sym[p->opr.op[0]->id.i] = ex(p->opr.op[1]);
        // case UMINUS:    return -ex(p->opr.op[0]);
        case EQUALVAR:
            printf("%s .word",p->opr.operands[0]->identifier.value);

            nodeType *nodeRight =  p->opr.operands[1];
            if (nodeRight->type == typeConstant) {
                printf(" %d\n", p->opr.operands[1]->constant.value);
            } else {
                printf(" 0\n");
                treeToAssignValue[count] = nodeRight;
                strcpy(identifierToAssignValue[count], p->opr.operands[0]->identifier.value);
                count++;
                //cgenAssignVariable(p->opr.operands[0]->identifier.value, nodeRight);
            }
            break;

        case PLUS: 
            cgen(p->opr.operands[0]);      
            printf("  sw $a0, 0($sp)\n");
            printf("  addiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            printf("  lw $t1, 4($sp)\n");
            printf("  add $a0, $t1, $a0\n");
            printf("  addiu $sp, $sp, 4\n");
            break;

        case MINUS: 
            cgen(p->opr.operands[0]);      
            printf("  sw $a0, 0($sp)\n");
            printf("  addiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            printf("  lw $t1, 4($sp)\n");
            printf("  sub $a0, $t1, $a0\n");
            printf("  addiu $sp, $sp, 4\n");
            break;     
        // case TIMES:         return ex(p->opr.op[0]) * ex(p->opr.op[1]);
        // case DIVIDE:        return ex(p->opr.op[0]) / ex(p->opr.op[1]);
        // case LESS_THAN:     return ex(p->opr.op[0]) < ex(p->opr.op[1]);
        // case GREATER_THAN:  return ex(p->opr.op[0]) > ex(p->opr.op[1]);
        // case GREAT_EQUAL:   return ex(p->opr.op[0]) >= ex(p->opr.op[1]);
        // case LESS_EQUAL:    return ex(p->opr.op[0]) <= ex(p->opr.op[1]);
        // case NOT_EQUAL:     return ex(p->opr.op[0]) != ex(p->opr.op[1]);
        // case TWO_EQUAL:     return ex(p->opr.op[0]) == ex(p->opr.op[1]);
        }
    }
    return 0;
}
