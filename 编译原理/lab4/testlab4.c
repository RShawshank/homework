int a, b, c;
float m, n;
int d[10][10];

int fibo(int a) {
	if (a == 1 || a == 2)
		return 1;
	return fibo(a - 1) + fibo(a - 2);
}

int main(int argc)
{
	int i, temp;
	
	for(i = 1;i<10;i++)
	{
		if(i>9)
			{
				d[i][i]=i;
				temp = fibo(i);
				write(temp);
				break;
			} 
			else
			{
					temp = fibo(i);
					write(temp);
					continue;
			}
			i++;
	}
	return 1;
}