/*
 *  matrix.c
 *  21103CW2
 *
 *  Created by Matthew Livingston on 23/01/2012.
 *  Copyright 2012 University of Bristol. All rights reserved.
 *
 */

#include "coo.h"
#include "csr.h"

// Read in elements from a file into a coordinate list
void matrix_read(List * list, FILE * f) {
	fscanf(f, "%d,", &list->rows);
	fscanf(f, "%d\n", &list->cols);
	int row, col, val;
	while (fscanf(f, "%d", &row) != EOF) {
		fscanf(f, ",%d", &col);
		fscanf(f, ",%d", &val);
		list = addRowOrdered(list, row, col, val);
	}
}

// Write elements from a coordinate list into a file
void matrix_write(List * list, FILE * f) {
	Node * node = list->head;
	fprintf(f, "%d,%d\n", list->rows, list->cols);
	while (node != NULL) {
		fprintf(f, "%d,", node->row);
		fprintf(f, "%d,", node->col);
		fprintf(f, "%d\n", node->val);
		node = node->next;
	}
}

// Copy elements in coordinate list data structure into compressed sparse row structure
void convertCOOToCSR(List * list, CSR * csr) {
	Node * node = list->head;
	int i = 0, k = 0;
	while (node != NULL) {
		csr->val[i] = node->val;
 		csr->col_ind[i] = node->col;
 		if (csr->row_ptr[node->row] == -1) {
	 		csr->row_ptr[node->row] = i;
	 	}
		node = node->next;
		i++;
	}
	csr->row_ptr[0] = 0;
	csr->row_ptr[csr->row_ptr_length-1] = csr->val_length;
	for (k = csr->row_ptr_length-1; k > 0; k--) {
		if (csr->row_ptr[k] == -1) csr->row_ptr[k] = csr->row_ptr[k+1];
	}
	csr->rows = list->rows;
	csr->cols = list->cols;
}

void stage1(FILE * f, int row, int col) {
	List * list = calloc(1, sizeof(List));
	CSR * csr   = calloc(1, sizeof(CSR));
	matrix_read(list, f);
	if (row > list->rows-1) { 
		fprintf(stdout, "Error: Row input is larger than matrix row size\n"); 
		exit(1);
	}
	if (col > list->cols-1) {
		fprintf(stdout, "Error: Column input is larger than matrix column size\n");
		exit(1); 
	}
	csr = initCSR(csr, list->size, list->rows);
	convertCOOToCSR(list, csr);
	printf("%d\n", searchElement(list->head, row, col));
	printRow(csr, row);
	printCol(csr, col);
	free(freeList(list));
	free(freeCSR(csr));
}

void stage2(FILE * f, FILE * fout) {
	List * list = calloc(1, sizeof(List));
	matrix_read(list, f);
	list = transpose(list);
	matrix_write(list, fout);
	free(freeList(list));
	fclose(fout);
}

void stage3(FILE * fout, FILE * input0, FILE * input1) {
	List * list      = calloc(1, sizeof(List));
	CSR * matrix0    = calloc(1, sizeof(CSR));
	CSR * matrix1    = calloc(1, sizeof(CSR));
	List * outmatrix = calloc(1, sizeof(List));

	matrix_read(list, input0);
	matrix0 = initCSR(matrix0, list->size, list->rows);
	convertCOOToCSR(list, matrix0);
	list = freeList(list);
	matrix_read(list, input1);
	matrix1 = initCSR(matrix1, list->size, list->rows);
	convertCOOToCSR(list, matrix1);
	free(freeList(list));
	if ((matrix0->rows != matrix1->rows) || (matrix0->cols != matrix1->cols)) {
		printf("Abort: Input matrices are incompatible\n");
		exit(1);
	}
	outmatrix = addCSR(matrix0, matrix1, outmatrix);
	matrix_write(outmatrix, fout);
	free(freeCSR(matrix0));
	free(freeCSR(matrix1));
	free(freeList(outmatrix));
}

void stage4(FILE * fout, FILE * input0, FILE * input1) {
	List * list      = calloc(1, sizeof(List));
	CSR * matrix0    = calloc(1, sizeof(CSR));
	CSR * matrix1    = calloc(1, sizeof(CSR));
	List * outmatrix = calloc(1, sizeof(List));

	matrix_read(list, input0);
	matrix0 = initCSR(matrix0, list->size, list->rows);
	convertCOOToCSR(list, matrix0);
	list = freeList(list);
	matrix_read(list, input1);
	matrix1 = initCSR(matrix1, list->size, list->rows);
	convertCOOToCSR(list, matrix1);
	free(freeList(list));
	if (matrix0->cols != matrix1->rows) {
		printf("Abort: Input matrices are incompatible\n");
		exit(1);
	}
	outmatrix = multiplyCSR(matrix0, matrix1, outmatrix);
	matrix_write(outmatrix, fout);
	free(freeCSR(matrix0));
	free(freeCSR(matrix1));
	free(freeList(outmatrix));
}

void stage5(int argc, char ** argv) {
	List * list      = calloc(1, sizeof(List));
	CSR * matrix0    = calloc(1, sizeof(CSR));
	CSR * matrix1    = calloc(1, sizeof(CSR));
	List * outmatrix = calloc(1, sizeof(List));

	FILE * input0 = fopen(argv[3], "r");
	if (input0 == 0) { fprintf(stdout, "Could not open file %s\n", argv[3]); exit(1); }
	FILE * input1;
	matrix_read(list, input0);
	matrix0 = initCSR(matrix0, list->size, list->rows);
	convertCOOToCSR(list, matrix0);
	list = freeList(list);

	int i = 4;
	do {
		input1 = fopen(argv[i], "r");
		if (input1 == 0) { fprintf(stdout, "Could not open file %s\n", argv[i]); exit(1); }
		matrix_read(list, input1);
		matrix1 = initCSR(matrix1, list->size, list->rows);
		convertCOOToCSR(list, matrix1);
		list = freeList(list);
		if (matrix0->cols != matrix1->rows) {
			printf("Abort: Input matrices are incompatible\n");
			exit(1);
		}
		outmatrix = multiplyCSR(matrix0, matrix1, outmatrix);
		if (i == argc-1) {
			FILE * fout = fopen(argv[2], "w");
			if (fout == 0) { fprintf(stdout, "Could not open file %s\n", argv[2]); exit(1); }
			matrix_write(outmatrix, fout);
			free(list);
			free(freeCSR(matrix0));
			free(freeCSR(matrix1));
			free(freeList(outmatrix));
			fclose(fout);
			break;
		}
		matrix0 = freeCSR(matrix0);
		matrix1 = freeCSR(matrix1);
		matrix0 = initCSR(matrix0, outmatrix->size, outmatrix->rows);
		convertCOOToCSR(outmatrix, matrix0);
		outmatrix = freeList(outmatrix);
		i++;
	} while (i < argc);
}

int main(int argc, char * argv[]) {		
	if (strcmp(argv[1], "stage1") == 0) {
		if (argc != 5) {
			fprintf(stdout, "Please use the format: ./matrix stage1 ${INPUT} ${ROW} ${COL}\n");
			exit(1);
		}
		FILE * f = fopen(argv[2], "r");
		if (f == 0) { fprintf(stdout, "Could not open file %s\n", argv[2]); exit(1); }
		stage1(f, atoi(argv[3]), atoi(argv[4]));
	}
	else if (strcmp(argv[1], "stage2") == 0) {
		if (argc != 4) {
			fprintf(stdout, "Please use the format: ./matrix stage2 ${OUTPUT} ${INPUT}\n");
			exit(1);
		}
		FILE * f = fopen(argv[3], "r");
		if (f == 0) { fprintf(stdout, "Could not open file %s\n", argv[3]); exit(1); }
		FILE * fout = fopen(argv[2], "w");
		if (fout == 0) { fprintf(stdout, "Could not open file %s\n", argv[2]); exit(1); }
		stage2(f, fout);
	}
	else if (strcmp(argv[1], "stage3") == 0) {
		if (argc != 5) {
			fprintf(stdout, "Please use the format: ./matrix stage3 ${OUTPUT} ${INPUT_0} ${INPUT_1}\n");
			exit(1);
		}
		FILE * fout = fopen(argv[2], "w");
		if (fout == 0) { fprintf(stdout, "Could not open file %s\n", argv[2]); exit(1); }
		FILE * input0 = fopen(argv[3], "r");
		if (input0 == 0) { fprintf(stdout, "Could not open file %s\n", argv[3]); exit(1); }
		FILE * input1 = fopen(argv[4], "r");
		if (input1 == 0) { fprintf(stdout, "Could not open file %s\n", argv[4]); exit(1); }
		stage3(fout, input0, input1);
	}
	else if (strcmp(argv[1], "stage4") == 0) {
		if (argc != 5) {
			fprintf(stdout, "Please use the format: ./matrix stage4 ${OUTPUT} ${INPUT_0} ${INPUT_1}\n");
			exit(1);
		}
		FILE * fout = fopen(argv[2], "w");
		if (fout == 0) { fprintf(stdout, "Could not open file %s\n", argv[2]); exit(1); }
		FILE * input0 = fopen(argv[3], "r");
		if (input0 == 0) { fprintf(stdout, "Could not open file %s\n", argv[3]); exit(1); }
		FILE * input1 = fopen(argv[4], "r");
		if (input1 == 0) { fprintf(stdout, "Could not open file %s\n", argv[4]); exit(1); }
		stage4(fout, input0, input1);
	}
	else if (strcmp(argv[1], "stage5") == 0) {
		if (argc < 5) {
			fprintf(stdout, "Please use the format: ./matrix stage5 ${OUTPUT} ${INPUT_0} ${INPUT_1} ... ${INPUT_X}\n");
			exit(1);
		}
		stage5(argc, argv);
	}

	return 0;
}