#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct _Tree
{
	int leaf;
    int freq;
	unsigned char ASCII;
	struct _Tree * left;
	struct _Tree * right;
}Tree;

typedef struct _Node
{
	struct _Tree * info;
	struct _Node * next;
}Node;

typedef struct _List
{
	struct _Node * start;
}List;

List* construct_huffman(char* array, int size);
void transverse_huffman(FILE* output, List* list, int* code, int size);
int* bitify(char* code);
void huffman_values(Tree* tree, int* path, int index, FILE* output);
void header_bits(char* header, FILE* output, int len);
void free_node(List* list);
void convert(int* bin, int* code);
char* header_char(char* header_bytes, int size);

#endif
