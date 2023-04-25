#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int arraySize = 1000;
double precision = 0.1;
int numOfThreads = 0;
int iterations = 0;

int haveRandom = 1;

//for checking if array is correct
void PrintArray(double **a){
	printf("Two Dimensional array elements:\n");
	for(int k=0; k<arraySize; k++) {
	  for(int l=0;l<arraySize;l++) {
		 printf("%f ", a[k][l]);
	  }
	  printf("\n");
	}
}

//assigns ones to the top and left edges of the array
void SetupArray(double **a, double **b){
	double randomNum;
	for (int i = 0; i < arraySize; i++){
		a[i][0] = 1;
		a[0][i] = 1;
		a[i][arraySize-1] = 1;
		a[arraySize-1][i] = 1;
		b[i][0] = 1;
		b[0][i] = 1;
		b[i][arraySize-1] = 1;
		b[arraySize-1][i] = 1;
		if (haveRandom == 1){
			for (int j = 0; j < arraySize; j++){
				if (i > 0 & i < arraySize-1){
					if (j > 0 & j < arraySize-1){
						randomNum = ((double) rand()/(double) RAND_MAX);
						a[i][j] = randomNum;
						b[i][j] = randomNum;
					}
				}
			}
		}
	}
}

//swaps the references to each array
void SwapArray(double ***a, double ***b) {
    double **temp = *a;
    *a = *b;
    *b = temp;
}

int main(int argc, char *argv[])
{
	if (argc > 1){
		arraySize = atoi(argv[1]);
		numOfThreads = atoi(argv[2]);
		precision = atof(argv[3]);
	}
	//creation of arrayA
	double **arrayA = malloc(arraySize*sizeof(double*));
	double *bufA = malloc(arraySize*arraySize*sizeof(double));
	for (int i = 0; i < arraySize; i++) {
	arrayA[i] = bufA + arraySize*i;
	}
	
	//creation of arrayB
	double **arrayB = malloc(arraySize*sizeof(double*));
	double *bufB = malloc(arraySize*arraySize*sizeof(double));
	for (int i = 0; i < arraySize; i++) {
	arrayB[i] = bufB + arraySize*i;
	}
	
	//initialises identical values for both arrayA and arrayB
	SetupArray(arrayA,arrayB);
	
	//counter counts the number of cells that are within precision
	int counter = 0;
	
	//performs the relaxation
	while (counter < ((arraySize-2)*(arraySize-2))){
		counter = 0;
		for (int i = 1; i < arraySize-1; i++){
			for (int j = 1; j < arraySize-1; j++){
				//performs the averaging
				double total = arrayA[i+1][j] + arrayA[i-1][j] + arrayA[i][j+1] + arrayA[i][j-1];
				arrayB[i][j] = total/4;
				
				//checks if the cell in the new array is within the precision
				if (fabs(arrayB[i][j] - arrayA[i][j]) <= precision){
					counter++;
				}
			}
		}
		iterations+=1;
		SwapArray(&arrayA, &arrayB);
	}
	
	printf("sequential\n");
	PrintArray(arrayA);
	printf("Dimention: %d\n", arraySize);
	printf("Iterations: %d\n", iterations);
	printf("Precision: %f\n", precision);
	
	free(arrayA);
	free(bufA);
	
	free(arrayB);
	free(bufB);
	
	return 0;
}
