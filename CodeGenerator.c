#include <stdio.h>
#include <string.h>
#include "Structure.h"
#include "y.tab.h"

int count = 0;
char condition[4];
 
int ex(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeCon:       
        printf("li $a0, %d\n", p->con.value);
        break;
    case typeId:
        printf("li $a0, %c\n", p->id.i + 'a');      
        break;

    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:
            ex(p->opr.op[0]);
            printf("LOOP%d:\n", count);
            printf("%s $a0, $t1, ENTRY%d\n", condition, count);
            printf("j END_LOOP%d\n", count);
            printf("ENTRY%d:\n", count);
            ex(p->opr.op[1]);
            printf("j LOOP%d\n", count);
            printf("END_LOOP%d:\n", count);
            count = count + 1;
            break;
        case IF:
            ex(p->opr.op[0]);
            printf("%s $a0, $t1, ENTRY%d\n", condition, count);
            if (p->opr.nops > 2) ex(p->opr.op[2]);
            printf("j ELSE%d\n", count);
            printf("ENTRY%d:\n", count);
            ex(p->opr.op[1]);
            printf("ELSE%d:\n", count);
            break;
        case PRINT:     
            ex(p->opr.op[0]);
            printf("print\n");
            break;
        case EQUAL:       
            //return sym[p->opr.op[0]->id.i] = ex(p->opr.op[1]);
        case UMINUS:
            ex(p->opr.op[0]);
            printf("sub $a0, $zero, $a0\n");
            break;
        case PLUS: 
            ex(p->opr.op[0]);      
            printf("sw $a0, 0($sp)\n");
            printf("addiu $sp, $sp -4\n");
            ex(p->opr.op[1]);
            printf("lw $t1, 4($sp)\n");
            printf("add $a0, $t1, $a0\n");
            printf("addiu $sp, $sp, 4\n");
            break;
        case MINUS: 
            ex(p->opr.op[0]);      
            printf("sw $a0, 0($sp)\n");
            printf("addiu $sp, $sp -4\n");
            ex(p->opr.op[1]);
            printf("lw $t1, 4($sp)\n");
            printf("sub $a0, $t1, $a0\n");
            printf("addiu $sp, $sp, 4\n");
            break;     
        case TIMES:
            ex(p->opr.op[0]);      
            printf("sw $a0, 0($sp)\n");
            printf("addiu $sp, $sp -4\n");
            ex(p->opr.op[1]);
            printf("lw $t1, 4($sp)\n");
            printf("mul $a0, $t1, $a0\n");
            printf("addiu $sp, $sp, 4\n");      
        case DIVIDE:
            ex(p->opr.op[0]);      
            printf("sw $a0, 0($sp)\n");
            printf("addiu $sp, $sp -4\n");
            ex(p->opr.op[1]);
            printf("lw $t1, 4($sp)\n");
            printf("div $a0, $t1, $a0\n");
            printf("addiu $sp, $sp, 4\n");        
        case LESS_THAN:     return ex(p->opr.op[0]) < ex(p->opr.op[1]);
            ex(p->opr.op[0]);      
            printf("sw $a0, 0($sp)\n");
            printf("addiu $sp, $sp -4\n");
            ex(p->opr.op[1]);
            printf("lw $t1, 4($sp)\n");
            printf("addiu $sp, $sp, 4\n");
            strcpy(condition,"blt");
        case GREATER_THAN:  return ex(p->opr.op[0]) > ex(p->opr.op[1]);
            ex(p->opr.op[0]);      
            printf("sw $a0, 0($sp)\n");
            printf("addiu $sp, $sp -4\n");
            ex(p->opr.op[1]);
            printf("lw $t1, 4($sp)\n");
            printf("addiu $sp, $sp, 4\n");
            strcpy(condition,"bgt");
        case GREAT_EQUAL:
            ex(p->opr.op[0]);
            printf("sw $a0, 0($sp)\n");
            printf("addiu $sp, $sp -4\n");
            ex(p->opr.op[1]);
            printf("lw $t1, 4($sp)\n");
            printf("addiu $sp, $sp, 4\n");
            strcpy(condition,"bne");
        case LESS_EQUAL:    
            ex(p->opr.op[0]);
            printf("sw $a0, 0($sp)\n");
            printf("addiu $sp, $sp -4\n");
            ex(p->opr.op[1]);
            printf("lw $t1, 4($sp)\n");
            printf("addiu $sp, $sp, 4\n");
            strcpy(condition,"ble");
        case NOT_EQUAL:
            ex(p->opr.op[0]);
            printf("sw $a0, 0($sp)\n");
            printf("addiu $sp, $sp -4\n");
            ex(p->opr.op[1]);
            printf("lw $t1, 4($sp)\n");
            printf("addiu $sp, $sp, 4\n");
            strcpy(condition,"bne");
        case TWO_EQUAL:
            ex(p->opr.op[0]);
            printf("sw $a0, 0($sp)\n");
            printf("addiu $sp, $sp -4\n");
            ex(p->opr.op[1]);
            printf("lw $t1, 4($sp)\n");
            printf("addiu $sp, $sp, 4\n");
            strcpy(condition,"beq"); 
        }
    }
    return 0;
}
