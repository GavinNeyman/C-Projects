#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"
#include <time.h>

int main(int argc, char** argv)
{
	char* q_file = argv[1];
	int q_size;
	long* q_Array = Load_Into_Array(q_file, &(q_size));
	double start_t = clock();
	Quick_Sort(q_Array, q_size);
	double end_t = clock();
	double total_t = (end_t - start_t) / CLOCKS_PER_SEC;
	printf("Quick Sort: %le\n", total_t);
	Save_From_Array(argv[2], q_Array, q_size);
	free(q_Array);

	char* m_file = argv[3];
	int m_size;
	long* m_Array = Load_Into_Array(m_file, &(m_size));
	start_t = clock();
	Merge_Sort(m_Array, m_size);
	end_t = clock();
	total_t = (end_t - start_t) / CLOCKS_PER_SEC;
	printf("Merge Sort: %le\n", total_t);
	Save_From_Array(argv[4], m_Array, m_size);
	free(m_Array);
	return EXIT_SUCCESS;
}
