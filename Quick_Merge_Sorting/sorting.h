#ifndef SORTING_H
#define SORTING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int partition(long* Array, int lb, int ub);
void q_sort(long* Array, int lb, int ub);
void Quick_Sort(long* Array, int Size);
void merge(long* Array, int lb, int mid, int ub);
void msort(long* Array, int lb, int ub);
void Merge_Sort(long* Array, int Size);
long* Load_Into_Array(char* Filename, int* Size);
int Save_From_Array(char* Filename, long* Array, int Size);

#endif
