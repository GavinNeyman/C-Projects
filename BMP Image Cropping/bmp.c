#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bmp.h"

BMPImage* read_bmp(FILE* fp, char** error)
{
	BMPImage* bmp = malloc(sizeof(*bmp));
	if(bmp == NULL)
	{
		if(*error == NULL)
		{
			char* message = "Could not allocate memory.\n";
			*error = malloc((strlen(message) + 1) * sizeof(**error));
			strcpy(*error, message);
		}
		return NULL;
	}

	int check = fread(&(bmp->header), sizeof(bmp->header), 1, fp);
	if(check != 1)
	{
		if(*error == NULL)
		{
			char* message = "Could not read from file.\n";
			*error = malloc((strlen(message) + 1) * sizeof(**error));
			strcpy(*error, message);
		}
		free(bmp);
		return NULL;
	}

	int data_size = bmp->header.image_size_bytes;
	bmp->data = malloc(sizeof(*(bmp->data)) * data_size);
	if(bmp->data == NULL)
	{
		if(*error == NULL)
		{
			char* message = "Could not allocate memory.\n";
			*error = malloc((strlen(message) + 1) * sizeof(**error));
			strcpy(*error, message);
		}
		free(bmp);
		return NULL;
	}
	check = fread(bmp->data, sizeof(*(bmp->data)), data_size, fp);
	if(check != data_size)
	{
		if(*error == NULL)
		{
			char* message = "Could not read from file.\n";
			*error = malloc((strlen(message) + 1) * sizeof(**error));
			strcpy(*error, message);
		}
		free(bmp->data);
		free(bmp);
		return NULL;
	}

	char byte = 0;
	check = fread(&byte, sizeof(byte), 1, fp);
	if(check != 0)
	{
		if(*error == NULL)
		{
			char* message = "File has unread data after image data.\n";
			*error = malloc((strlen(message) + 1) * sizeof(**error));
			strcpy(*error, message);
		}
		free(bmp->data);
		free(bmp);
		return NULL;
	}
	
	bool success = check_bmp_header(&(bmp->header), fp);
	if(success == false)
	{
		if(*error == NULL)
		{
			char* message = "BMP Header is incorrect.\n";
			*error = malloc((strlen(message) + 1) * sizeof(**error));
			strcpy(*error, message);
		}
		free(bmp->data);
		free(bmp);
		return NULL;
	}

	return bmp;
}

bool check_bmp_header(BMPHeader* bmp_hdr, FILE* fp)
{
	if(bmp_hdr->type != 0x4d42) // 1. magic number
	{
		return false;
	}
	if(bmp_hdr->offset != BMP_HEADER_SIZE) // 2. data after header
	{
		return false;
	}
	if(bmp_hdr->dib_header_size != DIB_HEADER_SIZE) // 3. header is correct size
	{
		return false;
	}
	if(bmp_hdr->num_planes != 1) // 4. only one image plane
	{
		return false;
	}
	if(bmp_hdr->compression != 0) // 5. no compression
	{
		return false;
	}
	if(bmp_hdr->num_colors != 0 || bmp_hdr->important_colors != 0) // 6. num and important colours are 0
	{
		return false;
	}
	if(bmp_hdr->bits_per_pixel != 24) // 7. image has 24 bits/pixel
	{
		return false;
	}

	fseek(fp, 0L, SEEK_END);
	int file_size = ftell(fp);
	if(bmp_hdr->size != file_size) // 8. Correct File size
	{
		return false;
	}

	int image_size = file_size - BMP_HEADER_SIZE;
	if(bmp_hdr->image_size_bytes != image_size) // 8. img size is right
	{
		return false;
	}

	return true;
}

bool write_bmp(FILE* fp, BMPImage* image, char** error)
{
	int check = fwrite(&(image->header), sizeof(image->header), 1, fp);
	if(check != 1)
	{	
		if(*error == NULL)
		{
			char* message = "Unable to write to file.\n";
			*error = malloc((strlen(message) + 1) * sizeof(**error));
			strcpy(*error, message);
		}
		return false;
	}
	int len = image->header.size - BMP_HEADER_SIZE;
	check = fwrite(image->data, sizeof(*(image->data)), len, fp);
	if(check != len)
	{
		if(*error == NULL)
		{
			char* message = "Unable to write to file.\n";
			*error = malloc((strlen(message) + 1) * sizeof(**error));
			strcpy(*error, message);
		}
		return false;
	}
	return true;
}

void free_bmp(BMPImage* image)
{
	if(image->data != NULL)
	{
		free(image->data);
	}
	free(image);
}

BMPImage* crop_bmp(BMPImage* image, int x, int y, int w, int h, char** error)
{
	BMPImage* cropped = malloc(sizeof(*cropped));
	if(cropped == NULL)
	{
		if(*error == NULL)
		{
			char* message = "Could not allocate memory.\n";
			*error = malloc((strlen(message) + 1) * sizeof(**error));
			strcpy(*error, message);
		}
		return NULL;
	}
	
	int width = image->header.width_px;
	int height = image->header.height_px;
	int old_padding = 4 - ((width * 3) % 4);
	int padding = 4 - ((w * 3) % 4);
	int bytes = (w * h * 3) + (padding * height);
	cropped->data = malloc(sizeof(*(cropped->data)) * bytes); // adding one to malloc?
	if(cropped->data == NULL)
	{
		if(*error == NULL)
		{
			char* message = "Could not allocate memory.\n";
			*error = malloc((strlen(message) + 1) * sizeof(**error));
			strcpy(*error, message);
		}
		free(cropped);
		return NULL;
	}

	int bot_coord = y + h - 1; // lower right hand corner y coordinate
	int total_y = height - 1; // The highest y coordinate attainable
	int skip = total_y - bot_coord; // number of rows from bottom I want to skip
	int want = ((width * 3) + old_padding) * skip + (x * 3);

	// Error Checking
	if(w <= 0 || h <= 0)
	{
		if(*error == NULL)
		{
			char* message = "Insufficient dimentions.\n";
			*error = malloc((strlen(message) + 1) * sizeof(**error));
			strcpy(*error, message);
		}
		free(cropped->data);
		free(cropped);
		return NULL;
	}
	if(w > width || x > width - 1 || x < 0)
	{
		if(*error == NULL)
		{
			char* message = "Out of bounds of original image.\n";
			*error = malloc((strlen(message) + 1) * sizeof(**error));
			strcpy(*error, message);
		}
		free(cropped->data);
		free(cropped);
		return NULL;
	}
	if(h > height || y > height - 1 || y < 0)
	{
		if(*error == NULL)
		{
			char* message = "Out of bounds of original image.\n";
			*error = malloc((strlen(message) + 1) * sizeof(**error));
			strcpy(*error, message);
		}
		free(cropped->data);
		free(cropped);
		return NULL;
	}

	int row_count = 0;
	int count = 0;
	int col = 0;
	int pad = 0;
	while(row_count < h)
	{
		while(col < (w * 3))
		{
			cropped->data[count] = image->data[col + want];
			col++;
			count++;
		}
		while(pad < padding)
		{
			cropped->data[count] = '\0';
			pad++;
			count++;
		}
		pad = 0;
		col = 0;
		row_count++;
		skip++;
		want = ((width * 3) + old_padding) * skip + (x * 3);
	}

	cropped->header.type = image->header.type;
	cropped->header.size = (uint32_t) (bytes + BMP_HEADER_SIZE);
	cropped->header.reserved1 = image->header.reserved1;
	cropped->header.reserved2 = image->header.reserved2;
	cropped->header.offset = image->header.offset;
	cropped->header.dib_header_size = image->header.dib_header_size;
	cropped->header.width_px = (int32_t) w;
	cropped->header.height_px = (int32_t) h;
	cropped->header.num_planes = image->header.num_planes;
	cropped->header.bits_per_pixel = image->header.bits_per_pixel;
	cropped->header.compression = image->header.compression;
	cropped->header.image_size_bytes = (uint32_t) bytes;
	cropped->header.x_resolution_ppm = image->header.x_resolution_ppm;
	cropped->header.y_resolution_ppm = image->header.y_resolution_ppm;
	cropped->header.num_colors = image->header.num_colors;	
	cropped->header.important_colors = image->header.important_colors;

	return cropped;
}
