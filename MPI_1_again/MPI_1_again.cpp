// MPI_1_again.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <mpi.h>

void printMatrix(int* matrix, int dim){
	int temp = 0;
	for (int i = 0; i < dim*dim; i++) {
		std::cout << matrix[i] << "\t";
		temp++;
		if (temp == dim) {
			std::cout << "\n";
			temp = 0;
		}
	}
}
/*
Erstellt eine Quadratische Matrix mit
elemente ... Anzahl der Elemnte in der Matrix
startValue ... offset des Startwertes
*/
int* createMatrix(int elemente, int startValue) {
	int* matA = (int*)malloc(sizeof(int) * elemente);
	for (int i = 0; i < elemente; i++)
	{
		matA[i] = startValue+i;
	}
	return matA;
}

int main()
{

	int rank,size;
	int *matA, *matB, *matC, *puff1, *puff2;

	puff1 = (int*)malloc(sizeof(int) * 4);
	puff2 = (int*)malloc(sizeof(int) * 4);
	matA =  (int*)malloc(sizeof(int) * 16);
	matB =  (int*)malloc(sizeof(int) * 16);
	matC =  (int*)malloc(sizeof(int) * 16);

	for (int i = 0; i < 4; i++) {
		puff2[i] = 0;
	}

	MPI_Init(NULL, NULL);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	if (rank == 0) {
		matA = createMatrix(16, 0);
		matB = createMatrix(16, 10);	
	}

	MPI_Scatter(matA, 4, MPI_INT, puff1, 4, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(matB, 16, MPI_INT, 0, MPI_COMM_WORLD);

	int temp = 0;


	for (int i = 0; i <4; i++) {
		for (int j = 0; j < 4; j++) {
			puff2[j] += puff1[i] * matB[4*temp+j];
		}
		temp++;
	}
	

	MPI_Gather(puff2, 4, MPI_INT, matC, 4, MPI_INT, 0, MPI_COMM_WORLD);
	
	MPI_Finalize();
	if (rank == 0) {
		printMatrix(matC, 4);
	}
	
}

