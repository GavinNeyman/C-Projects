#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "packing.h"

Tree* Load_Binary_Tree_From_File(char* input_file_name)
{
	FILE* fp = fopen(input_file_name, "r");
	if(fp == NULL)
	{
		return NULL;
	}
	
	int len = 0;
	int cuts = 0;
	char mark = 0;
	while((mark = fgetc(fp)) != EOF)
	{
		if(mark == '\n')
		{
			len+=1;
		}
		else if(mark == 'H' || mark == 'V')
		{
			cuts+=1;
		}
	}
	fseek(fp,0L,SEEK_SET);
	
	double* nums = malloc(sizeof(*nums) * 3 * (len - cuts));
	char* array = malloc(sizeof(*array) * len);
	Load_From_File(fp, nums, array);
	Tree* tree = Tree_Construct(array,nums);
	free(nums);
	free(array);
	fclose(fp);
	return tree;
}

void Load_From_File(FILE* fp, double* nums, char* array)
{
	int i = 0;
	int size = 0;
	double dub = 0;
	char mark = '0';
	char hold[13] = {0};
	while((mark = fgetc(fp)) != EOF)
	{
		if(mark == 'H' || mark == 'V')
		{
			array[size] = mark;
			size+=1;
		}
		else if(mark != '\n' && mark != '\0')
		{
			dub = (double)atoi(&mark);
			while((mark = fgetc(fp)) != '(')
			{
				dub = dub * 10 + (double)atoi(&mark);
			}
			nums[i] = dub;
			i+=1;
			fread(hold, sizeof(mark), 12, fp);
			dub = atof(hold);
			nums[i] = dub;
			i+=1;	
			mark = fgetc(fp);
			fread(hold, sizeof(mark), 12, fp);
			dub = atof(hold);
			nums[i] = dub;
			i+=1;
			mark = fgetc(fp);
			array[size] = -1;
			size+=1;
		}
	}
	return;
}

Tree* Tree_Construct(char* array, double* nums)
{
	int count = 0;
	int i = 0;
	Tree* tree = malloc(sizeof(*tree));
	if (array[0] == 'H' || array[0] == 'V')
	{
		tree->cut = array[0];
		tree->value = -1;
		tree->x = 0;
		tree->y = 0;
		count+=1;
	}
	else
	{
		tree->cut = array[count];
		tree->value = nums[0];
		tree->width = nums[1];
		tree->height = nums[2];
		tree->x = 0;
		tree->y = 0;
		count+=1;
		i+=3;
	}
	tree->left = tree->right = tree->previous = NULL;
	Tree* curr = tree;
	while(1)
	{
		if(curr->left == NULL && (array[count] == 'H' || array[count] == 'V'))
		{
			curr->left = malloc(sizeof(*curr));
			curr->left->cut = array[count];
			curr->left->value = -1;
			curr->left->x = 0;
			curr->left->y = 0;
			curr->left->previous = curr;
			curr->left->left = NULL;
			curr->left->right = NULL;
			curr = curr->left;
			count += 1;
		}
		else if(curr->left == NULL)
		{
			curr->left = malloc(sizeof(*curr));
			curr->left->cut = array[count];
			curr->left->value = (int)(nums[i]);
			curr->left->width = nums[i+1];
			curr->left->height = nums[i+2];
			curr->left->x = 0;
			curr->left->y = 0;
			curr->left->left = NULL;
			curr->left->right = NULL;
			count+=1;
			i+=3;
		}
		else if(curr->right == NULL && (array[count] == 'H' || array[count] == 'V'))
		{
			curr->right = malloc(sizeof(*curr));
			curr->right->cut = array[count];
			curr->right->value = -1;
			curr->right->x = 0;
			curr->right->y = 0;
			curr->right->previous = curr;
			curr->right->left = NULL;
			curr->right->right = NULL;
			curr = curr->right;
			count += 1;
		}
		else if(curr->right == NULL)
		{
			curr->right = malloc(sizeof(*curr));
			curr->right->cut = array[count];
			curr->right->value = (int)(nums[i]);
			curr->right->width = nums[i+1];
			curr->right->height = nums[i+2];
			curr->right->x = 0;
			curr->right->y = 0;
			curr->right->left = NULL;
			curr->right->right = NULL;
			count+=1;
			i+=3;
		}
		else
		{
			if(curr->cut == 'V')
			{
				curr->width = curr->left->width + curr->right->width;
				if(curr->right->height > curr->left->height)
				{
					curr->height = curr->right->height;
				}
				else
				{
					curr->height = curr->left->height;
				}
			}
			else
			{
				curr->height = curr->left->height + curr->right->height;
				if(curr->right->width > curr->left->width)
				{
					curr->width = curr->right->width;
				}
				else
				{
					curr->width = curr->left->width;
				}
			}

			if(curr->previous != NULL)
			{
				curr = curr->previous;
			}
			else
			{
				return tree;
			}
		}
		
	}
	return tree;
}

int Print_Binary_Tree_Postorder(char* output_file1, Tree* tree)
{
	FILE* fp = fopen(output_file1, "w");
	if(fp == NULL)
	{
		return -1;
	}
	output1(fp,tree);
	fclose(fp);
	return 1;
}

void output1(FILE* fp, Tree* tree)
{
	if(tree->cut != 'H' && tree->cut != 'V')
	{
		fprintf(fp,"%d(%le,%le)\n",(int)(tree->value),tree->width,tree->height);
		return;
	}
	output1(fp, tree->left);
	output1(fp, tree->right);
	if(tree->cut == 'H')
	{
		fprintf(fp,"H\n");
	}
	if(tree->cut == 'V')
	{
		fprintf(fp,"V\n");
	}
	return;
}

int Save_Dimensions_To_File(char* output_file2, Tree* tree)
{
	FILE* fp = fopen(output_file2, "w");
	if(fp == NULL)
	{
		return -1;
	}
	output2(fp,tree);
	fclose(fp);
	return 1;
}

void output2(FILE* fp, Tree* tree)
{
	if(tree->cut != 'H' && tree->cut != 'V')
	{
		fprintf(fp,"%d(%le,%le)\n",(int)(tree->value),tree->width,tree->height);
		return;
	}
	if(tree->cut == 'H')
	{
		fprintf(fp,"H(%le,%le)\n",tree->width,tree->height);
	}
	if(tree->cut == 'V')
	{
		fprintf(fp,"V(%le,%le)\n",tree->width,tree->height);
	}
	output2(fp, tree->left);
	output2(fp, tree->right);
	return;
}

void Perform_Packing(Tree* tree)
{
	if(tree->left == NULL || tree->right == NULL)
	{
		return;
	}
	if(tree->cut == 'H')
	{
		tree->left->x = tree->x;
		tree->left->y = tree->y + tree->right->height;
		tree->right->x = tree->x;
		tree->right->y = tree->y;
	}
	else if(tree->cut == 'V')
	{
		tree->left->x = tree->x;
		tree->left->y = tree->y;
		tree->right->x = tree->x + tree->left->width;
		tree->right->y = tree->y;	
	}
	Perform_Packing(tree->left);
	Perform_Packing(tree->right);
	return;
}

int Save_Coordinates_To_File(char* output_file3, Tree* tree)
{
	FILE* fp = fopen(output_file3, "w");
	if(fp == NULL)
	{
		return -1;
	}
	output3(fp, tree);
	fclose(fp);
	return 1;
}

void output3(FILE* fp, Tree* tree)
{
	if(tree->cut != 'H' && tree->cut != 'V')
	{
		fprintf(fp, "%d(%le,%le)\n",(int)(tree->value),tree->x,tree->y);
		return;
	}
	output3(fp, tree->left);
	output3(fp, tree->right);
	return;
}

void Tree_Destroy(Tree* tree)
{
	if(tree->left != NULL)
	{
		Tree_Destroy(tree->left);
	}
	if(tree->right != NULL)
	{
		Tree_Destroy(tree->right);
	}
	free(tree);
	return;
}


