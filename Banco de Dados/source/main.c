/*
*	Aluno: Natan Henrique Sanches (11795680)
*	ICMC/USP - Bacharelado em Ciências da Computação
*	Sistema de Gerenciamento de Banco de Dados (SGBD)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <databank.h>
#include <utility.h>

#define RUN -10
#define STOP -11

int main(int argc, char *argv[]) {
	FILE *fp_metadata = openMetadataFile();  
	if (!fp_metadata) {exit(0);}
	
	char *reg_filename;
	char *idx_filename;
	getFilename(&reg_filename, &idx_filename, fp_metadata);

	int field_counter;
	Metadata *data_bank = extractMetadata(fp_metadata, &field_counter);
	int global_offset = getGlobalOffset(data_bank, field_counter);
	fclose(fp_metadata);

	int insert_counter = 0;
	Idx_File *insert = NULL;

	char *command;
	int flag_RUN = RUN;
	do {
		command = (char *) calloc(7, sizeof(char));
		scanf("%s", command);
		if(!strcmp(command, "insert")) {
			readInsertData(&data_bank, field_counter);
			FILE *fp_reg = fopen(reg_filename, "ab");
			size_t bytesWritten = writeFile(data_bank, field_counter, fp_reg);
			toInsertedDataStruct(data_bank, &insert, &insert_counter, global_offset, bytesWritten);
			freeStrings(data_bank, field_counter);
			fclose(fp_reg);
		}
		if(!strcmp(command, "index")) {
			loadIdxFile(idx_filename, &insert, &insert_counter); 
			if (insert) {
				FILE *fp_idx = fopen(idx_filename, "wb+");
				qsort(insert, insert_counter, sizeof(Idx_File), numberSort);
				writeIdxFile(fp_idx, insert, insert_counter);
				insert = clearInsertStruct(insert, &insert_counter);
				fclose(fp_idx);
			}
		}
		if(!strcmp(command, "search")) {
			int search_error = 0;
			int searched_key;
			scanf("%d", &searched_key);

			FILE *fp_src = fopen(reg_filename, "rb");
			int search_counter = 0;
			Idx_File *search = loadInsertedData(&search_counter, insert, insert_counter, idx_filename);
			int searched_offset = binarySearch(search, search_counter, searched_key, &search_error);
			if (search_error != KEY_NOT_FOUND) {printSearchedData(fp_src, &data_bank, searched_offset, field_counter);}
			if (search) {free(search);}
			if (fp_src) {fclose(fp_src);}
		}
		if(!strcmp(command, "exit")) {
			if(insert) {free(insert);}
			flag_RUN = STOP;
		}
		free(command);
	} while (flag_RUN == RUN);

	freeMetadataStruct(data_bank, field_counter);
	free(reg_filename);
	free(idx_filename);
	return 0;
}