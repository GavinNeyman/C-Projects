#ifndef SORTING_H
#define SORTING_H
#include<stdio.h>
#include<stdlib.h>

typedef struct _Node{
	long value;
	struct _Node *next;
}Node;

typedef struct _LinkedList{
	struct _Node *start;
}LinkedList;

long* Load_Into_Array(char* Filename, int* Size);
int Save_From_Array(char* Filename, long* Array, int Size);
void Shell_Sort_Array(long* array, int Size, double* N_cmp);

Node* Load_Into_List(char* Filename);
int Save_From_List(char* Filename, Node* list);
Node* Shell_Sort_List(Node* list, double* N_cmp);

#endif
