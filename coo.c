/*
 *  coo.c
 *  21103CW2
 *
 *  Created by Matthew Livingston on 23/01/2012.
 *  Copyright 2012 University of Bristol. All rights reserved.
 *
 */

#include "coo.h"

// Allocate memory for a new Node struct
Node * insertNode(int row, int col, int val, Node * nextNode) {
	Node * newNode = calloc(1, sizeof(Node)); 
	newNode->row = row;
	newNode->col = col;
	newNode->val = val;
	newNode->next = nextNode;
	
	return newNode;	
}

// Add a new node to the List 
List * add(List * list, int row, int col, int val) {
	if (list->head == NULL) list->head = list->tail = insertNode(row, col, val, NULL);  
	else list->tail = list->tail->next = insertNode(row, col, val, NULL); 
	list->size++;
	return list;
}

// Add a new node to the List in ascending row order
List * addRowOrdered(List * list, int row, int col, int val) {
	if (list->head == NULL) list->head = list->tail = insertNode(row, col, val, NULL); 
	else if (row <= list->head->row) list->head = insertNode(row, col, val, list->head); 
	else if (row >= list->tail->row) list->tail = list->tail->next = insertNode(row, col, val, NULL); 
	else {
		Node * prev = searchList(list->head, row);
		if (prev != NULL) prev->next = insertNode(row, col, val, prev->next);
		else list->tail = insertNode(row, col, val, NULL);
	}
	list->size++;
	return list;
}

// Search a List sequentially
Node * searchList(Node * node, int row) {
	while (node->next != NULL) {
		if (node->next->row > row) return node;
		else node = node->next;
	}
	return NULL;
}

// Search a List for a specific element
int searchElement(Node * node, int row, int col) {
	while (node != NULL) {
		if ((node->row == row) && (node->col == col)) return node->val;
		else node = node->next;
	}
	return -1;
}

// Transpose a COO matrix
List * transpose(List * list) {
	Node * node = list->head;
	int temp;
	while (node != NULL) {
		temp = node->row;
		node->row = node->col;
		node->col = temp;
		node = node->next;
	}
	temp       = list->rows;
	list->rows = list->cols;
	list->cols = temp; 
	return list;
}

// Return pointer to an empty list
List * freeList(List * list) {
	Node * temp;
	while ((temp = list->head) != NULL) {
		list->head = list->head->next;
		free(temp);
	}
	list->rows = 0;
	list->cols = 0;
	list->size = 0;
	return list;
}

// Print a List
void printList(Node * node) {
	while (node != NULL) 
	{
		printf("%d %d %d", node->row, node->col, node->val);
		printf("\n");
		node = node->next;
	}
}
