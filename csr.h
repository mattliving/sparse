/*
 *  csr.h
 *  21103CW2
 *
 *  Created by Matthew Livingston on 23/01/2012.
 *  Copyright 2012 University of Bristol. All rights reserved.
 *
 */

#ifndef csr_h
#define csr_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "coo.h"

typedef struct {
	int * val;
	int * col_ind;
	int * row_ptr;
	int val_length;
	int row_ptr_length;
	int rows;
	int cols;
} CSR;

List * addCSR(CSR * A, CSR * B, List * M);
List * multiplyCSR(CSR * A, CSR * B, List * M);
int findVal(CSR * csr, int row, int col);
CSR * initCSR(CSR * csr, int size, int rows);
CSR * freeCSR(CSR * csr);
void printCSR(CSR * csr);
void printCol(CSR * csr, int col);
void printRow(CSR * csr, int row);

#endif