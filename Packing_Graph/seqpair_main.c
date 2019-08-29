#include <stdio.h>
#include <stdlib.h>
#include "seqpair.h"
#include <time.h>

int main(int argc, char** argv)
{
	if(argc != 3){
		return EXIT_FAILURE;
	}
	FILE* fp = fopen(argv[1], "r");
	if(fp == NULL){
		return EXIT_FAILURE;
	}
	char mark = '0';
	int rectangles = 0; // Number of Rectangles
	while((mark = fgetc(fp)) != '\n'){ // Account for double degit and high numbers
			rectangles = rectangles * 10 + atoi(&mark);
	}
	double* array = malloc(sizeof(*array) * rectangles * 3);
	int* seq1 = malloc(sizeof(*seq1) * rectangles);
	int* seq2 = malloc(sizeof(*seq1) * rectangles);
	Load_From_File(fp, array, seq1, seq2, rectangles);
	fclose(fp);
	
	// Vertical Constraint Graph
	//double start_t = clock();
	Graph* g = makeGraph(rectangles);
	VCG(g, seq1, seq2, rectangles);
	//double end_t = clock();
	//double total_t = (end_t - start_t) / CLOCKS_PER_SEC;
	//printf("Building Vertical Constraint Graph time: %le\n", total_t);
	
	int* visited = malloc(sizeof(*visited) * rectangles);
	int* list = malloc(sizeof(*list) * rectangles);
	int index = 0;
	for(int i = 0; i < rectangles; i++){
		visited[i] = 0;
		list[i] = 0;
	}
	// VCG DFS
	//start_t = clock();
	for(int i = 0; i < g->nv; i++){
		if(visited[i] == 0){
			DFS(g, i, visited, list, &index);
		}
	}
	//end_t = clock();
	//total_t = (end_t - start_t) / CLOCKS_PER_SEC;
	//printf("Vertical Constraint Graph depth first search time: %le\n", total_t);

	double* coords = malloc(sizeof(*coords) * rectangles * 3);
	for(int i = 0; i < rectangles; i++){
		coords[i*3] = i+1;
		coords[i*3 + 1] = 0;
		coords[i*3 + 2] = 0;
	}
	int j = 0;
	int* flip = malloc(sizeof(*flip) * rectangles);
	for(int i = rectangles-1; i >= 0; i--){
		flip[j] = list[i];
		j++;
	}
	// Get Y Coords
	//start_t = clock();
	y_coords(g, flip, array, coords);
	//end_t = clock();
	//total_t = (end_t - start_t) / CLOCKS_PER_SEC;
	//printf("Getting Y Coordinants time: %le\n", total_t);
	
	destroyGraph(g);
	
	// Horizontal Constraint Graph
	//start_t = clock();
	Graph* g2 = makeGraph(rectangles);
	HCG(g, seq1, seq2, rectangles);
	//end_t = clock();
	//total_t = (end_t - start_t) / CLOCKS_PER_SEC;
	//printf("Building Horizontal Constraint Graph time: %le\n", total_t);
	
	index = 0;
	for(int i = 0; i < rectangles; i++){
		visited[i] = 0;
		list[i] = 0;
	}
	// HCG DFS
	//start_t = clock();
	for(int i = 0; i < rectangles; i++){
		if(visited[i] == 0){
			DFS(g2, i, visited, list, &index);
		}
	}
	//end_t = clock();
	//total_t = (end_t - start_t) / CLOCKS_PER_SEC;
	//printf("Horizontal Constraint Graph depth first search time: %le\n", total_t);
	
	j = 0;
	for(int i = rectangles-1; i >= 0; i--){
		flip[j] = list[i];
		j++;
	}
	// Get X coords
	//start_t = clock();
	x_coords(g, flip, array, coords);
	//end_t = clock();
	//total_t = (end_t - start_t) / CLOCKS_PER_SEC;
	//printf("Getting X Coordinants time: %le\n", total_t);
	
	destroyGraph(g2);
	free(flip);
	free(list);
	free(visited);
	
	// Display
	FILE* output = fopen(argv[2], "w");
	if(output == NULL){
		return EXIT_FAILURE;
	}
	writeCoords(coords, output, rectangles);
	fclose(output);
	
	free(coords);
	free(array);
	free(seq1);
	free(seq2);
	return EXIT_SUCCESS;
}
