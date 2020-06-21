int a, b, c,i;
float d, e, f;
char gaaa;
int array[5];
char arr[10];  
int funcA(int i, int j) {
	int x = 1;
	return x;  
}
int funcB(int i, int j) { 
	return i+j;
}
int main() {
	//int i=0;
	a=0;
	b=0;
	g='A';
	b=funcA(a,b);
	array[0]=1;
	array[1]=a;
	array[2]=b;
	arr[1]='c';
	b--;	
	for(i=0;i<2;i++)
	{
		a-=1;
		b--;
	}
	c=2;
	while(c)
	{
		array[c]=1;
		c--;
	}
	a=2;
	switch(a)
	{
		case 2:
		b++;break;
		default:
		break;
	}
return 0; 
}//实验三测试代码