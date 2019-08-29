#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorting.h"


// Function to split the array in halfs using the middle value
int partition(long* Array, int lb, int ub){
	// Find the middle element of the array and swap it with the first element in the array
	int pi = (lb+((ub-lb)/2));
	long pivot = Array[pi];
	long swap = Array[lb];
	Array[lb] = pivot;
	Array[pi] = swap;
	// Preform partitioning as usual
	int right = ub;
	int left = lb;
	while(left < right)
	{
		while(Array[left] <= pivot && left < ub)
		{
			left+=1;
		}
		while(Array[right] > pivot)
		{
			right-=1;
		}
		if(left < right)
		{
			swap = Array[left];
			Array[left] = Array[right];
			Array[right] = swap;
		}
	}
	Array[lb] = Array[right];
	Array[right] = pivot;
	return right;
}

// Normal quick sort function
void q_sort(long* Array, int lb, int ub){
	if(lb >= ub)
	{
		return;
	}
	int pivot = partition(Array, lb, ub);
	q_sort(Array, lb, pivot - 1);
	q_sort(Array, pivot + 1, ub);
}

/* tr_quicksort
// Quick sort with tail recursion removed that iterates on which ever is largest
void tr_qsort(long* Array, int lb, int ub){
	while(lb < ub)
	{
		int pivot = partition(Array, lb, ub);
		if((pivot) < (lb + (ub-lb)/2))
		{
			tr_qsort(Array, lb, pivot-1);
			lb = pivot+1;
		}
		else
		{
			tr_qsort(Array, pivot+1, ub);
			ub = pivot-1;
		}
	}
}
*/

// Calls the real quick sort function
void Quick_Sort(long* Array, int Size){
	q_sort(Array, 0, Size-1);
}


// merges two sorted parts of the array together
void merge(long* Array, int lb, int mid, int ub){
	int sub1 = mid - lb + 1;
	int sub2 = ub - mid;
	long* left = malloc(sizeof(*left) * sub1);
	long* right = malloc(sizeof(*right) * sub2);
	int i = 0;
	int j = 0;
	
	for(i = 0; i < sub1; i++){
		left[i] = Array[lb+i];
	}
	for(j = 0; j < sub2; j++){
		right[j] = Array[mid+1+j];
	}
	
	i = j = 0;
	int k = lb;
	while((j < sub2) && (i < sub1)){
		if(left[i] <= right[j]){
			Array[k] = left[i];
			i+=1;
		}
		else{
			Array[k] = right[j];
			j+=1;
		}
		k+=1;
	}
	for(int m = i; m < sub1; m++){ // Accounts for leftovers
		Array[k] = left[m];
		k+=1;
	}
	for(int m = j; m < sub2; m++){ // Accounts for leftovers
		Array[k] = right[m];
		k+=1;
	}
	free(left);
	free(right);
}


/*
// merges two sorted parts of the array together
void merge(long* Array, long* tmp, int lb, int mid, int ub){
	memcpy(&tmp[lb], &Array[lb], mid-lb+1);
	memcpy(&tmp[mid+1], &Array[mid+1], ub-mid);
	int i = lb;
	int j = mid+1;
	for(int k = lb; k < ub; k++){
		if(i > mid){
			Array[k] = tmp[j++];
		}
		else if(j > ub){
			Array[k] = tmp[i++];
		}
		else if(tmp[j] < tmp[i]){
			Array[k] = tmp[j++];
		}
		else{
			Array[k] = tmp[i++];
		}
	}
}
*/

void msort(long* Array, int lb, int ub){
	if(lb >= ub){
		return;
	}
	int mid = lb+((ub-lb)/2);
	msort(Array, lb, mid);
	msort(Array, mid+1, ub);
	merge(Array, lb, mid, ub);
}

/* iterative msort
// An iterative form of merge sort
void it_msort(long* Array, int n){
	int size = 1;
	int index = 0;
	int min = 0;
	long* tmp = malloc(sizeof(*tmp) * n);
	while(size < n){
		index = 0;
		while(index < n - size){
			if(index+2*size-1 < n-1){
				min = index+2*size-1;
			}
			else{
				min = n-1;
			}
			merge(Array, tmp, index, index+size-1, min);
			index = index+2*size;
		}	
		size = size*2;
	}
	free(tmp);
}
*/

/* iterative msort /w exchanging Array and tmp
// An iterative form of merge sort
void it_msort(long* Array, int n){
	int size = 1;
	int index = 0;
	int min = 0;
	long* tmp = malloc(sizeof(*tmp) * n);
	long** sorted = &(Array);
	long** merged = &(tmp);
	while(size < n){
		index = 0;
		while(index < n - size){
			if(index+2*size-1 < n-1){
				min = index+2*size-1;
			}
			else{
				min = n-1;
			}
			merge(*(merged), *(sorted), index, index+size-1, min);
			index = index+2*size;
		}	
		while(index < n){
			(*merged)[index] = (*sorted)[index];
			index+=1;
		}
		long** hold = merged;
		merged = sorted;
		sorted = hold;
		size = size*2;
	}
	if((*sorted) == tmp){
		memcpy(Array, tmp, n);
	}
	free(tmp);
}
*/

// Calls the real merge sort function
void Merge_Sort(long* Array, int Size){
	msort(Array, 0, Size-1);
}

long* Load_Into_Array(char* Filename, int* Size){
	char mark = '0';
	long num = 0;
	*Size = 1;
	FILE* fp = fopen(Filename, "r");
	if(fp == NULL)
	{
		return NULL;
	}
	while((mark = fgetc(fp)) != EOF){
		if(mark == ' '){
			*Size = *Size + 1;
		}
	}
	fseek(fp, 0L, SEEK_SET);
	long* Array = malloc(sizeof(*Array) * *Size);
	int i = 0;
	mark = fgetc(fp);
	while(mark != EOF){
		num = (long)(atoi(&mark)); // Turn a char into an int
		while((mark = fgetc(fp)) != ' '){ // Account for double degit and high numbers
			if(mark == EOF){
				break;
			}
			num = num * 10 + (long)atoi(&mark);
		}
		Array[i] = num;
		i++;
		mark = fgetc(fp);
	}
	fclose(fp);
	return Array;
}

int Save_From_Array(char* Filename, long* Array, int Size){
	FILE* fp = fopen(Filename, "w");
	if(fp == NULL)
	{
		return -1;
	}
	for(int i = 0; i < Size; i+=1)
	{
		fprintf(fp, "%ld ", Array[i]);
	}
	fclose(fp);
	FILE* fptr = fopen(Filename, "b");
	if(fptr == NULL)
	{
		return -1;
	}
	int check = ftell(fptr);
	fclose(fptr);
	return check;
}
