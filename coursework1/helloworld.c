#include <stdio.h>
#include <stdlib.h>

int arraySize = 5;
double precision = 0.1;

int main()
{
	double **arrayA = malloc(arraySize*sizeof(double*));
	double *bufA = malloc(arraySize*arraySize*sizeof(double));
	for (int i = 0; i < arraySize; i++) {
	arrayA[i] = bufA + arraySize*i;
	}
	
	setupArray(arrayA);
	
	printArray(arrayA);
	
	free(arrayA);
	free(bufA);
	
	return 0;
}

/*int main()
{
	double **arrayA = malloc(arraySize*sizeof(double*));
	double *bufA = malloc(arraySize*arraySize*sizeof(double));
	for (int i = 0; i < arraySize; i++) {
	arrayA[i] = bufA + arraySize*i;
	}
	
	double **arrayB = malloc(arraySize*sizeof(double*));
	double *bufB = malloc(arraySize*arraySize*sizeof(double));
	for (int i = 0; i < arraySize; i++) {
	arrayB[i] = bufB + arraySize*i;
	}
	
	setupArray(arrayA);
	setupArray(arrayB);
	
	for (int i = 1; i < arraySize; i++){
		for (int j = 1; j < arraySize; j++){
			double total = arrayA[i+1][j] + arrayA[i-1][j] + arrayA[i][j+1] + arrayA[i][j-1];
			arrayB[i][j] = total/4;
		}
	}
	
	printArray(arrayB);
	
	
	
	free(arrayA);
	free(bufA);
	
	free(arrayB);
	free(bufB);
	
	return 0;
}*/
/*while () //outside precision threshold
	//relax array
	//set new precision
	//swap matrices(swap values with new relaxed array)
	calculate(a);
	end while*/

void printArray(double **a){
	printf("Two Dimensional array elements:\n");
	for(int k=0; k<arraySize; k++) {
	  for(int l=0;l<arraySize;l++) {
		 printf("%.1f ", a[k][l]);
	  }
	  printf("\n");
	}
}

void setupArray(double **a){
	for (int i = 0; i < arraySize; i++){
		a[i][0] = 1;
		a[i][arraySize-1] = 1;
		a[0][i] = 1;
		a[arraySize-1][i] = 1;
	}
}

/*void calculate(){
	
}*/
