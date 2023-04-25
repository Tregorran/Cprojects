#include <stdio.h>
#include <stdbool.h>

int isEven(int number);
void printer(bool decide);

//main method to put in the numbers into the isEven function
//and to send to the printer function if the number is true or false
int main()
{
	bool decide;	
	int number = 10;
	decide = isEven(number);
	printer(decide);
	
	number = 21;
	decide = isEven(number);
	printer(decide);
	
	number = 33;
	decide = isEven(number);
	printer(decide);
	
	number = 8;
	decide = isEven(number);
	printer(decide);
	
	number = 200;
	decide = isEven(number);
	printer(decide);
	return 0;
}

//This function calculates if the number is even or odd
int isEven(int number)
{

	if (number % 2 == 0)
	{
	return true;
	}
	else
	{
	return false;
	}
}

//this function prints out true or false if the number is even or odd
void printer(bool decide)
{
	if (decide == true)
	{
	printf("True\n");
	}
	else
	{
	printf("False\n");
	}
}