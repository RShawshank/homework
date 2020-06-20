#include "def.h"

extern struct symboltable symbolTable;

int main_flag = 0;//主函数标志位

void object_code(struct codenode *head) {
    char opnstr1[32],opnstr2[32],resultstr[32];//寄存器编号
    struct codenode *h=head,*p;
    int i;
    FILE *fp;
	/*
	如MARS，要求MIPS汇编程序的入口函数要放在最前面，
	可以考虑目标代码生成时，
	把main函数调整到最前面，或增加一个放在最前面的入口函数（假定为main0），
	在入口函数中调用main函数。*/
    fp=fopen("object.s","w");
    fprintf(fp,".data\n");
    fprintf(fp,"_Prompt: .asciiz \"Enter an integer:  \"\n");
    fprintf(fp,"_ret: .asciiz \"\\n\"\n");
    fprintf(fp,".globl main\n");
    fprintf(fp,".text\n");
    fprintf(fp, "j main\n");//跳转到main函数
    int cnt = 0;
    do{
        cnt++;
        switch (h->op)
        {
			//约定操作数使用$t1和$t2，运算结果使用$t3
        case ASSIGNOP:
            if(h->opn1.kind == INT) {//int型常量
                fprintf(fp, "  li $t3, %d\n", h->opn1.const_int);
            }
            fprintf(fp, "  sw $t3, %d($sp)\n", h->result.offset);
            break;
        case PLUS:
			fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
            fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
			fprintf(fp, "  add $t3, $t1, $t2\n");
			break;
        case MINUS:
			fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
            fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
			fprintf(fp, "  sub $t3, $t1, $t2\n");
			break;
        case STAR:

			fprintf(fp, "  mul $t3, $t1, $t2\n");
				break;
        case DIV:
			fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
            fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
			fprintf(fp, "  div $t1, $t2\n");
            fprintf(fp, "  mflo $t3\n");
		    break;
        case FUNCTION:
            if(!strcmp(h->result.id, "main")){
                main_flag = 1;
            }
            fprintf(fp, "\n%s:\n", h->result.id);
            if(!strcmp(h->result.id, "main"))
                fprintf(fp, "  addi $sp, $sp, -%d\n", symbolTable.symbols[h->result.offset].offset);//-main函数的活动记录大小
            break;

        case PARAM:
            break;
        case LABEL:
            fprintf(fp, "%s:\n", h->result.id);
            break;
        case GOTO:  
            fprintf(fp, "  j %s\n", h->result.id);
            break;
        case JLE:
			fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
            fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
			fprintf(fp, "  ble $t1,$t2,%s\n", h->result.id);
		break;
        case JLT:
			fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
            fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
            fprintf(fp, "  blt $t1,$t2,%s\n", h->result.id);
		break;
		case JGE:
			fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
            fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
            fprintf(fp, "  bge $t1,$t2,%s\n", h->result.id);
			break;
		case JGT:
			fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
            fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
			fprintf(fp, "  bgt $t1,$t2,%s\n", h->result.id);
        break;
		case EQ:
			fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
            fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
            fprintf(fp, "  beq $t1,$t2,%s\n", h->result.id);
		break;
        case NEQ:
			fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
            fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
			fprintf(fp, "  bne $t1,$t2,%s\n", h->result.id);
		break;
        case ARG:
            break;

        case RETURN:
            if(!main_flag){
                fprintf(fp, "  lw $v0,%d($sp)\n",h->result.offset); 
                fprintf(fp, "  jr $ra\n");
            }
            else{
                main_flag = 0;
            }
            
            break;

        case CALL:
            if (!strcmp(h->opn1.id,"read")){ 
                fprintf(fp, "  addi $sp, $sp, -4\n");
                fprintf(fp, "  sw $ra,0($sp)\n"); 
                fprintf(fp, "  jal read\n"); 
                fprintf(fp, "  lw $ra,0($sp)\n"); 
                fprintf(fp, "  addi $sp, $sp, 4\n");
                fprintf(fp, "  sw $v0, %d($sp)\n",h->result.offset);
                break;
            }
            if (!strcmp(h->opn1.id,"write")){ 
                fprintf(fp, "  lw $a0, %d($sp)\n",h->prior->result.offset);
                fprintf(fp, "  addi $sp, $sp, -4\n");
                fprintf(fp, "  sw $ra,0($sp)\n");
                fprintf(fp, "  jal write\n");
                fprintf(fp, "  lw $ra,0($sp)\n");
                fprintf(fp, "  addi $sp, $sp, 4\n");
                break;
            }
			//函数定义参数，获得各个实参值所存放的单元，取出后送到形式参数的单元中
            for(p = h,i = 0;i < symbolTable.symbols[h->opn1.offset].paramnum; i++)  
                p=p->prior;
                      
            fprintf(fp, "  move $t0,$sp\n"); 
            fprintf(fp, "  addi $sp, $sp, -%d\n", symbolTable.symbols[h->opn1.offset].offset);
            fprintf(fp, "  sw $ra,0($sp)\n"); 
            i=h->opn1.offset+1;  
            while (symbolTable.symbols[i].flag == 'P'){
                fprintf(fp, "  lw $t1, %d($t0)\n", p->result.offset);
                fprintf(fp, "  move $t3,$t1\n");
                fprintf(fp, "  sw $t3,%d($sp)\n",  symbolTable.symbols[i].offset);
                p=p->next; 
                i++;
            }
            fprintf(fp, "  jal %s\n",h->opn1.id);
            fprintf(fp, "  lw $ra,0($sp)\n");
            fprintf(fp, "  addi $sp,$sp,%d\n",symbolTable.symbols[h->opn1.offset].offset);
            fprintf(fp, "  sw $v0,%d($sp)\n", h->result.offset); 
            break;
        
        default:
            break;
        }
    h = h->next;
    }while(h != head);
    fclose(fp);
}