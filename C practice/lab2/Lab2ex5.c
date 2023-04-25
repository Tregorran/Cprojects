#include <stdio.h>

int multiply(int num1, int num2);

//main method passing the numbers into the function multiply and getting a 
//returned value which is printed
int main()
{
	int result = multiply(6,3);
	printf("%d\n",result);
	
	result = multiply(9,9);
	printf("%d\n",result);
	
	result = multiply(3,0);
	printf("%d\n",result);
	
	result = multiply(-1,7);
	printf("%d\n",result);
	
	result = multiply(-29,803);
	printf("%d\n",result);
	return 0;
}


//multiply function which multiplies the two numbers passed to it and returns the result
int multiply(int num1, int num2)
{
	if (num1>num2)
	{
		return multiply(num2, num1);
	}
	
	if (num1 == 0 || num2 == 0)
	{
		return 0;
	}
	
	if (num1 == 1)
	{
		return num2;
	}
	
	return multiply(num1, num2-1)+num1;
}