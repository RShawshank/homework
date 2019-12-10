#include<stdio.h>
#include<stdlib.h>
#include<string.h>
extern    void   THREE1(void);
extern    void   THREE2(void);
extern    void   RECOMMEND(void);
extern    void   RANK(void);
extern    void   INFORMATIONS(void);
extern    char   GA1;
extern int RECOMMEND_RANK;
void function5(char* GA1);
int  menu1();
int  menu2();
#define  N 3
#define EXIT -1
#define OK 2
#define FASLE 3
void function5(char *GA1)
{
	char* temp;
	int*num;
	int*rank;
    int i;
    temp=GA1;
	rank=&RECOMMEND_RANK;
    for(i=0; i<N; i++)
    {
		while(*temp!='\0'&&*temp!='$')
        {
            putchar(*temp);
            temp++;
        }
        while(*temp!='$')
            temp++;
        temp++;
		printf("  information:  ");
        printf("%d ",*temp);
        temp+=1;
		num=temp;
        printf("%d ",*num);
        num++;
		printf("%d ",*num);
		num++;
        printf("%d ",*num);
        num++;
        printf("%d ",*num);
        num++;
        printf("%d \n",*num);
        printf("Recommend degree Rank:%d\n",*rank);
		rank+=1;
		temp+=10;		
    }
}
int  menu1()
{
	int op = 1;
	while (op)
	{
		system("cls");
		printf("              menu\n");
		printf("            1.check for goods information\n");
		printf("            2.change the goods information\n");
		printf("            3.calculate the recommendation\n");
		printf("            4.calculate the recommendation rank\n");
		printf("            5.output all goods information\n");
		printf("            6.exit\n");
		printf("            please choose one of 1-6\n");
		scanf("%d", &op);
		getchar();
		switch (op)
		{
		case 1:
			THREE1();
			printf("\n");
			break;
		case 2:
			THREE2();
			break;
		case 3:
			RECOMMEND();
			break;
		case 4:
			RANK();
			break;
		case 5:
			function5(&GA1);
			break;
		case 6:
		op=0;
			break;
		}
	}
	return OK;
}
int  menu2()
{
	int op = 1;
	while (op)
	{
		system("cls");
		printf("              menu\n");
		printf("            1.check for goods information\n");
		printf("            6.exit\n");
		printf("            please choose one of 1 or 6\n");
		scanf("%d", &op);
		getchar();
		switch (op)
		{
		case 1:
			THREE1();
			break;
		case 6:
		op=0;
			break;
		}
	}
	return OK;
}

int main()
{
	int auth = 0;
	int i;
	char name[20]={0};
	char password[10]={0};
	char name_[20] = { "raohongbin\n" };
	char password_[10] = {"test\n"};
	printf("SHOP\n");
	do {
		for(i=0;i<20;i++)
		name[i]=0;
	for(i=0;i<10;i++)
		password[i]=0;
		printf("please input the name?\n");
		   fgets(name, 15, stdin);
			if (name[0] == 'p')
				return EXIT;
			else if (name[0] == '\n')
			{
				auth = 0;
				break;
			}
			else
			{
				printf("please input the password?\n");
				fgets(password,10,stdin);
				auth = 1;
			}
	} while (strcmp(password,password_)!=0||strcmp(name,name_)!=0);
		if (auth == 1)
		{
			return menu1();
		}
		else
		{
			return	menu2();
		}
	return 0;
}