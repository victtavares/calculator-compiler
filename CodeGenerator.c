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
int count2 = 0;
int countWhile = 0;

//conditional paramater
char condition[4];


void cgenAssignVariable(char *variable, nodeType *expression) {
    cgen(expression);
    fprintf(file,"\tsw $a0, %s\n", variable);
}


void generateHeader(nodeType *tree) {
    if (!headerWasCreated 
        && (tree->type != typeOpr || tree->opr.operator != EQUALVAR)
        && tree->opr.operator != END_LINE) {

        //creating the header text
        fprintf(file,"\t.text\n");
        fprintf(file,"\t.globl main\n\n");
        fprintf(file,"main:\n");

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
        fprintf(file,"\tli $a0, %d\n", p->constant.value);
        break;
    case typeIdentifier:
        fprintf(file,"\tsw $a0, %s\n", p->identifier.value);      
        break;

    case typeOpr:
        switch(p->opr.operator) {

        case WHILE:
            countWhile++;
            cgen(p->opr.operands[0]);
            fprintf(file,"LOOP%d:\n", countWhile);
            fprintf(file,"\t%s $a0, $t1, ENTRY%d\n", condition, countWhile);
            fprintf(file,"\tb END_LOOP%d\n", countWhile);
            fprintf(file,"ENTRY%d:\n", countWhile);
            int oldWhileValue = countWhile;
            cgen(p->opr.operands[1]);
            fprintf(file,"\tb LOOP%d\n", oldWhileValue);
            fprintf(file,"END_LOOP%d:\n", oldWhileValue);
            break;

        case IF:
            cgen(p->opr.operands[0]);
            count2++;
            fprintf(file,"\t%s $a0, $t1, true_branch%d\n", condition, count2);
            if (p->opr.numberOfOperands > 2) {
                fprintf(file,"false_branch%d:\n", count2);
                
                cgen(p->opr.operands[2]);
                fprintf(file,"\tb end_if\n");
            }

            fprintf(file,"true_branch%d:\n", count2);
            int oldValue = count2;
            cgen(p->opr.operands[1]);
            fprintf(file,"\tend_if%d:\n", oldValue);

            break;


        case PRINT:     
            cgen(p->opr.operands[0]);
            fprintf(file,"\tli $vo, 1\n");
            fprintf(file,"\tsyscall\n");
            break;

        case EQUAL:
            cgenAssignVariable(p->opr.operands[0]->identifier.value,p->opr.operands[1] );
            break;


        case EQUALVAR:

            if (firstTime) {
                fprintf(file,".data\n");
                firstTime = 0;
            }


            fprintf(file,"\t%s .word",p->opr.operands[0]->identifier.value);

            nodeType *nodeRight =  p->opr.operands[1];
            if (nodeRight->type == typeConstant) {
                fprintf(file," %d\n", p->opr.operands[1]->constant.value);
            } else {
                fprintf(file," 0\n");
                treeToAssignValue[count] = nodeRight;
                strcpy(identifierToAssignValue[count], p->opr.operands[0]->identifier.value);
                count++;
                //cgenAssignVariable(p->opr.operands[0]->identifier.value, nodeRight);
            }
            break;

        case UMINUS:
            cgen(p->opr.operands[0]);
            fprintf(file,"\tsub $a0, $zero, $a0\n");
            break;

        case PLUS: 
            cgen(p->opr.operands[0]);      
            fprintf(file,"\tsw $a0, 0($sp)\n");
            fprintf(file,"\taddiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            fprintf(file,"\tlw $t1, 4($sp)\n");
            fprintf(file,"\tadd $a0, $t1, $a0\n");
            fprintf(file,"\taddiu $sp, $sp, 4\n");
            break;

        case MINUS: 
            cgen(p->opr.operands[0]);      
            fprintf(file,"\tsw $a0, 0($sp)\n");
            fprintf(file,"\taddiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            fprintf(file,"\tlw $t1, 4($sp)\n");
            fprintf(file,"\tsub $a0, $t1, $a0\n");
            fprintf(file,"\taddiu $sp, $sp, 4\n");
            break;

        case TIMES:
            cgen(p->opr.operands[0]);      
            fprintf(file,"\tsw $a0, 0($sp)\n");
            fprintf(file,"\taddiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            fprintf(file,"\tlw $t1, 4($sp)\n");
            fprintf(file,"\tmul $a0, $t1, $a0\n");
            fprintf(file,"\taddiu $sp, $sp, 4\n");
            break;

        case DIVIDE:
            cgen(p->opr.operands[0]);      
            fprintf(file,"\tsw $a0, 0($sp)\n");
            fprintf(file,"\taddiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            fprintf(file,"\tlw $t1, 4($sp)\n");
            fprintf(file,"\tdiv $a0, $t1, $a0\n");
            fprintf(file,"\taddiu $sp, $sp, 4\n");
            break;


        case LESS_THAN: 
            cgen(p->opr.operands[0]);      
            fprintf(file,"\tsw $a0, 0($sp)\n");
            fprintf(file,"\taddiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            fprintf(file,"\tlw $t1, 4($sp)\n");
            fprintf(file,"\taddiu $sp, $sp, 4\n");
            strcpy(condition,"blt");
            break;

        case GREATER_THAN:  
            cgen(p->opr.operands[0]);      
            fprintf(file,"\tsw $a0, 0($sp)\n");
            fprintf(file,"\taddiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            fprintf(file,"\tlw $t1, 4($sp)\n");
            fprintf(file,"\taddiu $sp, $sp, 4\n");
            strcpy(condition,"bgt");
            break;

        case GREAT_EQUAL:
            cgen(p->opr.operands[0]);
            fprintf(file,"\tsw $a0, 0($sp)\n");
            fprintf(file,"\taddiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            fprintf(file,"\tlw $t1, 4($sp)\n");
            fprintf(file,"\taddiu $sp, $sp, 4\n");
            strcpy(condition,"bne");
            break;

        case LESS_EQUAL:    
            cgen(p->opr.operands[0]);
            fprintf(file,"\tsw $a0, 0($sp)\n");
            fprintf(file,"\taddiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            fprintf(file,"\tlw $t1, 4($sp)\n");
            fprintf(file,"\taddiu $sp, $sp, 4\n");
            strcpy(condition,"ble");
            break;

        case NOT_EQUAL:
            cgen(p->opr.operands[0]);
            fprintf(file,"\tsw $a0, 0($sp)\n");
            fprintf(file,"\taddiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            fprintf(file,"\tlw $t1, 4($sp)\n");
            fprintf(file,"\taddiu $sp, $sp, 4\n");
            strcpy(condition,"bne");
            break;

        case TWO_EQUAL:
            cgen(p->opr.operands[0]);
            fprintf(file,"\tsw $a0, 0($sp)\n");
            fprintf(file,"\taddiu $sp, $sp -4\n");
            cgen(p->opr.operands[1]);
            fprintf(file,"\tlw $t1, 4($sp)\n");
            fprintf(file,"\taddiu $sp, $sp, 4\n");
            strcpy(condition,"beq");
            break;

        }
    }
    return 0;
}
