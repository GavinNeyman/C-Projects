#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "packing.h"

int main(int argc, char** argv)
{
	if(argc != 5)
	{
		return EXIT_FAILURE;
	}
	//double start_t = clock();
	Tree* tree = Load_Binary_Tree_From_File(argv[1]);
	//double end_t = clock();
	//double total_t = (end_t - start_t)/CLOCKS_PER_SEC;
	//printf("Time taken to build the tree: %le\n", total_t);
	if(tree == NULL)
	{
		return EXIT_FAILURE;
	}
	int success = Print_Binary_Tree_Postorder(argv[2],tree);
	if(success == -1)
	{
		return EXIT_FAILURE;
	}
	//start_t = clock();
	Perform_Packing(tree);
	//end_t = clock();
	//total_t = (end_t - start_t)/CLOCKS_PER_SEC;
	//printf("Time taken to compute coordinates: %le\n", total_t);
	success = Save_Dimensions_To_File(argv[3],tree);
	if(success == -1)
	{
		return EXIT_FAILURE;
	}
	success = Save_Coordinates_To_File(argv[4],tree);
	if(success == -1)
	{
		return EXIT_FAILURE;
	}
	Tree_Destroy(tree);
	return EXIT_SUCCESS;
}
