#include<stdio.h>
#include<stdlib.h>
#include"sorting.h"

long* Load_Into_Array(char* Filename, int* Size)
{
	FILE* fp = fopen(Filename, "rb");
	fseek(fp, 0L, SEEK_END);
	*Size = ftell(fp)/sizeof(long);
	fseek(fp, 0L, SEEK_SET);
	long* Array = malloc(sizeof(*Array) * *Size);
	for(int i = 0; i < *Size; i+=1)
	{
		fread(Array + i, sizeof(long), 1, fp);
	}
	fclose(fp);
	return Array;
}

int Save_From_Array(char* Filename, long* Array, int Size)
{
	FILE* fp = fopen(Filename, "wb");
	for(int i = 0; i < Size; i+=1)
	{
		fwrite(Array + i, sizeof(long), 1, fp);
	}
	fclose(fp);
	FILE* fptr = fopen(Filename, "rb");
	fseek(fptr, 0L, SEEK_END);
	int check = ftell(fptr);
	fclose(fptr);
	return check;
}

void Shell_Sort_Array(long* Array, int Size, double* N_Cmp)
{
	int n = Size;
	int h = 1;
	int k = 0;
	while(h < n/3)
	{
		h = 3*h+1;
	}
	while(h >= 1)
	{
		for(int i=h; i<n; i+=1)
		{
			for(int j=i; j>=h && Array[j]<Array[j-1]; j-=h)
			{
				*N_Cmp+=1;
				long swap = Array[j];
				Array[j] = Array[j-1];
				Array[j-1] = swap;
				k = j;
			}
		}
		if(k>=h)
		{
			*N_Cmp+=1;
		}
		h/=3;
	}
}

Node* Load_Into_List(char* Filename)
{
	FILE* fp = fopen(Filename, "rb");
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp)/sizeof(long);
	fseek(fp, 0L, SEEK_SET);
	Node* tmp = NULL;
	Node* node = NULL;

	for(int i = 0; i < size; i+=1)
	{
		if(i == 0)
		{
			node = malloc(sizeof(*node));
			int success = fread(&(node->value), sizeof(long), 1, fp);
			if(success == 0)
			{
				node = NULL;
			}
			tmp = node;
		}
		else
		{
			tmp->next = malloc(sizeof(*tmp));
			fread(&(tmp->next->value), sizeof(long), 1, fp);
			tmp = tmp->next;
		}
	}
	tmp->next = NULL; // valgrind fix
	fclose(fp);
	return node;
}

int Save_From_List(char* Filename, Node* list)
{
	FILE* fp = fopen(Filename, "wb");
	Node* tmp = list;
	while(tmp)
	{
		fwrite(&(tmp->value), sizeof(long), 1, fp);
		tmp = tmp->next;
	}
	fclose(fp);
	FILE* fptr = fopen(Filename, "rb");
	fseek(fptr, 0L, SEEK_END);
	int check = ftell(fptr);
	fclose(fptr);
	return check;	
}

Node* Shell_Sort_List(Node* list, double* N_Cmp)
{
	//LinkedList* linked = NULL;
	//linked->start = list;
	Node* node = list;
	Node* tmp = NULL;
	int n = 0;
	int h = 1;
	int x = 0;
	while(node != NULL)
	{
		node = node->next;
		n+=1;
	}
	while(2*(3*h+1) < n)
	{
		h = 3*h+1;
	}
	while(h >= 1)
	{
		for(int i=h; i>0; i-=1)
		{
			for(int j=h-i; j<n; j+=h)
			{
				node = list;
				for(x=0; x<j; x+=1)
				{
					node = node->next;
				}
				int count = x+h;
				tmp = node;
				while(tmp != NULL)
				{
					while(x<count)
					{
						if(tmp != NULL)
						{
							tmp=tmp->next;
							x+=1;
							if(tmp != NULL)
							{
								if(node->value>tmp->value)
								{
									/*
									if(linked->start == node)
									{
										linked->start = tmp;
									}
									else if(linked->start == tmp)
									{
										linked->start = node;
									}
									swap(list, node, tmp);
									*/
									int swap = node->value;
									node->value = tmp->value;
									tmp->value = swap;
								}
								*N_Cmp+=1;
							}
						}
						else
						{
							break;
						}
						count+=h;
					}
				}
			}
		}
		h/=3;
	}
	return list;
}
