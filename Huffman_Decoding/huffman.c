#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void free_tnode(Tree*);

void insert_node(List* list, Node* node){
	Node* begining = list->start;
	node->next = begining;
	list->start = node;
}

void remove_node(List* list, Node* node){
	Node* begining = list->start;
	if(begining == node){
		list->start = node->next;
	}
	else{
		while(begining->next != NULL){
			if(begining->next == node){
				begining->next = node->next;
				node->next = NULL;
				free(node);
				break;
			}
			else{
				begining = begining->next;
			}
		}
	}
	return;
}

List* construct_huffman(char* array, int size){
	int i = size;
	int j = 0;
	
	List* list = malloc(sizeof(*list));
	list->start = NULL;
	
	while(i > 0){
		if(array[j] == '1'){
			Tree* tree = malloc(sizeof(*tree));
			tree->ASCII = array[j + 1];
			tree->leaf = 1;
			tree->freq = i;
			tree->left = NULL;
			tree->right = NULL;

			Node* node = malloc(sizeof(*node));
			node->info = tree;
			node->next = NULL;
			insert_node(list, node);

			i = i - 2;
			j = j + 2;
		}
		else if(array[j] == '0'){
			Node* right = list->start;
			Node* left = right->next;

			Tree* parent = malloc(sizeof(*parent));
			parent->leaf = 0;
			parent->ASCII = -1;
			parent->freq = i;
			if(left != NULL)
			{	
				parent->left = left->info;
			}
			parent->right = right->info;

			Node* node = malloc(sizeof(*node));
			node->info = parent;
			node->next = NULL;
			insert_node(list, node);

			remove_node(list, left);
			remove_node(list, right);

			i = i - 1;
			j = j + 1;
		}
		else{
			break;	
		}
	}

	return list;
}

void huffman_values(Tree* tree, int* path, int index, FILE* output){
	if(tree == NULL){
		return;
	}
	if(tree->leaf == 1){
		fprintf(output, "%c:", tree->ASCII);
		for(int i = 0; i < index; i++)
		{
			fprintf(output, "%d", path[i]);
		}
		fputc('\n', output);
		return;
	}
	if(tree->left != NULL){
		path[index] = 0;
		huffman_values(tree->left, path, index + 1, output);
	}
	if(tree->right != NULL){
		path[index] = 1;
		huffman_values(tree->right, path, index + 1, output);	
	}
}

int* bitify(char* code){
	int size = strlen(code);
	int* bin = malloc(sizeof(*bin) * size * 8);
	int i = 0;
	int k = 0;
	while(i < size){
		for(int j = 7; j >= 0; j--)
		{
			bin[k] = ((code[i] & (1<<j)) ? 1 : 0);
			k+=1;
		}
		i+=1;
	}
	return bin;
}

void transverse_huffman(FILE* output, List* list, int* code, int size){
	Tree* tree = list->start->info;
	while(tree->left == NULL){
		tree = tree->right;
	}
	Tree* start = tree;
	int i = 0;
	int j = 0;
	while(i < size){
		if(code[j] == 0){
			tree = tree->left;
		}
		else if(code[j] == 1){
			tree= tree->right;
		}
		
		if(tree == NULL){ 
			return;
		}
		else if(tree->leaf == 1){
			fputc(tree->ASCII, output);
			tree = start;
			i+=1;
		}
		j+=1;
	}
}

char* header_char(char* header_bytes, int size){
	int flag = 0;
	unsigned char one = 1;
	int bits = 1;
	int i = 0;
	int j = 0;
	unsigned char mask = one << 7;
	unsigned char first = 0;
	unsigned char second = 0;
	char* array = malloc(sizeof(*array) * size * 8);
	// Used to determine number of padding zeros
	int zeros = 0;
	int chars = 0;
	// Number of ones in header = number of zeros
	
	while(i < size){
		flag = mask & (unsigned char)header_bytes[i]; // Get flag
		flag = flag >> (8 - bits);
		if(flag == 1){
			array[j] = '1';
			j++;
			first = (255 >> bits) & (unsigned char)header_bytes[i];
			first = first << bits;
			second = (255 << (8 - bits)) & (unsigned char)header_bytes[i+1];
			second = second >> (8 - bits);
			array[j] = first | second;
			j++;
			i++;
			mask = mask >> 1;
			bits++;
			chars++;
		}
		else{
			array[j] = '0';
			j++;
			mask = mask >> 1;
			bits++;
			zeros++;
		}
		if(bits == 9){
			bits = 1;
		}
		if(mask == 0){
			mask = one << 7;
			i++;
		}
	}
	char* final = malloc(sizeof(final) * (j + (chars - zeros)));
	int k = 0;
	while(k < j){ // Tranfer into a properly allocated array
		final[k] = array[k];
		k++;
	}
	while(k < (j + (chars - zeros))){ // Accounts for padding zeros
		final[k] = 0;
		k++;
	}
	free(array);
	return final;
}

void free_tnode(Tree* t_n)
{
	if(t_n->left != NULL)
	{
		free_tnode(t_n->left);
	}
	if(t_n->right != NULL)
	{
		free_tnode(t_n->right);	
	}
	free(t_n);
}

void free_node(List* list)
{
	Node* curr = list->start;
	while(curr != NULL)
	{
		Node* tmp = curr;
		curr = curr->next;
		free_tnode(tmp->info);
		free(tmp);
	}
}
