#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "bmp.h"

int main(int argc, char* argv[])
{
	FILE* fp = fopen("purduetrain.bmp", "r");
	if(fp == NULL)
	{
		fprintf(stderr, "File could not be opened.\n");
		return EXIT_FAILURE;
	}
	
	char* error = NULL;
	
	BMPImage* img = read_bmp(fp, &error);
	if(img == NULL)
	{
		fprintf(stderr, error);
		assert(error != NULL);
		free(error);
		fclose(fp);
		return EXIT_FAILURE;
	}
	fclose(fp);

	int w = img->header.width_px;
	int h = img->header.height_px;
	BMPImage* crop = crop_bmp(img, 0, 0, w, h, &error); // 2 2 4 4
	if(crop == NULL)
	{
		fprintf(stderr, error);
		assert(error != NULL);
		free(error);
		free_bmp(img);
		return EXIT_FAILURE;
	}
	free_bmp(img);

	FILE* op = fopen("comp.bmp", "w");
	if(op == NULL)
	{
		fprintf(stderr, error);
		assert(error != NULL);
		free(error);
		free_bmp(crop);
		return EXIT_FAILURE;
	}
	bool success = write_bmp(op, crop, &error);
	if(success == false)
	{
		fprintf(stderr, error);
		assert(error != NULL);
		free(error);
		free_bmp(crop);
		fclose(op);
		return EXIT_FAILURE;
	}
	fclose(op);
	free_bmp(crop);
	fprintf(stdout, "Tasks completed with no errors.\n");
	assert(error == NULL);
	return EXIT_SUCCESS;
}
