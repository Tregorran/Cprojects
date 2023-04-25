#include <stdio.h>

int forLoop(int number);
int whileLoop(int number);

//main method that passes parameter into the functions
int main()
{
	forLoop(1);
	forLoop(2);
	forLoop(4);
	forLoop(5);
	forLoop(8);
	whileLoop(1);
	whileLoop(2);
	whileLoop(4);
	whileLoop(5);
	whileLoop(8);
	return 0;
}

//calculates the factorial of the parameter given using a for loop
int forLoop(int number)
{
	for(int i=number-1; i>0 ;i--)
	{
		number = number * i;
	}
	printf("%d\n", number);
}

//calculates the factorial of the parameter given using a while loop
int whileLoop(int number)
{
	int num = number;
	while(num>1)
	{
		num=num-1;
		number=number*num;
	}
	printf("%d\n", number);
}