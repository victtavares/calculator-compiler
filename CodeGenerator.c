#include <stdio.h>
#include "Structure.h"
#include "y.tab.h"


int ex(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeCon:       
        printf("li $a0 %d\n", p->con.value);
        break;//return p->con.value;
    case typeId:
        printf("li $a0 %c\n", p->id.i + 'a');      
    //printf("li $a0%d\n", sym[p->id.i]);
        break;//return sym[p->id.i];
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:     while(ex(p->opr.op[0])) ex(p->opr.op[1]); return 0;
        case IF:        if (ex(p->opr.op[0]))
                            ex(p->opr.op[1]);
                        else if (p->opr.nops > 2)
                            ex(p->opr.op[2]);
                        return 0;
        case PRINT:     
            ex(p->opr.op[0]);
            printf("print\n");
            break;
        case ';':       //ex(p->opr.op[0]); return ex(p->opr.op[1]);
        case '=':       return sym[p->opr.op[0]->id.i] = ex(p->opr.op[1]);
        case UMINUS:    return -ex(p->opr.op[0]);
        case '+': 
            ex(p->opr.op[0]);      
            printf("sw $a0 0($sp)\naddiu $sp $sp -4\n");
            ex(p->opr.op[1]);
            printf("lw $t1 4($sp)\n");
            printf("add $a0 $t1 $a0 addiu $sp $sp 4\n");
            break;
            //return ex(p->opr.op[0]) + ex(p->opr.op[1]);
        case '-':       return ex(p->opr.op[0]) - ex(p->opr.op[1]);
        case '*':       return ex(p->opr.op[0]) * ex(p->opr.op[1]);
        case '/':       return ex(p->opr.op[0]) / ex(p->opr.op[1]);
        case '<':       return ex(p->opr.op[0]) < ex(p->opr.op[1]);
        case '>':       return ex(p->opr.op[0]) > ex(p->opr.op[1]);
        case GE:        return ex(p->opr.op[0]) >= ex(p->opr.op[1]);
        case LE:        return ex(p->opr.op[0]) <= ex(p->opr.op[1]);
        case NE:        return ex(p->opr.op[0]) != ex(p->opr.op[1]);
        case EQ:        return ex(p->opr.op[0]) == ex(p->opr.op[1]);
        }
    }
    return 0;
}
