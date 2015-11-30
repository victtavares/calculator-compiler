#include <stdio.h>
#include <string.h>
#include "Structure.h"
#include "y.tab.h"

int cgen(nodeType *p);

int headerWasCreated = 0;
int firstTime = 1;

//keep count of the assigners
nodeType* treeToAssignValue[255];
char identifierToAssignValue[255][255];
int count = 0;

//conditional paramater
char condition[4];

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
}


void generateHeader(nodeType *tree) {
    if (!headerWasCreated 
        && (tree->type != typeOpr || tree->opr.operator != EQUALVAR)
        && tree->opr.operator != END_LINE) {

        //creating the header text
        printf("  .text\n");
        printf("  .globl main\n\n");
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
        printf("  sw $a0, %s\n", p->identifier.value);      
        break;

    case typeOpr:
        switch(p->opr.operator) {

        case WHILE:
            cgen(p->opr.operands[0]);
            printf("LOOP%d:\n", count);
            printf("%s $a0, $t1, ENTRY%d\n", condition, count);
            printf("j END_LOOP%d\n", count);
            printf("ENTRY%d:\n", count);
            cgen(p->opr.operands[1]);
            printf("j LOOP%d\n", count);
            printf("END_LOOP%d:\n", count);
            count = count + 1;
            break;

        case IF:
            cgen(p->opr.operands[0]);
            printf("%s $a0, $t1, ENTRY%d\n", condition, count);
            if (p->opr.numberOfOperands > 2) cgen(p->opr.operands[2]);
            printf("j ELSE%d\n", count);
            printf("ENTRY%d:\n", count);
            cgen(p->opr.operands[1]);
            printf("ELSE%d:\n", count);
            break;


        case PRINT:     
            cgen(p->opr.operands[0]);
            printf("  li $vo, 1\n");
            printf("  syscall\n");
            break;

        case EQUAL:
            cgenAssignVariable(p->opr.operands[0]->identifier.value,p->opr.operands[1] );
            break;


        case EQUALVAR:

            if (firstTime) {
                printf(".data\n");
                firstTime = 0;

            }
            printf("  %s .word",p->opr.operands[0]->identifier.value);

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

        case UMINUS:
            cgen(p->opr.operands[0]);
            printf("  sub $a0, $zero, $a0\n");
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

        case TIMES:
            cgen(p->opr.operands[0]);      
            printf("  sw $a0, 0($sp)\n");
            printf("  addiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            printf("  lw $t1, 4($sp)\n");
            printf("  mul $a0, $t1, $a0\n");
            printf("  addiu $sp, $sp, 4\n");
            break;

        case DIVIDE:
            cgen(p->opr.operands[0]);      
            printf("  sw $a0, 0($sp)\n");
            printf("  addiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            printf("  lw $t1, 4($sp)\n");
            printf("  div $a0, $t1, $a0\n");
            printf("  addiu $sp, $sp, 4\n");
            break;


        case LESS_THAN: 
            cgen(p->opr.operands[0]);      
            printf("  sw $a0, 0($sp)\n");
            printf("  addiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            printf("  lw $t1, 4($sp)\n");
            printf("  addiu $sp, $sp, 4\n");
            strcpy(condition,"blt");
            break;

        case GREATER_THAN:  
            cgen(p->opr.operands[0]);      
            printf("  sw $a0, 0($sp)\n");
            printf("  addiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            printf("  lw $t1, 4($sp)\n");
            printf("  addiu $sp, $sp, 4\n");
            strcpy(condition,"bgt");
            break;

        case GREAT_EQUAL:
            cgen(p->opr.operands[0]);
            printf("  sw $a0, 0($sp)\n");
            printf("  addiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            printf("  lw $t1, 4($sp)\n");
            printf("  addiu $sp, $sp, 4\n");
            strcpy(condition,"bne");
            break;

        case LESS_EQUAL:    
            cgen(p->opr.operands[0]);
            printf("  sw $a0, 0($sp)\n");
            printf("  addiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            printf("  lw $t1, 4($sp)\n");
            printf("  addiu $sp, $sp, 4\n");
            strcpy(condition,"ble");
            break;

        case NOT_EQUAL:
            cgen(p->opr.operands[0]);
            printf("  sw $a0, 0($sp)\n");
            printf("  addiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            printf("  lw $t1, 4($sp)\n");
            printf("  addiu $sp, $sp, 4\n");
            strcpy(condition,"bne");
            break;

        case TWO_EQUAL:
            cgen(p->opr.operands[0]);
            printf("  sw $a0, 0($sp)\n");
            printf("  addiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            printf("  lw $t1, 4($sp)\n");
            printf("  addiu $sp, $sp, 4\n");
            strcpy(condition,"beq");
            break;

        }
    }
    return 0;
}
