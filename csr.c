/*
 *  csr.c
 *  21103CW2
 *
 *  Created by Matthew Livingston on 23/01/2012.
 *  Copyright 2012 University of Bristol. All rights reserved.
 *
 */

#include "csr.h"

// Add two CSR matrices together and return a COO matrix
List * addCSR(CSR * A, CSR * B, List * M) {
	M->rows = A->rows;
	M->cols = A->cols;
	int total, i, j, k = 0; 
	for (i = 0; i < M->rows; i++) {
		for (j = 0; j < M->cols; j++) {
				total = findVal(A, i, j) + findVal(B, i, j);
				if (total != 0) { M = add(M, i, j, total); k++; }
		}	
	}
	M->size = k;
	return M;
}

// Find a column value in a particular row of a CSR matrix
int findVal(CSR * csr, int row, int col) {
	int i;
	for (i = csr->row_ptr[row]; i < csr->row_ptr[row+1]; i++) {
		if (csr->col_ind[i] == col) return csr->val[i]; 
	}
	return 0;
}

// Multiply two CSR matrices together and return a COO matrix
List * multiplyCSR(CSR * A, CSR * B, List * M) {
	M->rows = A->rows;
	M->cols = B->cols;
	int size = 0, total, MRow, MCol, ARow, BCol; 
	for (MRow = 0; MRow < M->rows; MRow++) {
		for (MCol = 0; MCol < M->cols; MCol++) {
			total = 0;
			for (ARow = A->row_ptr[MRow]; ARow < A->row_ptr[MRow+1]; ARow++) {
				for (BCol = B->row_ptr[A->col_ind[ARow]]; BCol < B->row_ptr[A->col_ind[ARow]+1]; BCol++) {
					if (B->col_ind[BCol] == MCol) total += A->val[ARow] * B->val[BCol];
				}
			}
			if (total != 0) { M = add(M, MRow, MCol, total); size++; }
		}	
	}
	M->size = size;
	return M;
}

// Allocate space for and initialise a CSR matrix 
CSR * initCSR(CSR * csr, int size, int rows) {
	csr->val     = calloc(size, sizeof(int)); 
	csr->col_ind = calloc(size, sizeof(int)); 
	csr->row_ptr = calloc(rows+1, sizeof(int));
	csr->val_length = size;
	csr->row_ptr_length = rows+1;
	int i;
	for (i = 0; i < csr->row_ptr_length; i++) csr->row_ptr[i] = -1;

	return csr;
}

// Return pointer to empty CSR matrix
CSR * freeCSR(CSR * csr) { 
	free(csr->val);
	free(csr->col_ind);
	free(csr->row_ptr); 
	return csr;
}

// Print a particular column of a CSR matrix
void printCol(CSR * csr, int col) {
	int * A = calloc(csr->cols, sizeof(int));
	int i, j; 
	for (i = 0; i < csr->row_ptr_length-1; i++) {
		for (j = csr->row_ptr[i]; j < csr->row_ptr[i+1]; j++) if (csr->col_ind[j] == col) A[i] = csr->val[j];
	}
	for (i = 0; i < csr->cols; i++) {
		if (i < csr->cols-1) printf("%d,", A[i]);
		else printf("%d", A[i]);
	}
	printf("\n");
	free(A);
}

// Print a particular row of a CSR matrix
void printRow(CSR * csr, int row) {
	int * A = calloc(csr->rows, sizeof(int));
	int i;
	for (i = csr->row_ptr[row]; i < csr->row_ptr[row+1]; i++) A[csr->col_ind[i]] = csr->val[i];
	for (i = 0; i < csr->rows; i++) {
		if (i < csr->rows-1) printf("%d,", A[i]);
		else printf("%d", A[i]);
	}
	printf("\n");
	free(A);
}

// Print out the three arrays that make up the CSR matrix
void printCSR(CSR * csr) {
	int i;
	for (i = 0; i < csr->val_length; i++) {
		printf("%d ", csr->val[i]);
	}
	printf("\n\n");
	for (i = 0; i < csr->val_length; i++) {
		printf("%d ", csr->col_ind[i]);
	}
	printf("\n\n");
	for (i = 0; i < csr->row_ptr_length; i++) {
		printf("%d ", csr->row_ptr[i]);
	}
	printf("\n");
}