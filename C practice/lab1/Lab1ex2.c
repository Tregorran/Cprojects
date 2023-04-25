#include <stdio.h>
int main()
{
	int x = 5;
	for ( int i = 0; i < 11; i++)
	{
		printf("%d\n", x+i);
	}
	
	x = 10;
	for ( int i = 0; i < 11; i++)
	{
		printf("%d\n", x+i);
	}
	
	x = 12;
	for ( int i = 0; i < 11; i++)
	{
		printf("%d\n", x+i);
	}
	
	return 0;
}

