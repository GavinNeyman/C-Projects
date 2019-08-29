#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "seqpair.h"

void Load_From_File(FILE* fp, double* array, int* seq1, int* seq2, int rectangles){
	int size = 0; // Size of current array. This is used for indexing
	double dub = 0; // Used as a place holder before a number is ready to be put into the array.
	char mark = '0'; // Variable to put read characters from the file
	char hold[13] = {0}; // 13 character array that holds the doubles so they can be transfered into doubles
	
	for(int i = 0; i < rectangles; i++){ // Read 'rectangles' lines from the file
		mark = fgetc(fp); // Read first char of the line
		dub = (double)atoi(&mark); // Turn the char read into an double
		while((mark = fgetc(fp)) != '('){ // Account for double degit and high numbers
			dub = dub * 10 + (double)atoi(&mark);
		}
		array[size] = dub; // Put rectangle number in array
		size++; // incement counter
		fread(hold, sizeof(mark), 12, fp); // read in all chars of the format ????????e+??
		dub = atof(hold); // convert read chars into a floating point
		array[size] = dub; // insert coordinate x into array
		size++; // incement counter
		mark = fgetc(fp); // get rid of comma between dimensions
		fread(hold, sizeof(mark), 12, fp); // read in all chars of the format ????????e+??
		dub = atof(hold); // convert read chars into a floating point
		array[size] = dub; // insert coordinate y into array
		size++; // incement counter
		mark = fgetc(fp); // gets rid of closing paranthases
		mark = fgetc(fp); // gets rid of new line character
	}
	for(int j = 0; j < rectangles; j++){ // Read in the first sequence
		mark = fgetc(fp);
		seq1[j] = atoi(&mark);
		//seq1[j] = mark - '0';
		while((mark = fgetc(fp)) != ' ' && mark != '\n'){ // Account for double degit and high numbers
			seq1[j] = seq1[j] * 10 + atoi(&mark);
		}
		//fgetc(fp); // Gets rid of the space/new line character
	}
	for(int j = 0; j < rectangles; j++){ // Read in the second sequence
		mark = fgetc(fp);
		seq2[j] = atoi(&mark);
		//seq2[j] = mark - '0';
		while((mark = fgetc(fp)) != ' ' && mark != '\n' && mark != EOF){ // Account for double degit and high numbers
			seq2[j] = seq2[j] * 10 + atoi(&mark);
		}
		//fgetc(fp); // Gets rid of the space
	}
	return;
}

Graph* makeGraph(int nv){ // Malloc for graph using adj matrix
	Graph* g = malloc(sizeof(*g));
	g->nv = nv;
	g->ne = 0;
	g->adj = malloc(nv * sizeof(int*));
	for(int i = 0; i < nv; i++){
		g->adj[i] = malloc(nv * sizeof(int));
		for(int j = 0; j < nv; j++){
			g->adj[i][j] = 0;
		}
	}
	return g;
}

void HCG(Graph* g, int* seq1, int* seq2, int r){ // Horizontal Constraint application
	for(int i = 0; i < r; i++){
		int j = 0;
		while(seq1[i] != seq2[j]){ // Find the index of the pair in sequence 2
			j++;
		}
		for(int k = i+1; k < r; k++){ // Interate though all values to the right of index in seq1
			for(int z = j+1; z < r; z++){ // Interate though all values to the right of index in seq2
				if(seq1[k] == seq2[z]){ // Value is to the right of rectangle in both sequences
					g->adj[seq1[i]-1][seq1[k]-1] = 1; // seq1[k] is to the right of seq1[i]
					break; // Sequence already found begin search for next sequence
				}
			}
		}
	}
}

void VCG(Graph* g, int* seq1, int* seq2, int r){ // Create Vertical Constraint Graph
	for(int i = 0; i < r; i++){
		int j = 0;
		while(seq1[i] != seq2[j]){ // Find the index of the pair in sequence 2
			j++;
		}
		for(int k = i+1; k < r; k++){ // Interate though all values to the right of index in seq1
			for(int z = 0; z < j; z++){ // Interate through all values to the left of index in seq2
				if(seq1[k] == seq2[z]){ // Value is between rectangle's place in both sequences
					g->adj[seq1[k]-1][seq1[i]-1] = 1; // seq1[k] is below seq1[i]
					break; // Sequence already found begin search for next sequence
				}
			}
		}
	}
}

void destroyGraph(Graph* g){
	for(int i = 0; i < g->nv; i++){
		free(g->adj[i]);
	}
	free(g->adj);
	free(g);
}

void DFS(Graph* g, int vertex, int* visited, int* list, int* index){
	visited[vertex] = 1;
	for(int i = 0; i < g->nv; i++){
		if(!visited[i] && g->adj[vertex][i]){
			DFS(g, i, visited, list, index);
		}
	}
	list[*index] = vertex+1;
	*index = *index + 1;
}

void y_coords(Graph* g, int* list, double* array, double* coords){
	int vertex = 0;
	for(int i = 0; i < g->nv; i++){
		vertex = list[i] - 1;
		for(int j = 0; j < g->nv; j++){
			if(g->adj[vertex][j]){
				if((array[vertex*3+2] + coords[vertex*3+2]) > coords[j*3+2]){
					coords[j*3+2] = array[vertex*3+2] + coords[vertex*3+2];
				}
			}
		}
	}
}

void x_coords(Graph* g, int* list, double* array, double* coords){
	int size = g->nv;
	int vertex = 0;
	for(int i = 0; i < size; i++){
		vertex = list[i] - 1;
		for(int j = 0; j < size; j++){
			if(g->adj[vertex][j]){
				if((array[vertex*3+1] + coords[vertex*3+1]) > coords[j*3+1]){
					coords[j*3+1] = array[vertex*3+1] + coords[vertex*3+1];
				}
			}
		}
	}
}
	
void writeCoords(double* coords, FILE* fp, int rectangles){
	for(int i = 0; i < rectangles; i++){
		fprintf(fp, "%d(%le,%le)\n",(int)coords[i*3], coords[i*3+1], coords[i*3+2]);
	}
}
	
void printGraph(Graph* g){ // Display graph (for testing purposes)
	for(int i = 0; i < g->nv; i++){
		printf("%d -> ", i+1);
		for(int j = 0; j < g->nv; j++){
			if(g->adj[i][j] == 1){
				printf("%d ", j+1);
			}
		}
		printf("\n");
	}
}