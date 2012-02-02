/*
 *  coo.h
 *  21103CW2
 *
 *  Created by Matthew Livingston on 23/01/2012.
 *  Copyright 2012 University of Bristol. All rights reserved.
 *
 */

#ifndef coo_h
#define coo_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
	int row, col, val;
	struct node * next;
} Node;

typedef struct {
	Node * head;
	Node * tail; 
	int rows;
	int cols;
	int size;
} List;

Node * insertNode(int row, int col, int val, Node * nextNode);
List * add(List * list, int row, int col, int val);
List * addRowOrdered(List * list, int row, int col, int val);
Node * searchList(Node * node, int row);
int searchElement(Node * node, int row, int col);
List * transpose(List * list);
List * freeList(List * list);
void printList(Node * node);

#endif