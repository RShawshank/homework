#include "def.h"
#include<string.h>
extern struct symboltable symbolTable;
int call_flag = 0;
int main_call=0;
int LEV;
int main_flag = 0;//主函数标志位
//截取指定长度的字符数组
int sublen(char a[],char b[],int start,int len)
{
	/*strncpy(a,b+start,len);
	a[len]='\0';*/
	char* p = b + start;   //定义指针变量指向需要提取的字符的地址
	int n = strlen(p);       //求字符串长度
	int i = 0;
		if(n < len)
		{
			len = n;
		}
		while(len != 0)
		{
			a[i] = b[i+start];
			len --;
			i++;
		}                        //复制字符串到dst中
		a[i] = '\0';
	return 0;
}

int searchSymbolTableByAlias(char *alias)
{
    int i;
    for (i = symbolTable.index - 1; i >= 0; i--)
        if (!strcmp(symbolTable.symbols[i].alias, alias))
            return i;
    return -1;
}
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
	fprintf(fp,"read:\n");
    fprintf(fp,"  li $v0,4\n");
    fprintf(fp,"  la $a0,_Prompt\n");
    fprintf(fp,"  syscall\n");
    fprintf(fp,"  li $v0,5\n");
    fprintf(fp,"  syscall\n");
    fprintf(fp,"  jr $ra\n");
    fprintf(fp,"write:\n");
    fprintf(fp,"  li $v0,1\n");
    fprintf(fp,"  syscall\n");
    fprintf(fp,"  li $v0,4\n");
    fprintf(fp,"  la $a0,_ret\n");
    fprintf(fp,"  syscall\n");
    fprintf(fp,"  move $v0,$0\n");
    fprintf(fp,"  jr $ra\n");
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
            else if(h->opn1.kind == FLOAT) {
                fprintf(fp, "  li $t3, %f\n", h->opn1.const_float);
            }
            else if(h->opn1.kind == CHAR) {
                fprintf(fp, "  li $t3, %c\n", h->opn1.const_char);
            }
			else{
                int SymbolTable;
                char array[20];
                if(strstr(h->opn1.id, " offset：") != NULL) {
                    sublen(array, h->opn1.id, 0, 2);
                    SymbolTable = searchSymbolTableByAlias(array);
                }
                else
                    SymbolTable = searchSymbolTableByAlias(h->opn1.id);
                if(SymbolTable != -1 && symbolTable.symbols[SymbolTable].level == 0){
                    fprintf(fp, "  lw $t1, %d($gp)\n", h->opn1.offset);
                    fprintf(fp, "  move $t3, $t1\n");
                }
                else{
                    fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
                    fprintf(fp, "  move $t3, $t1\n");
                }
            }
			//数组
            int SymbolTable;
            char array[20] = {'\0'};
            if(strstr(h->result.id, " offset：") != NULL) {
                for(int i = 0; i < strlen(h->result.id); i++){
                    if(h->result.id[i] != ' '){
                        array[i] = h->result.id[i];
                    }
                }
                SymbolTable = searchSymbolTableByAlias(array);
            }
            else
                SymbolTable = searchSymbolTableByAlias(h->result.id);
            if(SymbolTable != -1 && symbolTable.symbols[SymbolTable].level == 0){
                fprintf(fp, "  sw $t3, %d($gp)\n", h->result.offset);
            }
            else{
                fprintf(fp, "  sw $t3, %d($sp)\n", h->result.offset);
            }
            break;
        case PLUS:
			fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
            fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
			fprintf(fp, "  add $t3, $t1, $t2\n");
			fprintf(fp, "  sw $t3, %d($sp)\n", h->result.offset);
			break;
        case MINUS:
			fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
            fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
			fprintf(fp, "  sub $t3, $t1, $t2\n");
            fprintf(fp, "  sw $t3, %d($sp)\n", h->result.offset);		
		break;
        case STAR:
			fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
            fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
			fprintf(fp, "  mul $t3, $t1, $t2\n");
            fprintf(fp, "  sw $t3, %d($sp)\n", h->result.offset);
			break;
        case DIV:
			fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
            fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
			fprintf(fp, "  div $t1, $t2\n");
            fprintf(fp, "  mflo $t3\n");
            fprintf(fp, "  sw $t3, %d($sp)\n", h->result.offset);
		    break;
		case MOD:
			fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
            fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
			fprintf(fp, "  div $t1, $t2\n");
            fprintf(fp, "  mfhi $t3\n");
            fprintf(fp, "  sw $t3, %d($sp)\n", h->result.offset);
			break;
        case FUNCTION:
            if(!strcmp(h->result.id, "main")){
                main_flag = 1;
				LEV=0;
				main_call=0;
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
				fprintf(fp, "  li $v0, 10\n");
                fprintf(fp, "  syscall\n");
                main_flag = 0;
            }
			call_flag = 0;
			LEV--;

            break;

        case CALL:
		    call_flag = 1;
            LEV++;
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