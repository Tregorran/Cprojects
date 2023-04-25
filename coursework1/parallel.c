#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int arraySize = 20;
int numOfThreads = 2;
double precision = 0.1;
int haveRandom = 1;

pthread_barrier_t barrier;
int *threadPrecision;
int allWithinPrecision = 0;

int iterations = 0;

//each thread will have data to store per loop
typedef struct threadData{
	int id;
	double **arrayA;
	double **arrayB;
	int numOfCellsRelax;
	int startRow;
	int startCol;
} threadsData;

//for checking if array is correct
void PrintArray(double **a){
	printf("Two Dimensional array elements:\n");
	for(int k = 0; k < arraySize; k++) {
	  for(int l = 0;l < arraySize;l++) {
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

//distributes the cells for each thread to relax
void DistributeCells(threadsData *threadData, double **a, double **b){
	int *cellSize = malloc(numOfThreads*sizeof(int));
	
	//calculates the number of cells per thread
	int cellCount = (arraySize - 2) * (arraySize - 2);
	int cellsPerThread = cellCount / numOfThreads;
	
	//assigns the number of cells per thread
	for (int i = 0; i < numOfThreads; i++){
		cellSize[i] = cellsPerThread;
	}
	
	//divides the remaining cells between the threads
	int cellsRemaining = cellCount % numOfThreads;
	int threadNum = 0;
	while (cellsRemaining > 0){
		cellSize[threadNum] += 1;
		cellsRemaining -= 1;
		
		threadNum += 1;
		
		if (threadNum == numOfThreads){
			threadNum = 0;
		}
	}
	
	//assigns the the data to each thread
	for (int j = 0; j < numOfThreads; j++){
		threadData[j].id = j;
        threadData[j].arrayA = a;
        threadData[j].arrayB = b;
        threadData[j].numOfCellsRelax = cellSize[j];
		//printf("Thread num: %d, group size %d\n", j, cellSize[j]);
	}
	
	free(cellSize);
}

//calculate the start cell location of each thread to start their relaxing process
void CalcStartCells(threadsData *threadData){
	int row = 0;
	int col = 0;
	int nextLoc = 0;

	for (int i = 0; i < numOfThreads; i++) {
		
		//assign starting row and col for the starting location of the relaxation process for each thread
		threadData[i].startRow = row + 1; 
		threadData[i].startCol = col + 1;
		
		//printf("thread num: %d, start row: %d, start col: %d\n", i, threadData[i].startRow, threadData[i].startCol);
		
		//calculate starting row and col for next thread
		nextLoc += threadData[i].numOfCellsRelax;
		row = nextLoc % (arraySize - 2);
		col = nextLoc / (arraySize - 2);
	}
}

//performs the averaging of the cells surrounding
void RelaxCell(double **arrayA, double **arrayB, int row, int col)
{
	double total = arrayA[row+1][col] + arrayA[row-1][col] + arrayA[row][col+1] + arrayA[row][col-1];
	total = total / 4;
	arrayB[row][col] = total;
}

//checks if the cell in the new array is within the precision
int CheckPrecision(double **arrayA, double **arrayB, int row, int col){
	if (fabs(arrayA[row][col]-arrayB[row][col]) <= precision){
		return 1;
	}
	return 0;
}

//checks all the threads that they are within precision
int CheckAllPrecisions(){
	for (int i = 0; i < numOfThreads; i++){
		if (threadPrecision[i] == 0){
			return 0;
		}
	}
	return 1;
}

//performs the relaxing process per thread
void *RelaxMatrix(threadsData *threadData){
	int id = threadData -> id;
	int cellsToRelax = threadData -> numOfCellsRelax;
	int startingRow = threadData -> startRow;
	int startingCol = threadData -> startCol;
	int matrixSize = arraySize;	
	int row = 0;
	int col = 0;
	int threadP;
	
	while(1){
		row = startingRow;
		col = startingCol;
		threadP = 1;
		
		//relaxes the cells assigned to the thread then checks their precisions
		for (int i = 0; i < cellsToRelax; i++){
			//printf("id: %d, relaxing: %d, %d\n ", threadData -> id, row, col);
			RelaxCell(threadData -> arrayA, threadData -> arrayB, row, col);
			
			if(!CheckPrecision(threadData -> arrayA, threadData -> arrayB, row, col)){
				threadP = 0;
			}
			
			//go to next cell
			row += 1;
			if (row == arraySize - 1) {
				row = 1;
                col += 1;
			}
		}
		
		//records the precision, will be 1 if all are within precision
		threadPrecision[threadData->id] = threadP;
		
		//printf("id: %d, barrier reached\n", id);
		pthread_barrier_wait (&barrier);
		
		//waits for main to check all precisions
		pthread_barrier_wait (&barrier);
		
		//thread exits when all cells are within precision
		if (allWithinPrecision){
			pthread_exit(NULL); 
		}
		
		//swaps the reference of the two arrays so the thread can restart the relaxing process
		SwapArray(&threadData -> arrayA, &threadData -> arrayB);
	}
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
	
	pthread_t *thread = malloc(numOfThreads*sizeof(pthread_t));
	threadsData *threadData = malloc(6*numOfThreads*sizeof(pthread_t));
	threadPrecision = malloc(numOfThreads*sizeof(int));
	
	DistributeCells(threadData, arrayA, arrayB);
	
	CalcStartCells(threadData);
	
	//initialise barrier plus main thread.
	if (pthread_barrier_init(&barrier, NULL, numOfThreads+1)) {
        printf("Initialisation of pthread_barrier failed");
    }
	
	//creation of threads running function relaxMatrix and passing the data of each thread
	for (int i =0; i < numOfThreads; i++){
		if (pthread_create(&thread[i], NULL, (void*(*)(void*))RelaxMatrix, &threadData[i])){
			printf("Failed creating thread: %d", i);
		}
	}
	
	while(!allWithinPrecision){
		//waits for all the threads to have completed their relaxation process
		pthread_barrier_wait (&barrier);
		
		//printf("iterations: %d\n", iterations);
		//PrintArray(arrayB);
		
		if (CheckAllPrecisions()){
			allWithinPrecision = 1;
		}
		
		iterations += 1;
		//allows the other process to then check if they can exit otherwise all loop
		pthread_barrier_wait(&barrier);
	}
	
	//printf("main part done\n");
	
	//halts the main thread until all other threads have terminated
	for (int i = 0; i < numOfThreads; i++) {
		if (pthread_join(thread[i], NULL)){
			printf("Thread: %d, failed in joining\n", i);
		}
	}
	
	//destroy all barriers after completing relaxing arrays 
    if (pthread_barrier_destroy(&barrier)) {
        printf("Destorying barriers failed");
    }
	
	printf("\nParallel\n");
	PrintArray(arrayA);
	printf("iterations: %d\n", iterations);
	printf("size of matrix: %d\n", arraySize);
	printf("Number of threads: %d\n", numOfThreads);
	printf("Precision: %f\n", precision);
	
	printf("\nParallel\n");
	PrintArray(arrayB);
	printf("iterations: %d\n", iterations);
	printf("size of matrix: %d\n", arraySize);
	printf("Number of threads: %d\n", numOfThreads);
	printf("Precision: %f\n", precision);
	
	free(arrayA);
	free(bufA);
	
	free(arrayB);
	free(bufB);
	
	free(threadData);
	free(thread);
	
	return 0;
}