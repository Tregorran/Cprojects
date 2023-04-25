#include <stdio.h>
#include <math.h>

double calculateDistance(double xOne, double yOne, double xTwo, double yTwo);

//main method which passes in the numbers to the function calculateDistance 
//and when returned prints the result
int main()
{
	printf("%1.2f\n", calculateDistance(0, 0, 4, 3));
	printf("%1.2f\n", calculateDistance(21, 3, 7, 9));
	printf("%1.2f\n", calculateDistance(-2, 5, 17, 1));
	printf("%1.2f\n", calculateDistance(-13, -5, 9, 2));
	printf("%1.2f\n", calculateDistance(-6, -4, -14, 19));
	return 0;
}

//calculateDistance function which calculates the distance between the two points 
//given and returns the result
double calculateDistance(double xOne, double yOne, double xTwo, double yTwo)
{
	double minusX = xTwo-xOne;
	double minusY = yTwo-yOne;
	double squareX = pow(minusX,2);
	double squareY = pow(minusY,2);
	double addXY = squareX + squareY;
	double result = sqrt(addXY);
	return result;
}