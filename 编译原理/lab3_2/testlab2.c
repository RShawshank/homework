int a, b, c;
float d, e, f;
char g;
int array[5];
float b;  //变量名重复定义(3)
char array[10];  //数组变量名重复定义（3）
int arr[-5][0];  //数组定义错误
int funcA(int i, int j) {
	float x = 1.0;
	return x;  //函数返回值类型与函数定义的返回值类型不匹配(16)
}

int funcA(int i, int j) {  //函数名重复定义(3)

}
int main() {
	k++;  //使用未定义的变量(1)
	z = y * 1;  //使用未定义的变量(1)
	NoFunc(a);  //调用未定义或未声明的函数(2)
	array[1][-1];  //数组访问出错(3)
	array[1]['A']; //数组访问出错(3)
	a(i, j);  //对非函数名采用函数调用形式（4）
	funcA--;  //对函数名采用非函数调用形式访问；(5)
	funcA(a, b, c); // 实参表达式个数太多(6)
	funcA(b);  //参表达式个数太少(6)	
	funcA(d, e);  //函数调用时实参和形参类型不匹配(7)
	a[1] = 5;  //对非数组变量采用下标变量的形式访问(8)
	arr[1.1];  //数组变量的下标不是整型表达式(9)
	arr['A'];  //数组变量的下标不是整型表达式(9)
	10=a;  //赋值号左边不是左值表达式(12)
	funcA(i, j)++;//对非左值表达式进行自增、自减运算(13)
	b = 1 + 'A';  //类型不匹配(15)
	d = 10 * 12.3;  //类型不匹配(15)
	break;  //break语句不在循环语句或switch语句中；(18)
	continue;  //continue语句不在循环语句中；(19)
//检查：
	funcA(i,j) = i;
	10++;
	++10;
	'A'--;
	--'A';
	'A' = g;  //赋值号左边不是左值表达式
	g = 'A';  //合法
	g--;
	--g;
	++g;
	g++;
	'A' += 1;  //复合运算类型匹配
	1.01 -= 'A';
	a *= 'A';
	1.11 /= 1;
	a[10] %= 1.0;
	a += 1.0;
	
	return 'A';  //函数返回值类型与函数定义的返回值类型不匹配；
}
