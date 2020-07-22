#ifndef _DATABANK_H_
#define _DATABANK_H_

#include <stdio.h>

#define SUCCESS 0
#define FILE_NOT_FOUND -1
#define KEY_NOT_FOUND -2

#define KEYNAME 10
#define KEYTYPE 11
#define FIELDNAME 12
#define FIELDTYPE 13

// Tamanho da palavra cada tipo de dado. Foi usado para uma análise rápida de qual tipo está sendo tratado com strlen.
// Inteiro (int) = 3 letras; Caractere (char) = 4 letras; ...
#define TYPE_INT 3
#define TYPE_CHAR 4
#define TYPE_FLOAT 5
#define TYPE_DOUBLE 6

// Union para armazenamento dos diferentes tipos de dado
typedef union _info {
	int integer_type;
	char char_type;
	char *string_type;
	float float_type;
	double double_type;
} Info;

// Struct que armazena os metadados e o conteúdo de cada campo
typedef struct _metadata {
	char *field_name;
	char *field_type;
	Info data;
	int size;
} Metadata;

// Struct para geração do arquivo .idx
typedef struct _idx_file {
	int key;
	int offset;
} Idx_File;

FILE *openMetadataFile();
void getFilename(char **, char **, FILE *);
void readLabels(FILE *, const int);
void getSize(Metadata **, const int);
Metadata *extractMetadata(FILE *, int *);
int getGlobalOffset(Metadata *, const int);
void freeMetadataStruct(Metadata *, const int);
void readInsertData(Metadata **, const int);
size_t writeFile(Metadata *, const int, FILE *);
void toInsertedDataStruct(Metadata *, Idx_File **, int *, const int, const size_t);
void freeStrings(Metadata *, const int);
void readIdxFile(Idx_File **, int *, FILE *);
void loadIdxFile(char *, Idx_File **, int *);
void writeIdxFile(FILE *, Idx_File *, const int);
Idx_File *clearInsertStruct(Idx_File *, int *);
void fromInsertStructToSearchStruct(Idx_File **, int *, Idx_File *, const int);
Idx_File *loadInsertedData(int *, Idx_File *, const int, char *);
void printSearchedData(FILE *, Metadata **, const int, const int);

#endif