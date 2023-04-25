#include <stdio.h>

void arrow(int width, int length);
void star(int num);
void blank(int spaces);

//passes the width of the tree and length of the trunk to the funciton arrow
int main()
{
	arrow(9,4);
	arrow(5,2);
	arrow(11,6);
	return 0;
}

//function that calculates the shape of the tree and trunk
void arrow(int width, int length)
{
	int spaces;
	for(int i=1; i<=width; i+=2)
	{
		spaces = (width-i)/2;
		blank(spaces);
		star(i);
	}
	
	for(int i=length; i>0; i--)
	{
		spaces = (width-3)/2;
		blank(spaces);
		printf("***\n");
	}
	printf("\n");
}

//outputs blank spaces
void blank(int spaces)
{
	while(spaces>0)
	{
		printf(" ");
		spaces-=1;
	}
}

//outputs the stars for the tree and trunks
void star(int num)
{
	while(num>0)
	{
		printf("*");
		num-=1;
	}
	printf("\n");
}

