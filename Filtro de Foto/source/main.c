/*
*	Aluno: Natan Henrique Sanches (11795680)
*	ICMC/USP - Bacharelado em Ciências da Computação
*	Filtro de Foto
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>

#define NEGATIVE 1
#define BLACK_WHITE 2

char *readline(FILE *stream) {
	char *string = NULL;
	int pos = 0;
	char ch;
	scanf(" ");
	do {
		ch = fgetc(stream);
		if (ch != '\r') {
			string = (char *) realloc(string, (pos+1)*sizeof(char));
			string[pos] = ch;
		}
	} while (string[pos++] != '\n');
	string[pos-1] = '\0';

	return string;
}

int main(int argc, char *argv[]) {
	int error = 0;
	char *filename = readline(stdin);	
	int filter;
	scanf("%d", &filter);

	if (strlen(filename) < 5) {
		printf("Arquivo nao eh do formato BMP\n");
		exit(0);
	}

	error = checkBmpFile(filename);
	if (error == INVALID_BMP_FILE) {
		printf("Arquivo nao eh do formato BMP\n");
		exit(0);
	}

	FILE *fp_bmp = fopen(filename, "rb");
	if (!fp_bmp) {
		printf("Erro no arquivo\n");
		exit(0);
	}

	File_Header *f_header = getFileHeader(fp_bmp, &error);	
	if (error == INVALID_FILE_TYPE) {
		printf("Arquivo nao eh do formato BMP\n");
		exit(0);
	}
	
	Bitmap_Header *b_header = getBitmapHeader(fp_bmp);
	printHeader(f_header, b_header);

	Color_Palette *c_palette = getColorPalette(fp_bmp);
	printf("PALETA ORIGINAL:\n");
	printPalette(c_palette);

	int **ImageData = getImageData(fp_bmp, b_header);
	fclose(fp_bmp);
	
	switch (filter) {
		case NEGATIVE : {
			NegativeEffect(&c_palette);
			changeFilename(&filename, NEGATIVE);
			break;
		}
		case BLACK_WHITE : {
			BlackAndWhiteEffect(&c_palette);
			changeFilename(&filename, BLACK_WHITE);
			break;
		}
	}
	printf("PALETA NOVA:\n");
	printPalette(c_palette);

	FILE *fp_newbmp = fopen(filename, "wb+");
        
    	size_t bytesToWrite = FILE_HEADER_SIZE;
    	size_t bytesWritten = writeFileReader(fp_newbmp, f_header);
    	if (bytesWritten != bytesToWrite) {
        	error = WRITE_FAILED;
        	exit(0);
    	}
	freeFileHeader(f_header);

    	bytesToWrite += (*b_header).header_size;
	bytesWritten = writeBitmapHeader(fp_newbmp, b_header);
    	if (bytesWritten != bytesToWrite) {
        	error = WRITE_FAILED;
        	exit(0);
    	}
	
    	bytesToWrite = (*f_header).offset;
	bytesWritten = writeColorPalette(fp_newbmp, c_palette);
    	if (bytesWritten != bytesToWrite) {
        	error = WRITE_FAILED;
        	exit(0);
    	}
	free(c_palette);

	writeImageData(fp_newbmp, ImageData, b_header);
	fclose(fp_newbmp);

	printSumOfLines(ImageData, b_header);
	freeImageData(ImageData, (*b_header).image_height);
	free(b_header);

	printf("%s\n", filename);
	free(filename);
	return 0;
}
