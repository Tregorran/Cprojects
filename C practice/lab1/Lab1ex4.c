#include <stdio.h>
int main()
{
	int previous = 0;
	int current = 1;
	int next = 0;
	for (int i = 0; i < 10; i++)
	{
		printf("%d\n", previous);
		next = previous + current;
		previous = current;
		current = next;
	}
	
	return 0;
}