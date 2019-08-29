#ifndef SEQPAIR_H
#define SEQPAIR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Node{
	int num; // Rectangle number
	double width;
	double height;
	int start;
	int end;
	int visited; // 0 is a 'white' node 1 is 'grey' and 2 is 'black'
	struct _Node * next;
}Node;

/*
typedef struct Graph{
	int nv; // Number of verticies
	int ne; // Number of edges
	Node** adjList; // Adjacency List
}Graph;
*/

typedef struct Graph{
	int nv; // Number of verticies
	int ne; // Number of edges
	int** adj; // Adjacency List
}Graph;

Graph* makeGraph(int nv);
Node* insertInBeginning(Node* start, int num, int width, int height);
void insertEdge(Graph *g, int u, int v, double* array);
void deleteEdge(Graph *g, int u, int v);
void Load_From_File(FILE* fp, double* array, int* seq1, int* seq2, int rectangles);
void destroyGraph(Graph* g);
void printGraph(Graph* g);

void HCG(Graph* g, int* seq1, int* seq2, int r);
void int_swap(int* x, int* y);
void dub_swap(double* x, double* y);
void order(Graph* g, int left, int right);
void VCG(Graph* g, int* seq1, int* seq2, int r);
void transitive(Graph* g);
void printGraph(Graph* g);

void ghetto_Dijkstra(Graph * g, int* seq1, int* Nullity, int* bottom, double* coords, double* array);
void writeCoords(double* coords, FILE* fp, int size);
void Save(int* bottom, double* tallest, double* height, int* count, int size, double* coords, double* array);
int Check(int curr, int next, int* seq1);

void findRoots(Graph* g, int* roots);
int rootCheck(Graph* g, int key);
int* topological_sort(Graph* g);
void topo(Graph* g);
void dfs(Graph* g, int i, char* colour, int* time, int* clk);
//void DFS(Graph* g, int vertex, int* visited, int* clk);
void DFS(Graph* g, int vertex, int* visited, int* list, int* index);
void y_coords(Graph* g, int* list, double* array, double* coords);
void x_coords(Graph* g, int* list, double* array, double* coords);
void writeCoords(double* coords, FILE* output, int rectangles);

#endif
