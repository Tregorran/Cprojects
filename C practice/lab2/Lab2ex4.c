#include <stdio.h>
#include <math.h>

void testEvenPrime(int number);

//main method passing the numbers in the function testEvenPrime
int main()
{
	int number = 11;
	testEvenPrime(number);
	
	number = 74;
	testEvenPrime(number);
	
	number = 307;
	testEvenPrime(number);
	
	number = 7402;
	testEvenPrime(number);
	
	number = 9357;
	testEvenPrime(number);
	return 0;
}

//testEvenPrime function that tests if the number passed is even or odd and 
//if the number is prime or not prime
void testEvenPrime(int number)
{
	if (number % 2 == 0)
	{
	printf("%d is even ", number);
	}
	else
	{
	printf("%d is odd ", number);
	}
	
	for (int i = 2; i < number; i++)
	 {
		 if (number % i == 0)
		 {
			 printf("and not prime.\n");
			 return;
		 }
	 }
	 if (number == 1)
		 {
			 printf("and not prime.\n");
			 return;
		 }
	 printf("and prime.\n");
	 return;
}