#ifndef PACKING_H
#define PACKING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct _Tree{
	char cut;
	double value;
	double width;
	double height;
	double x;
	double y;
	struct _Tree *right;
	struct _Tree *left;
	struct _Tree *previous;
}Tree;

Tree* Load_Binary_Tree_From_File(char* input_file_name);
void Load_From_File(FILE* fp, double* nums, char* array);
Tree* Tree_Construct(char* array, double* num);
int Print_Binary_Tree_Postorder(char* output_file1, Tree* tree);
void output1(FILE* fp, Tree* tree);
int Save_Dimensions_To_File(char* output_file2, Tree* tree);
void output2(FILE* fp, Tree* tree);
void Perform_Packing(Tree* tree);
int Save_Coordinates_To_File(char* output_file3, Tree* tree);
void output3(FILE* fp, Tree* tree);
void Tree_Destroy(Tree* tree);

#endif
