#include <stdio.h>
#include <math.h>
int main()
{
	float height = 7.0;
	float radius = 4.0;
	float vol = 0;
	vol = M_PI * pow(radius,2) * height;
	printf("The cylinder with height %1.1f cm and radius %1.1fcm has a volume of %1.2fcm^3\n", height, radius, vol);

	height = 20.0;
	radius = 3.0;
	vol = M_PI * pow(radius,2) * height;
	printf("The cylinder with height %1.1f cm and radius %1.1fcm has a volume of %1.2fcm^3\n", height, radius, vol);
	
	height = 14.7;
	radius = 5.2;
	vol = M_PI * pow(radius,2) * height;
	printf("The cylinder with height %1.1f cm and radius %1.1fcm has a volume of %1.2fcm^3\n", height, radius, vol);
	
	return 0;
}