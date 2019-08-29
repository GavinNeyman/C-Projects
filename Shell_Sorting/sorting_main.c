#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"sorting.h"

int main(int argc, char** argv)
{
	char* input = argv[2];
	char* output = argv[3];
	if(argv[1][1] == 'a')
	{
		int Size = 0;
		double N_Cmp = 0;
		double start_t = (double)clock();
		long* Array = Load_Into_Array(input, &Size);
		double end_t = (double)clock();
		double input_t = end_t - start_t;
		start_t = (double)clock();
		Shell_Sort_Array(Array, Size, &N_Cmp);
		end_t = (double)clock();
		double sort_t = end_t - start_t;
		start_t = (double)clock();
		int num = Save_From_Array(output, Array, Size);
		if(num < 0)
		{
			return EXIT_FAILURE;
		}
		end_t = (double)clock();
		clock_t output_t = end_t - start_t;
		free(Array);
		//Assuming I/O time refers to the time required to load the unsorted list and save the sorted list
		//since the assignment PDF does not explain this. (input/output time)
		fprintf(stdout, "I/O time: %le\n", (input_t + output_t)/CLOCKS_PER_SEC);
		fprintf(stdout, "Sorting time: %le\n", (sort_t)/CLOCKS_PER_SEC);
		fprintf(stdout, "Number of comparisons: %le\n", N_Cmp);
	}
	else if(argv[1][1] == 'l')
	{
		double N_Cmp = 0;
		double start_t = clock();
		Node* node = Load_Into_List(input);
		double end_t = clock();
		double input_t = end_t - start_t;
		start_t = clock();
		node = Shell_Sort_List(node, &N_Cmp);
		end_t = clock();
		double sort_t = end_t - start_t;
		start_t = clock();
		int num = Save_From_List(output, node);
		if(num < 0)
		{
			return EXIT_FAILURE;
		}
		end_t = clock();
		double output_t = end_t - start_t;
		while(node != NULL)
		{
			Node* tmp = node;
			node = node->next;
			free(tmp);
		}
		fprintf(stdout, "I/O time: %le\n", (input_t + output_t)/CLOCKS_PER_SEC);
		fprintf(stdout, "Sorting time: %le\n", (sort_t)/CLOCKS_PER_SEC);
		fprintf(stdout, "Number of comparisons: %le\n", N_Cmp);
	}
	else
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
