#ifndef _IMAGE_H_
#define _IMAGE_H_

#define SUCCESS 0
#define INVALID_FILE_TYPE -10
#define FILE_NOT_FOUND -20
#define WRITE_FAILED -30
#define INVALID_BMP_FILE -40

#define FILE_HEADER_SIZE 14

#include <stdio.h>

typedef struct _file_header {
	char *signature;
	int file_size;
	int reserved;
	int offset;
} File_Header;

typedef struct _bmp_header {
	int header_size;
	int image_width;
	int image_height;
	short plan_number;
	short bits_per_pixel;
	int compression;
	int image_size;
	int horizontal_pixel;
	int vertical_pixel;
	int colors_number;
	int colors_in_image;
} Bitmap_Header;

typedef struct _color_palette {
	int blue;
	int green;
	int red;
	int reserved;
} Color_Palette;

int checkBmpFile(char *);
File_Header *getFileHeader(FILE *, int *);
Bitmap_Header *getBitmapHeader(FILE *);
int **getImageData(FILE *, Bitmap_Header *);
Color_Palette *getColorPalette(FILE *);
void NegativeEffect(Color_Palette **);
void BlackAndWhiteEffect(Color_Palette **);
void changeFilename(char **, const int);
size_t writeFileReader(FILE *, File_Header *);
size_t writeBitmapHeader(FILE *, Bitmap_Header *);
size_t writeColorPalette(FILE *, Color_Palette *);
size_t writeImageData(FILE *, int **, Bitmap_Header *);
void printHeader(File_Header *, Bitmap_Header *);
void printPalette(Color_Palette *);
void printSumOfLines(int **, Bitmap_Header *);
void freeFileHeader(File_Header *);
void freeImageData(int **, int);

#endif
