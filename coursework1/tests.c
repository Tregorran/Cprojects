#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int arraySize = 5;
double precision = 0.001;
int numOfThreads = 2;
pthread_barrier_t barrier;
char *threadPrecision;
int outside_threshold;

typedef struct threadData{
	int id;
	int numOfCellsRelax;
	int startRow;
	int startCol;
	double **arrayA;
	double **arrayB;
} threadsData;

void PrintArray(double **a){
	printf("Two Dimensional array elements:\n");
	for(int k = 0; k < arraySize; k++) {
	  for(int l = 0;l < arraySize;l++) {
		 printf("%.3f ", a[k][l]);
	  }
	  printf("\n");
	}
}

void SetupArray(double **a){
	for (int i = 0; i < arraySize; i++){
		a[i][0] = 1;
		a[0][i] = 1;
		//a[i][arraySize-1] = 1;
		//a[arraySize-1][i] = 1;
	}
}

void SwapArray(double ***a, double ***b) {
    double **temp = *a;
    *a = *b;
    *b = temp;
}

void DistributeCells(threadsData *threadData, double **a, double **b){
	int cellSize[numOfThreads];
	
	//calculates number of cells per thread
	int cellCount = (arraySize - 2) * (arraySize - 2);
	int cellsPerThread = cellCount / numOfThreads;
	
	//assigns the number of cells per thread
	for (int i = 0; i < numOfThreads; i++){
		cellSize[i] = cellsPerThread;
	}
	
	//divides the remaining cells between the threads
	/*int cellsRemaining = cellCount % numOfThreads;
	int threadGroup = 0;
	while (cellsRemaining > 0){
		cellSize[threadGroup] += 1;
		cellsRemaining -= 1;
		
		if (cellSize[threadGroup] == numOfThreads - 1){
			threadGroup = 0;
		} else {
			threadGroup += 1;
		}
	}*/
	
	/*int cellsRemaining = cellCount % numOfThreads;
	int threadGroup = 0;
	while (cellsRemaining > 0){
		cellSize[threadGroup] += 1;
		cellsRemaining -= 1;
		
		threadGroup += 1;
		
		if (threadGroup == numOfThreads){
			threadGroup = 0;
		}
	}*/
	
	//assigns the the data to each thread
	for (int j = 0; j < numOfThreads; j++){
		threadData[j].id = j;
        threadData[j].arrayA = a;
        threadData[j].arrayB = b;
        threadData[j].numOfCellsRelax = cellSize[j];
		printf("Thread num: %d, group size %d\n", j, cellSize[j]);
	}
}

//calculate the start cell of each thread to start their relaxing process
void CalcStartCells(threadsData *threadData){
	int next = 0, row = 0, col = 0;

	for (int i=0; i < numOfThreads; i++) {

		// Set starting cell location of each thread of where to start their relaxation
		
		
		threadData[i].startRow = row + 1; 
		threadData[i].startCol = col + 1;
		
		printf("thread num: %d, start row: %d, start col: %d\n", i, threadData[i].startRow, threadData[i].startCol);
		
		/* find starting cell for next thread */
		
		next += threadData[i].numOfCellsRelax;
		row = next % (arraySize - 2);
		col = next / (arraySize - 2);
	}
}

double RelaxCell(double **array, int row, int col)
{
	double total = array[row+1][col] + array[row-1][col] + array[row][col+1] + array[row][col-1];
	total = total / (double) 4;
    return total;
}

int CheckPrecision(double **arrayA, double **arrayB, int row, int col){
	if (fabs(arrayA[row][col]-arrayB[row][col]) > precision){
		return 0;
	}
	return 1;
}

///////////////////////////////////////////////////////////////////////////////////remake
/* checks elements of the precision array corresponding to matrix segments */
/* returns 0 if a single cell isn't in precision, 1 otherwise */
int CheckAllPrecisions(char *precision){
	for (int i = 0; i < numOfThreads; i++){
		if (!threadPrecision[i]){
			return 0;
		}
	}
	return 1;
}

void *RelaxMatrix(threadsData *threadData){
	int cellsToRelax = threadData -> numOfCellsRelax;
	int startingRow = threadData -> startRow;
	int startingCol = threadData -> startCol;
	int matrixSize = arraySize;	
	int id = threadData -> id;
	int row = 0;
	int col = 0;
	int p;
	
	while(1){
		printf("id: %d, loop\n", id);
		//reset variables
		row = startingRow;
		col = startingCol;
		p=1;
		
		for(int i = 0; i < cellsToRelax; i++)
		{
			printf("id: %d, relaxing: %d, %d\n ", threadData -> id, row, col);
            // Relax current cell and check if within desired precision
            threadData->arrayB[row][col] = RelaxCell(threadData->arrayA, row, col);
			
            /* p is assumed to be in precision, change if it isn't */
			if(!CheckPrecision(threadData->arrayA,threadData->arrayB, row, col)){
				p = 0;
			}
			
            // Wrap around to next row if current row finished
			row++;
            if (row == arraySize-1) {
				row = 1;
                col++;
			}
			printf("finished loop");
		}
		
		/* record if all cells are in precision to global array */
		threadPrecision[threadData->id] = p;
		
		printf("id: %d, barrier reached\n", id);
		pthread_barrier_wait (&barrier);
		pthread_barrier_wait (&barrier);
		
		if (outside_threshold){
			printf("outside threshold");
		}
		else{
			pthread_exit(NULL); 	/* precision reached */
		}
		
		SwapArray(&threadData -> arrayA, &threadData -> arrayB);
	}
	
	/*int counter = 0;
	while (counter < ((arraySize-2)*(arraySize-2))){
		printf("loop \n");
		counter = 0;
		for (int i = 1; i < arraySize-1; i++){
			for (int j = 1; j < arraySize-1; j++){
				double total = arrayA[i+1][j] + arrayA[i-1][j] + arrayA[i][j+1] + arrayA[i][j-1];
				arrayB[i][j] = total/4;
				
				if ((arrayB[i][j] - arrayA[i][j]) <= precision){
					counter++;
				}
			}
		}
		printArray(arrayB);
		swapArray(&arrayA, &arrayB);
	}
	
	printf("final matrix\n");
	printArray(arrayA);*/
}

int main()
{
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
	SetupArray(arrayA);
	SetupArray(arrayB);
	
	pthread_t *thread = malloc(numOfThreads*sizeof(pthread_t));
	threadsData *threadData = malloc(numOfThreads*sizeof(pthread_t));
	threadPrecision = malloc(numOfThreads*sizeof(char));
	
	//divides cells between threads
	
	DistributeCells(threadData, arrayA, arrayB);//done
	
	CalcStartCells(threadData);//done maybe
	
	//initialise barrier plus main thread.
	if (pthread_barrier_init(&barrier, NULL, numOfThreads+1)) {
        printf("Initialisation of pthread_barrier failed");
    }
	
	//creation of threads and calling relaxMatrix and passing data of each threads
	for (int i =0; i < numOfThreads; i++){
		if (pthread_create(&thread[i], NULL, (void*(*)(void*))RelaxMatrix, &threadData[i])){
			printf("Failed creating thread: %d", i);
		}
	}
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	int iterations = 0;
	
	printf("perform loop\n");
	while(1){
		outside_threshold = 1;
		iterations++;
		
		pthread_barrier_wait (&barrier);
		
		if (CheckAllPrecisions(threadPrecision)){
			outside_threshold = 0;
			pthread_barrier_wait(&barrier); /* sync threads and exit */
			break;
		}
		
		pthread_barrier_wait(&barrier); /* sync threads and loop */
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////
	
	//printf("main part done\n");
	//printArray(arrayB);
	
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
	
	PrintArray(arrayA);
	PrintArray(arrayB);
	
	free(arrayA);
	free(bufA);
	
	free(arrayB);
	free(bufB);
	
	free(threadData);
	free(thread);
	
	return 0;
}