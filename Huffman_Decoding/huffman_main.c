#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "huffman.h"


int main(int argc, char* argv[]) 
{		
	if(argv[1][1] == 'c'){
		// Read in file
		FILE* input = fopen(argv[2], "r");
		if(input == NULL){
			return EXIT_FAILURE;
		}
		long num_chars = 0;
		fread(&num_chars, sizeof(num_chars), 1, input);
		long header_chars = 0;
		fread(&header_chars, sizeof(header_chars), 1, input);
		long uncomp_chars = 0;
		fread(&uncomp_chars, sizeof(uncomp_chars), 1, input);
	
		int i = 0;
		char* array = malloc(sizeof(*array) * header_chars);
		while(i < header_chars){
			fread(&(array[i]), sizeof(*array), 1, input);
			i = i + 1;
		}

		i = 0;
		char* code = malloc(sizeof(*code) * (num_chars - header_chars - (3*8)));
		while(i < (num_chars - header_chars - (3*8))){
			fread(&(code[i]), sizeof(*code), 1, input);
			i = i + 1;
		}
		fclose(input);
		// Tranfer stream at the end of file into bit form
		int* bits = bitify(code);
		// Construct a huffman tree based on the header
		List* list = construct_huffman(array, header_chars);
		Tree* tree = list->start->info->right;
		// Determine code representation for the header
		int path[8] = {0};
		FILE* output = fopen(argv[3], "w");
		if(output == NULL){
			return EXIT_FAILURE;
		}
		huffman_values(tree, path, 0, output);
		fclose(output);
		// Determine the original phrase
		FILE* output_two = fopen(argv[4], "w");
		if(output_two == NULL){
			return EXIT_FAILURE;
		}
		transverse_huffman(output_two, list, bits, uncomp_chars);
		fclose(output_two);
		// Free allocated memory
		free(bits);
		free_node(list);
		free(list);
		free(array);
		free(code);
	}
	else if(argv[1][1] == 'b'){
		// Read in the full compessed file
		FILE* fp = fopen(argv[2], "r");
		if(fp == NULL){
			return EXIT_FAILURE;
		}
		// Read in first 3 long ints
		long num_chars = 0;
		fread(&num_chars, sizeof(num_chars), 1, fp);
		long header_chars = 0;
		fread(&header_chars, sizeof(header_chars), 1, fp);
		long uncomp_chars = 0;
		fread(&uncomp_chars, sizeof(uncomp_chars), 1, fp);
		// Read in header info pre bit representation
		char* compressed = malloc(sizeof(*compressed) * header_chars);
		int i = 0;
		while(i < header_chars){
			fread(&(compressed[i]), sizeof(*compressed), 1, fp);
			i = i + 1;
		}
		i = 0;
		char* code = malloc(sizeof(*code) * (num_chars - header_chars - (3*8)));
		while(i < (num_chars - header_chars - (3*8))){
			fread(&(code[i]), sizeof(*code), 1, fp);
			i = i + 1;
		}
		fclose(fp);	
		// Determine the char representation of the header
		char* array = header_char(compressed, header_chars);
		// Convert stream of data at eof to bits
		int* bits = bitify(code);
		
		List* list = construct_huffman(array, strlen(array));	

		FILE* output = fopen(argv[3], "w");
		if(output == NULL){
			return EXIT_FAILURE;
		}
		transverse_huffman(output, list, bits, uncomp_chars);
		fclose(output);
		
		free(code);
		free(compressed);
		free(bits);
		free(array);
		free_node(list);
		free(list);
	}
	else{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
