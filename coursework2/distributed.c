#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int arraySize = 5000;
double precision = 0.01;
int haveRandom = 0;

int iterations = 0;

//for checking if array is correct
void PrintArray(double *a){
	printf("Finial config\n");
		for (int i = 0; i < arraySize * arraySize; i++){
			if (i % arraySize == 0){
				printf("\n");
			}
			printf("%f ", a[i]);
		}
		printf("DONE\n");
}

//assigns ones to the top and left edges of the array
void SetupArray(double *a, double *b){
	double randomNum;
	for (int i = 0; i < (arraySize*arraySize); i++){
		/*if (i < arraySize + 1){
			//a[i] = 1;
			//b[i] = 1;
		}
		else if (i % arraySize == 0){
			//a[i] = 1;
			//b[i] = 1;
		}*/
		
		if (i < arraySize){
			a[i] = 1;
			b[i] = 1;
		}
		
		if (i > arraySize*arraySize - arraySize - 1){
			a[i] = 1;
			b[i] = 1;
		}
		
		if (haveRandom == 1){
			if (i > arraySize-1){
				if (i < arraySize * arraySize - arraySize){
					if (!(i % (arraySize) == 0)){
						if (!((i+1) % arraySize == 0)){
							randomNum = ((double) rand()/(double) RAND_MAX);
							//printf("Random Num: %f\n", randomNum);
							a[i] = randomNum;
							b[i] = randomNum;
						}
					}
				}
			}
		}
	}
}

//swaps the references to each array
void SwapArray(double **arrayA, double **arrayB) {
    double *temp = *arrayA;
    *arrayA = *arrayB;
    *arrayB = temp;
}

//each processor checks the precision of their own segment
int precisionCheck(double* original, double* sendbuf, int Num, int rank){
	//loops through their segment
	for (int i = 0; i < Num; i++){
		if (fabs(original[i] - sendbuf[i]) > precision){
			return 1;
		}
	}
	return 0;
}

//checking the precision of all segments
int checkTotalPrecision(int* totalPrecision, int NumOfProcessors){
	for (int i = 0; i < NumOfProcessors; i++){
		if (totalPrecision[i] == 1){
			return 1;
		}
	}
	return 0;
}

//performs the relaxation calculations for the segment
void relaxCells(double* recvbuf, double* sendbuf, double* original, int displacement, int rank, int recvSize){
	
	//to start where the relaxing begins
	int start = arraySize;
	
	//end before reaching cells not needing to relax
	int end = recvSize - arraySize;
	
	//the index of each cell to be gathered
	int counter = 0;
	
	displacement = displacement + arraySize;
	
	for (int i = start; i <= end - 1; i++){
		if (((i + displacement) % arraySize != 0) && ((i+1 + displacement) % arraySize != 0)){
			
			sendbuf[counter] = (recvbuf[i-1] + recvbuf[i+1] + recvbuf[i+arraySize] + recvbuf[i-arraySize])/4;
			
			//used to compare with the new relaxed segment
			original[counter] = recvbuf[i];
		}else{
			sendbuf[counter] = recvbuf[i];
			
			//used to compare with the new relaxed segment
			original[counter] = recvbuf[i];
		}
		counter += 1;
	}	
}

int main(int argc, char **argv)
{
	//for recording the execution time
	double start = MPI_Wtime();
	
	//to check if there were errors with MPI
	int rc;
	
	int NumOfProcessors;
	int rank;
	
	//creation of arrayA and arrayB
	double *arrayA = malloc(arraySize*arraySize*sizeof(double));
	double *arrayB = malloc(arraySize*arraySize*sizeof(double));
	
	//initialises identical values for both arrayA and arrayB
	SetupArray(arrayA,arrayB);
	
	//initialise connections
	rc = MPI_Init(&argc, &argv);
	if (rc != MPI_SUCCESS){
		printf("Error starting MPI program\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	
	
	rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rc != MPI_SUCCESS){
		printf("Error in Rank\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	
	rc = MPI_Comm_size(MPI_COMM_WORLD, &NumOfProcessors);
	if (rc != MPI_SUCCESS){
		printf("Error Comm size\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	
	//prints initial array for checking
	/*if (rank == 0){
		PrintArray(arrayA);
		printf("Number of processors: %d\n", NumOfProcessors);
	}*/
	
	// allocate arrays for communication between processes
	int *displacement = malloc((NumOfProcessors)*sizeof(int));
	int *sendNum = malloc((NumOfProcessors)*sizeof(int));
	int *gatherDisplacement = malloc((NumOfProcessors)*sizeof(int));
	int *gatherNum = malloc((NumOfProcessors)*sizeof(int));
	
	// allocate arrays for gathering percision between processes
	int *gatherDisplacement2 = malloc((NumOfProcessors)*sizeof(int));
	int *gatherNum2 = malloc((NumOfProcessors)*sizeof(int));
	int *withinPrecision = malloc(1 * sizeof(int));
	withinPrecision[0] = 0;
	
	//give each processor the cells to relax and neightbouring cells above and below
	//everytime it exceeds (sendNum / (arraySize - 2)) * 2
	int numOfInnerCells = (arraySize - 2) * (arraySize - 2);
	int cellsPerProcessor = (numOfInnerCells / NumOfProcessors);
	
	//for knowing when I need to add on the edge numbers
	int total = 0;
	int addon = 0;
	
	//size of the array for each segment
	int recvSize;
	int sendSize;
	for (int i = 0; i < NumOfProcessors; i++){
		//which number in the array is which processor precision for their segment
		gatherDisplacement2[i] = i;
		gatherNum2[i] = 1;
		
		//allocating how many cells for each processor to relax
		if (i == NumOfProcessors - 1){
			sendNum[i] = cellsPerProcessor + (numOfInnerCells % NumOfProcessors);
		} else {
			sendNum[i] = cellsPerProcessor;
		}
		
		
		//Add on the edge numbers to each processor segment
		total += sendNum[i];
		
		addon = floor(((total/(arraySize - 2)) * 2));
		if (addon > 0){
			total = total % (arraySize-2);
		}	
		
		sendNum[i] = sendNum[i] + (arraySize*2) + addon;

		if (i == NumOfProcessors-1){
			sendNum[i] = sendNum[i] - 1;
		}
		
		
		//calculates the displacement for each array that is communicated
		if (i == 0){
			displacement[0] = 1;
			gatherDisplacement[0] = arraySize + 1;
		} else {
			displacement[i] = displacement[i-1] + sendNum[i-1] - arraySize*2;
			gatherDisplacement[i] = displacement[i] + arraySize;
		}
		
		gatherNum[i] = sendNum[i] - (arraySize*2);
		
		if (i == rank){
			recvSize = sendNum[i];
			sendSize = sendNum[i] - (arraySize*2);
		}
	}
	//to store each segment
	double *recvbuf = malloc(recvSize * sizeof(double));
	double *sendbuf = malloc(sendSize * sizeof(double));
	
	//to store the original segment that is sent to compare
	double *original = malloc(sendSize * sizeof(double));
	
	//to be broadcast to all process to break the main loop
	int totalWithinPrecision = 0;
	int iterations = 0;
	
	//To hold the precisions of each processor
	int *totalPrecision = malloc((NumOfProcessors)*sizeof(int));
	int size = 1;
	
	while (1){
		iterations++;
		MPI_Scatterv(arrayA, sendNum, displacement, MPI_DOUBLE, recvbuf, recvSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		
		//performs the relaxation on the segment
		relaxCells(recvbuf, sendbuf, original, displacement[rank], rank, recvSize);
		
		//checks the precision of the processor's segment
		withinPrecision[0] = precisionCheck(original, sendbuf, gatherNum[rank], rank);
		
		MPI_Gatherv(sendbuf, sendSize, MPI_DOUBLE, arrayB, gatherNum, gatherDisplacement, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Gatherv(withinPrecision, size, MPI_INT, totalPrecision, gatherNum2, gatherDisplacement2, MPI_INT, 0, MPI_COMM_WORLD);

		//only the root processor checks all the segment's precisions
		if (rank == 0){
				totalWithinPrecision = checkTotalPrecision(totalPrecision, NumOfProcessors);
		}
		
		//broadcast to break the loop or not
		MPI_Bcast(&totalWithinPrecision, 1, MPI_INT, 0, MPI_COMM_WORLD);

		//stop relaxing if precision has been reached
		if (!totalWithinPrecision){
			break;
		}
		
		//swaps the arrays around to continue with the relaxation
		SwapArray(&arrayA, &arrayB);
	}

	/*if (rank == 0){
		PrintArray(arrayA);
		PrintArray(arrayB);
	}*/

	rc = MPI_Finalize();
	free(arrayA);
	free(arrayB);
	
	//for checking and timings
	if (rank == 0){
		printf("Number of processors: %d\n", NumOfProcessors);
		printf("Number of iterations: %d\n\n", iterations);
		double end = MPI_Wtime();
		double timeTotal = end - start;

		printf("\nTotal time: %f\n", timeTotal);
	}
	return 0;
}