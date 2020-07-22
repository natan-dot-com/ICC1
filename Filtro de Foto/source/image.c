#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>

// Calcula a média aritmética de três números inteiros a, b e c.
int aritmeticMean(int a, int b, int c) {
	int mean = (a+b+c)/3;
	return mean;
}

// Conserta o problema encontrado na largura de imagens BMP através do padding
void fixWidth(int *width) {
	int aux = (*width) % 4;
	(*width) += 4-aux;
}

// Checa se o arquivo de entrada é BMP
int checkBmpFile(char *filename) {
	char bmp[4] = ".bmp";
	int lengh = strlen(filename);
	for (int i = lengh-1, pos = 3; i > lengh-5; i--, pos--) {
		if (filename[i] != bmp[pos]) {return INVALID_BMP_FILE;}
	}

	return SUCCESS;
}

// Lê o cabeçalho do arquivo BMP e checa se o arquivo é uma imagem válida, 
// através da assinatura BM
File_Header *getFileHeader(FILE *fp_bmp, int *error) {
	File_Header *f_header = (File_Header *) malloc(sizeof(File_Header));
	(*f_header).signature = (char *) calloc(3, sizeof(char));

	for(int i = 0; i < 2; i++) {
		(*f_header).signature[i] = fgetc(fp_bmp);
	}
	if (strcmp((*f_header).signature, "BM")) {
		*error = INVALID_FILE_TYPE;
		return NULL;
	}
	
	fread(&(f_header->file_size), sizeof(int), 1, fp_bmp);
	fread(&(f_header->reserved), sizeof(int), 1, fp_bmp);
	fread(&(f_header->offset), sizeof(int), 1, fp_bmp);
	
	return f_header;
}

// Lê o cabeçalho da imagem bitmap
Bitmap_Header *getBitmapHeader(FILE *fp_bmp) {
	Bitmap_Header *b_header = (Bitmap_Header *) malloc(sizeof(Bitmap_Header));
	fread(&(b_header->header_size), sizeof(int), 1, fp_bmp);
	fread(&(b_header->image_width), sizeof(int), 1, fp_bmp);
	fread(&(b_header->image_height), sizeof(int), 1, fp_bmp);
	fread(&(b_header->plan_number), sizeof(short), 1, fp_bmp);
	fread(&(b_header->bits_per_pixel), sizeof(short), 1, fp_bmp);
	fread(&(b_header->compression), sizeof(int), 1, fp_bmp);
	fread(&(b_header->image_size), sizeof(int), 1, fp_bmp);
	fread(&(b_header->horizontal_pixel), sizeof(int), 1, fp_bmp);
	fread(&(b_header->vertical_pixel), sizeof(int), 1, fp_bmp);
	fread(&(b_header->colors_number), sizeof(int), 1, fp_bmp);
	fread(&(b_header->colors_in_image), sizeof(int), 1, fp_bmp);

	return b_header;
}

// Lê a área de dados que define a imagem, já com a correção do padding
int **getImageData(FILE *fp_bmp, Bitmap_Header *b_header) {
	int height = (*b_header).image_height;
	int width = (*b_header).image_width;
	if (width % 4 != 0) {
		fixWidth(&width);
	}
	
	int **ImageData = (int **) malloc(height*sizeof(int *));
	for (int i = 0; i < height; i++) {
		ImageData[height-1-i] = (int *) malloc(width*sizeof(int));
		for (int j = 0; j < width; j++) {
			fread(&(ImageData[height-1-i][j]), 1, 1, fp_bmp);
		}
	}
	
	return ImageData;
}

// Lê a paleta de 256 cores RGB do arquivo BMP 
Color_Palette *getColorPalette(FILE *fp_bmp) {
	Color_Palette *c_palette = (Color_Palette *) malloc(256*sizeof(Color_Palette));
	for(int i = 0; i < 256; i++) {
		c_palette[i].blue = 0;
		c_palette[i].green = 0;
		c_palette[i].red = 0;
		c_palette[i].reserved = 0;

		fread(&(c_palette[i].blue), 1, 1, fp_bmp);
		fread(&(c_palette[i].green), 1, 1, fp_bmp);
		fread(&(c_palette[i].red), 1, 1, fp_bmp);
		fread(&(c_palette[i].reserved), 1, 1, fp_bmp);
	}

	return c_palette;
}

// Modifica as cores RGB lidas para aplicar o efeito de foto negativa
void NegativeEffect(Color_Palette **c_palette) {
	for (int i = 0; i < 256; i++) {
		(*c_palette)[i].blue = 255 - (*c_palette)[i].blue;
		(*c_palette)[i].green = 255 - (*c_palette)[i].green;
		(*c_palette)[i].red = 255 - (*c_palette)[i].red;
	}
}

// Modifica as cores RGB lidas para aplicar o efeito de foto preto e branca
void BlackAndWhiteEffect(Color_Palette **c_palette) {
	for (int i = 0; i < 256; i++) {
		int aritmetic_mean = aritmeticMean((*c_palette)[i].blue, (*c_palette)[i].red, (*c_palette)[i].green);
		(*c_palette)[i].blue = aritmetic_mean;
		(*c_palette)[i].green = aritmetic_mean;
		(*c_palette)[i].red = aritmetic_mean;
	}
}

// Produz o nome da nova imagem: filenameNegativo.bmp ou filenamePretoBranco.bmp
void changeFilename(char **filename, const int effect_type) {
	(*filename)[strlen(*filename)-4] = '\0';
	int fn_lengh = strlen(*filename);
	switch (effect_type) {
		case 1 : {
			char *new_filename = "Negativo.bmp";
			int new_fn_lengh = strlen(new_filename);
			(*filename) = (char *) realloc((*filename), (fn_lengh+new_fn_lengh+1)*sizeof(char));
			strcat((*filename), new_filename);
			break;
		}
		case 2 : {
			char *new_filename = "PretoBranco.bmp";
			int new_fn_lengh = strlen(new_filename);
			(*filename) = (char *) realloc((*filename), (fn_lengh+new_fn_lengh+1)*sizeof(char));
			strcat((*filename), new_filename);
			break;
		}
	}
}

// Escreve o cabeçalho do arquivo de imagem em um novo arquivo 
size_t writeFileReader(FILE *fp_newbmp, File_Header *f_header) {
	fprintf(fp_newbmp, "%s", (*f_header).signature);
	fwrite(&(f_header->file_size), sizeof(int), 1, fp_newbmp);
	fwrite(&(f_header->reserved), sizeof(int), 1, fp_newbmp);
	fwrite(&(f_header->offset), sizeof(int), 1, fp_newbmp);
	return ftell(fp_newbmp);
}

// Escreve o cabeçalho bitmap da imagem em um novo arquivo
size_t writeBitmapHeader(FILE *fp_newbmp, Bitmap_Header *b_header) {
	fwrite(&(b_header->header_size), sizeof(int), 1, fp_newbmp);
	fwrite(&(b_header->image_width), sizeof(int), 1, fp_newbmp);
	fwrite(&(b_header->image_height), sizeof(int), 1, fp_newbmp);
	fwrite(&(b_header->plan_number), sizeof(short), 1, fp_newbmp);
	fwrite(&(b_header->bits_per_pixel), sizeof(short), 1, fp_newbmp);
	fwrite(&(b_header->compression), sizeof(int), 1, fp_newbmp);
	fwrite(&(b_header->image_size), sizeof(int), 1, fp_newbmp);
	fwrite(&(b_header->horizontal_pixel), sizeof(int), 1, fp_newbmp);
	fwrite(&(b_header->vertical_pixel), sizeof(int), 1, fp_newbmp);
	fwrite(&(b_header->colors_number), sizeof(int), 1, fp_newbmp);
	fwrite(&(b_header->colors_in_image), sizeof(int), 1, fp_newbmp);

	return ftell(fp_newbmp);
}

// Escreve a paleta de cores modificada em um novo arquivo
size_t writeColorPalette(FILE *fp_newbmp, Color_Palette *c_palette) {
	for(int i = 0; i < 256; i++) {
		fwrite(&(c_palette[i].blue), 1, 1, fp_newbmp);
		fwrite(&(c_palette[i].green), 1, 1, fp_newbmp);
		fwrite(&(c_palette[i].red), 1, 1, fp_newbmp);
		fwrite(&(c_palette[i].reserved), 1, 1, fp_newbmp);
	}

	return ftell(fp_newbmp);
}

// Escreve a área de dados da imagem em um novo arquivo 
size_t writeImageData(FILE *fp_newbmp, int **ImageData, Bitmap_Header *b_header) {
	int width = (*b_header).image_width;
	int height = (*b_header).image_height;

	if (width % 4 != 0) {
		fixWidth(&width);
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			fwrite(&(ImageData[height-1-i][j]), 1, 1, fp_newbmp);
		}
	}

	return ftell(fp_newbmp);
}

// Exibe os dados da imagem lida na tela
void printHeader(File_Header *f_header, Bitmap_Header *b_header) {
	printf("CABECALHO:\n");
	printf("Iniciais: %s\n", (*f_header).signature);
	printf("Tamanho do arquivo: %d\n", (*f_header).file_size);
	printf("Reservado: %d\n", (*f_header).reserved);
	printf("Deslocamento, em bytes, para o inicio da area de dados: %d\n", (*f_header).offset);
	printf("Tamanho em bytes do segundo cabecalho: %d\n", (*b_header).header_size);
	printf("Resolucao: %d x %d\n", (*b_header).image_width, (*b_header).image_height);
	printf("Numero de planos: %hd\n", (*b_header).plan_number);
	printf("Bits por pixel: %hd\n", (*b_header).bits_per_pixel);
	printf("Compressao usada: %d\n", (*b_header).compression);
	printf("Tamanho imagem: %d\n", (*b_header).image_size);
	printf("Resolucao horizontal: %d pixel por metro\n", (*b_header).horizontal_pixel);
	printf("Resolucao Vertical: %d pixel por metro\n", (*b_header).vertical_pixel);
	printf("Numero de cores usadas: %d\n", (*b_header).colors_number);
	printf("Numero de cores importantes: %d\n", (*b_header).colors_in_image);
}

// Exibe a paleta de cores lida na tela
void printPalette(Color_Palette *c_palette) {
	for(int i = 0; i < 256; i++) {
		printf("Paleta[%d]: R:%d G:%d B:%d\n", i, c_palette[i].red, c_palette[i].green, c_palette[i].blue);
	}
}

// Exibe a soma dos dados da imagem de cada linha, subtraindo uma unidade para cada algarismo de padding que fora feito anteriormente 
void printSumOfLines(int **ImageData, Bitmap_Header *b_header) {
	int height = (*b_header).image_height;
	int width = (*b_header).image_width;
	if (width % 4 != 0) {
		fixWidth(&width);
	}

	int padding = width - (*b_header).image_width;
	for (int i = 0; i < height; i++) {
		long long sum_of_line = 0;
		for (int j = 0; j < (*b_header).image_width; j++) {
			sum_of_line += ImageData[i][j];
		}
		for (int k = 0; k < padding; k++) {
			sum_of_line -= 1;
		}
		printf("Soma linha %d: %lld\n", i, sum_of_line);
	}
}


void freeFileHeader(File_Header *f_header) {
	free((*f_header).signature);
	free(f_header);
}

void freeImageData(int **ImageData, int height) {
	for (int i = 0; i < height; i++) {
		free(ImageData[i]);
	}
	free(ImageData);
}
