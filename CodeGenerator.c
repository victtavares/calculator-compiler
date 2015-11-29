#include <stdio.h>
#include "Structure.h"
#include "y.tab.h"


int ex(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeConstant:       
        printf("li $a0, %d\n", p->constant.value);
        break;
    case typeIdentifier:
        printf("li $a0, %c\n", p->identifier.value + 'a');      
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
            ex(p->opr.operands[0]);
            printf("print\n");
            break;
        // case DOT_COMMA:       //ex(p->opr.op[0]); return ex(p->opr.op[1]);
        // case EQUAL:       return sym[p->opr.op[0]->id.i] = ex(p->opr.op[1]);
        // case UMINUS:    return -ex(p->opr.op[0]);
        case PLUS: 
            ex(p->opr.operands[0]);      
            printf("sw $a0, 0($sp)\n");
            printf("addiu $sp, $sp -4\n");
            ex(p->opr.operands[1]);
            printf("lw $t1, 4($sp)\n");
            printf("add $a0, $t1, $a0\n");
            printf("addiu $sp, $sp, 4\n");
            break;

        case MINUS: 
            ex(p->opr.operands[0]);      
            printf("sw $a0, 0($sp)\n");
            printf("addiu $sp, $sp -4\n");
            ex(p->opr.operands[1]);
            printf("lw $t1, 4($sp)\n");
            printf("sub $a0, $t1, $a0\n");
            printf("addiu $sp, $sp, 4\n");
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
