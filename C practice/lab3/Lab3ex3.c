#include <stdio.h>

void multiTable(int N);

//main method that passes the parameters to the function multiTable
int main()
{
	multiTable(5);
	multiTable(3);
	multiTable(8);
	return 0;
}

//creates array and calculates and outputs the multiplication table from
//the parameters given
void multiTable(int N) {
	
	int array2d[N+1][N+1];
	for (int i=1; i<=N; i++)
	{
		for(int j=1; j<=N; j++)
		{
			array2d[i][j] = i*j;
			printf("%d", array2d[i][j]);
			if (j<N)
			{
				printf(",");
			}
		}
		printf("\n");
	}
	printf("\n");
}