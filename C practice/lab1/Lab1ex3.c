#include <stdio.h>
int main()
{
	int height = 101;
	int feet = height / 30.48;
	int inches = height / 2.54;
	inches = inches - feet*12;
	printf ("%d cm is %d feet %d inches to the nearest inch\n", height, feet, inches);


	height = 3;
	feet = height / 30.48;
	inches = height / 2.54;
	inches = inches - feet*12;
	printf ("%d cm is %d feet %d inches to the nearest inch.\n", height, feet, inches);

	height = 15;
	feet = height / 30.48;
	inches = height / 2.54;
	inches = inches - feet*12;
	printf ("%d cm is %d feet %d inches to the nearest inch.\n", height, feet, inches);
	
	height = 92;
	feet = height / 30.48;
	inches = height / 2.54;
	inches = inches - feet*12;
	printf ("%d cm is %d feet %d inches to the nearest inch.\n", height, feet, inches);
	
	height = 24;
	feet = height / 30.48;
	inches = height / 2.54;
	inches = inches - feet*12;
	printf ("%d cm is %d feet %d inches to the nearest inch.\n", height, feet, inches);
	
	return 0;
}
