//rpp25
//This program either inverts or grayscales an image
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#pragma pack(1)

typedef struct{
	unsigned short int format;
	unsigned int file_size;
	unsigned short int res1;
 	unsigned short int res2;
 	unsigned int offset;
} Header;

typedef struct{
	unsigned int size;
	int width;
	int height;
	unsigned short int planes;
	unsigned short int bits;
 	unsigned int compress;
	unsigned int imageSize;
	int xres;
	int yres;
	unsigned int colors;
 	unsigned int impColors;
} DIBHeader;

typedef struct{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
} RGB;

int main(int argc, char *argv[]){
	FILE *input;
	FILE *output;
 	Header head;
 	DIBHeader dib;
	RGB colors;
	int height;
	int padBytes;
	char invertGrey;
	int i, j, k;
	float m;
	if(strcmp(argv[1], "-invert") == 0) {
		invertGrey = 'a';
	}
	else if(strcmp(argv[1], "-grayscale") == 0) {
		invertGrey = 'b';
	}
	else {
		exit(1);
	}
	input = fopen(argv[2], "r");
	if(input == NULL) {
		printf("Error\n");
			exit(1);
	}
	fread(&head, sizeof(Header), 1, input);
	fread(&dib, sizeof(DIBHeader), 1, input);
	if(head.format != 0x4d42 || dib.bits != 24)  {
		printf("Format not supported\n");
		fclose(input);
		fclose(output);
		exit(1);
	}

	switch (invertGrey) {
		case 'a':
		output = fopen("inverted.bmp", "w");
		if(output == NULL) {
			printf("Error\n");
			exit(1);
		}
		fwrite(&head, sizeof(Header), 1, output);
		fwrite(&dib, sizeof(DIBHeader), 1, output);
		padBytes = (4 - (dib.width*sizeof(RGB)) % 4) % 4;
		//padBytes bytes used to force the next pixel row to start at an offset that is a multiple of 4.
		height = dib.height; 
		for (i = 0; i < height; i++) {
			for (j = 0; j < dib.width; j++) {
				fread(&colors, sizeof(RGB), 1, input);
				colors.blue = ~colors.blue;
				colors.green = ~colors.green;
				colors.red = ~colors.red;
				fwrite(&colors, sizeof(RGB), 1, output);
			}
			fseek(input, padBytes, SEEK_CUR);
			for (k = 0; k < padBytes; k++) {
				fputc(0, output);
			}
		}
		break;
		
		case 'b':
		output = fopen("grayscale.bmp", "w");
		if(output == NULL) {
			printf("Error\n");
			exit(1);
		}
		fwrite(&head, sizeof(Header), 1, output);
		fwrite(&dib, sizeof(DIBHeader), 1, output);
		padBytes = (4 - (dib.width * sizeof(RGB)) % 4) % 4;
		height = dib.height;
		for (i = 0; i < height; i++) {
			for (j = 0; j < dib.width; j++) {
				fread(&colors, sizeof(RGB), 1, input);
				m = (0.2126 * colors.red) + (0.7152 * colors.green) + (0.0722 * colors.blue);
				colors.blue = (char) m;
				colors.green = (char) m;
				colors.red = (char) m;
				fwrite(&colors, sizeof(RGB), 1, output);
			}
			fseek(input, padBytes, SEEK_CUR);
			for (k = 0; k < padBytes; k++) {
				fputc(0, output);
			}
		}
		break;
		
		default:
		printf("Error\n");
		exit(1);
	}
	
	fclose(input);
	fclose(output);
	return 0;
}