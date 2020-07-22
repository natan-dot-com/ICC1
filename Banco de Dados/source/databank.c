#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <databank.h>
#include <utility.h>

// Seção que trata das funções para manipulação do arquivo de metadados //

// Abre o arquivo de metadados
FILE *openMetadataFile() {
	FILE *fp;
	char *file = readline(stdin);
	fp = fopen(file, "r");
	free(file);
	return fp;
}

// Coleta o campo de filename dentro do arquivo de metadados, retornando dois nomes de arquivo: filename.reg e filename.idx
void getFilename(char **reg_filename, char **idx_filename, FILE *fp_metadata) {
	fscanf(fp_metadata, "filename: ");
	
	(*reg_filename) = readline(fp_metadata);
	int filename_lengh = strlen(*reg_filename);

	(*idx_filename) = (char *) malloc((filename_lengh+1)*sizeof(char));
	strcpy(*idx_filename, *reg_filename);
	
	(*idx_filename)[filename_lengh-3] = 'i';
	(*idx_filename)[filename_lengh-2] = 'd';
	(*idx_filename)[filename_lengh-1] = 'x';
}

// Lê os rótulos dentro do arquivo de metadados para que sejam ignorados
void readLabels(FILE *fp_metadata, const int label) {
	switch (label) {
		case KEYNAME : {fscanf(fp_metadata, "key-name: "); break;}
		case KEYTYPE : {fscanf(fp_metadata, "key-type: "); break;}
		case FIELDNAME : {fscanf(fp_metadata, "field-name: "); break;}
		case FIELDTYPE : {fscanf(fp_metadata, "field-type: "); break;}
	}
}

// Calcula o tamanho em bytes de cada campo após terem sido lidos. Uma vez que as palavras int, char, float e double possuem todas 
// tamanhos distintos, para analisar o tipo de dado do campo pode ser utilizado um strlen. O algoritmo feito em default (para strings)
// é responsável por ler o número entre colchetes do tipo de dado (e.g.: char[50]) e transformar esse número em um inteiro.
void getSize(Metadata **data_bank, const int counter) {
	for (int i = 0; i < counter; i++) {
		int lengh = strlen((*data_bank)[i].field_type);
		if (lengh >= 3) {
			switch (lengh) {
				case TYPE_INT : {(*data_bank)[i].size = sizeof(int); break;}
				case TYPE_CHAR : {(*data_bank)[i].size = sizeof(char); break;}
				case TYPE_FLOAT : {(*data_bank)[i].size = sizeof(float); break;}
				case TYPE_DOUBLE : {(*data_bank)[i].size = sizeof(double); break;}
				default : {
					int number_lengh = 0;
					char *aux = NULL;

					for (int j = 5; j < lengh-1; j++) {
						aux = (char *) realloc(aux, (number_lengh+2)*sizeof(char));
						aux[number_lengh] = (*data_bank)[i].field_type[j];
						number_lengh++;
					}
					aux[number_lengh] = '\0';

					(*data_bank)[i].size = atoi(aux);
					free(aux);
					break;
				}
			}
		}
	}
}

// Extrai todas as informações do arquivo de metadados e calcula o tamanho em bytes de cada campo. Por conveniência, é conhecido que 
// a chave é sempre a posição zero da struct de metadados
Metadata *extractMetadata(FILE *fp_metadata, int *counter) {
	Metadata *data_bank = NULL;
	(*counter) = 0;

	data_bank = (Metadata *) malloc(sizeof(Metadata));
	readLabels(fp_metadata, KEYNAME);
	data_bank[*counter].field_name = readline(fp_metadata);
	readLabels(fp_metadata, KEYTYPE);
	data_bank[*counter].field_type = readline(fp_metadata);
	(*counter)++;

	do {
		readLabels(fp_metadata, FIELDNAME);
		char *aux = readline(fp_metadata);
		readLabels(fp_metadata, FIELDTYPE);
		char *aux2 = readline(fp_metadata);

		if (strcmp(aux, "") && strcmp(aux2, "")) {
			data_bank = (Metadata *) realloc(data_bank, ((*counter)+1)*sizeof(Metadata));

			data_bank[*counter].field_name = (char *) malloc((strlen(aux)+1)*sizeof(char));
			strcpy(data_bank[*counter].field_name, aux);
			data_bank[*counter].field_type = (char *) malloc((strlen(aux2)+1)*sizeof(char));
			strcpy(data_bank[*counter].field_type, aux2);
			(*counter)++;
		}
		free(aux);
		free(aux2);
	} while(!feof(fp_metadata));

	getSize(&data_bank, *counter);
	return data_bank;
}

// Calcula o offset global que o conjunto de dados terá, isto é, a soma do tamanho (em bytes) dos tipos de cada campo
int getGlobalOffset(Metadata *data_bank, const int field_counter) {
	int global_offset = 0;
	for (int i = 0; i < field_counter; i++) {
		global_offset += data_bank[i].size;
	}
	return global_offset;
}

void freeMetadataStruct(Metadata *data_bank, const int counter) {
	for (int i = 0; i < counter; i++) {
		free(data_bank[i].field_name);
		free(data_bank[i].field_type);
	}
	free(data_bank);
}

// Seção que trata das funções do comando "insert" //

// Coleta as informações, fornecidas de entrada, para um conjunto de dados
void readInsertData(Metadata **data_bank, const int field_counter) {
	scanf(" ");
	for (int i = 0; i < field_counter; i++) {
		int lengh = strlen((*data_bank)[i].field_type);
		if (lengh >= 3) {
			switch (lengh) {
				case TYPE_INT : {
					int aux;
					scanf("%d", &aux);
					(*data_bank)[i].data.integer_type = aux;
					if (i != field_counter-1) {scanf(", ");}
					break;
				}
				// 39 representa o número ASCII do apóstrofe. O algoritmo ignora os apóstrofes passados de entrada
				case TYPE_CHAR : {
					char aux;
					int aphostrophe_flag = 0;

					do {
						aux = (char) fgetc(stdin);
						if (aux != 39) {(*data_bank)[i].data.char_type = aux;}
						else {aphostrophe_flag++;}
					} while (aphostrophe_flag < 2);

					if (i != field_counter-1) {scanf(", ");}
					break;
				}
				case TYPE_FLOAT : {
					float aux;
					scanf("%f", &aux);
					(*data_bank)[i].data.float_type = aux;
					if (i != field_counter-1) {scanf(", ");}
					break;
				}
				case TYPE_DOUBLE : {
					double aux;
					scanf("%lf", &aux);
					(*data_bank)[i].data.double_type = aux;
					if (i != field_counter-1) {scanf(", ");}
					break;
				}
				// 34 representa o código ASCII das aspas. Assim, o algoritmo ignora as aspas passadas pela entrada
				default : {
					(*data_bank)[i].data.string_type = NULL;
					(*data_bank)[i].data.string_type = (char *) calloc(((*data_bank)[i].size+1), sizeof(char));

					int pos = 0;
					int quotation_marks_flag = 0;

					do {
						char ch = (char) fgetc(stdin);
						if (ch != 34) {(*data_bank)[i].data.string_type[pos++] = ch;} 
						else {quotation_marks_flag++;}
					} while (quotation_marks_flag < 2);

					if (i != field_counter-1) {scanf(", ");}
					break;
				}
			}
		}
	}
}

// Escreve o conjunto de dados lido no arquivo .reg. A função devolve o número de bytes escritos após todos os campos
// terem sido escritos, este é usado para o cálculo do offset daquele conjunto de dados.
size_t writeFile(Metadata *data_bank, const int field_counter, FILE *fp_reg) {
	for (int i = 0; i < field_counter; i++) {
		int lengh = strlen(data_bank[i].field_type);
		if (lengh >= 3) {
			switch (lengh) {
				case TYPE_INT : {
					fwrite(&(data_bank[i].data.integer_type), sizeof(int), 1, fp_reg);
					break;
				}
				case TYPE_CHAR : {
					fwrite(&(data_bank[i].data.char_type), sizeof(char), 1, fp_reg);
					break;
				}
				case TYPE_FLOAT : {
					fwrite(&(data_bank[i].data.float_type), sizeof(float), 1, fp_reg);
					break;
				}
				case TYPE_DOUBLE : {
					fwrite(&(data_bank[i].data.double_type), sizeof(double), 1, fp_reg);
					break;
				}
				default : {
					fwrite(data_bank[i].data.string_type, sizeof(char), data_bank[i].size, fp_reg);
					break;
				}
			}
		}
	}
	size_t bytesWritten = ftell(fp_reg);
	return bytesWritten;
}

// Calcula o offset em que o conjunto de dados, que acabou de ser colocado, se encontra no arquivo. Além disso, transfere a chave 
// e o offset para uma struct a parte, que é usada para gerar o arquivo .idx  
void toInsertedDataStruct(Metadata *data_bank, Idx_File **insert, int *insert_counter, const int globalOffset, const size_t bytesWritten) {
	*insert = (Idx_File *) realloc((*insert), ((*insert_counter)+1)*sizeof(Idx_File));
	(*insert)[*insert_counter].offset = bytesWritten-globalOffset;
	(*insert)[*insert_counter].key = data_bank[0].data.integer_type;
	(*insert_counter)++;
}

// Limpa as strings que foram utilizadas, para um possível novo insert
void freeStrings(Metadata *data_bank, const int field_counter) {
	for(int i = 0; i < field_counter; i++) {
		if (strlen(data_bank[i].field_type) > 6) {
			free(data_bank[i].data.string_type);
		}
	}
}


// Seção que trata das funções do comando "index" //

// Carrega um arquivo .idx já existente para a memória RAM. Utilizado no comando index e no comando search
void readIdxFile(Idx_File **idx_file_struct, int *idx_file_struct_counter, FILE *fp_idx) {
	do {
        if (!feof(fp_idx)) {
            (*idx_file_struct) = (Idx_File *) realloc((*idx_file_struct), ((*idx_file_struct_counter)+1)*sizeof(Idx_File));
            fread(&(*idx_file_struct)[*idx_file_struct_counter].key, sizeof(int), 1, fp_idx);
            fread(&(*idx_file_struct)[*idx_file_struct_counter].offset, sizeof(int), 1, fp_idx);
            (*idx_file_struct_counter)++;
        }
	} while (!feof(fp_idx));
}

// Checa se já existe um arquivo .idx existente. Se sim, a função o carrega.
void loadIdxFile(char *filename_idx, Idx_File **idx_file_struct, int *idx_file_struct_counter) {
	FILE *check_file = fopen(filename_idx, "rb");
	if (check_file) {
		readIdxFile(idx_file_struct, idx_file_struct_counter, check_file);
		fclose(check_file);
	}
}

// Função para escrita do novo arquivo .idx
void writeIdxFile(FILE *fp_idx, Idx_File *insert, const int insert_counter) { 
	for (int i = 0; i < insert_counter; i++) {
            fwrite(&(insert[i].key), sizeof(int), 1, fp_idx);
            fwrite(&(insert[i].offset), sizeof(int), 1, fp_idx);
	}
}

// Limpa a struct que acabou de ser escrita no arquivo .idx e a devolve pronta para ser utilizada novamente
Idx_File *clearInsertStruct(Idx_File *insert, int *insert_counter) {
	free(insert);
	Idx_File *insert_cleared = NULL;
	*insert_counter = 0;
	return insert_cleared;
}

// Seção que trata das funções do comando "search"

// Transfere os dados já inseridos que estão carregados na memória para a lista de procura do comando search
void fromInsertStructToSearchStruct(Idx_File **search, int *search_counter, Idx_File *insert, const int insert_counter) {
	for (int i = 0; i < insert_counter; i++) {
		*search = (Idx_File *) realloc(*search, ((*search_counter)+1)*sizeof(Idx_File));
		(*search)[*search_counter].key = insert[i].key;
		(*search)[*search_counter].offset = insert[i].offset;
		(*search_counter)++;
	}
}

// Cria a lista para procura através do carregamento do arquivo .idx e das chaves já carregadas na memória
Idx_File *loadInsertedData(int *search_counter, Idx_File *insert, const int insert_counter, char *idx_filename) {
	Idx_File *search = NULL;
	(*search_counter) = 0;
	loadIdxFile(idx_filename, &search, search_counter); 
	if (insert) {
		fromInsertStructToSearchStruct(&search, search_counter, insert, insert_counter);
		qsort(search, *search_counter, sizeof(Idx_File), numberSort);
	}

	return search;

}

// Função responsável por extrair as informações no offset desejado e exibi-las na tela
void printSearchedData(FILE *fp_src, Metadata **data_bank, const int searched_offset, const int field_counter) {
	fseek(fp_src, searched_offset, SEEK_SET);
	for (int i = 0; i < field_counter; i++) {
		int lengh = strlen((*data_bank)[i].field_type);
		switch (lengh) {
			case TYPE_INT : {
				fread(&(*data_bank)[i].data.integer_type, sizeof(int), 1, fp_src);
				printf("%s: %d\n", (*data_bank)[i].field_name, (*data_bank)[i].data.integer_type);
				break;
			}
			case TYPE_CHAR : {
				fread(&(*data_bank)[i].data.char_type, sizeof(char), 1, fp_src);
				printf("%s: %c\n", (*data_bank)[i].field_name, (*data_bank)[i].data.char_type);
				break;
			}
			case TYPE_FLOAT : {
				fread(&(*data_bank)[i].data.float_type, sizeof(float), 1, fp_src);
				printf("%s: %.2f\n", (*data_bank)[i].field_name, (*data_bank)[i].data.float_type);
				break;
			}
			case TYPE_DOUBLE: {
				fread(&(*data_bank)[i].data.double_type, sizeof(double), 1, fp_src);
				printf("%s: %.2lf\n", (*data_bank)[i].field_name, (*data_bank)[i].data.double_type);
				break;
			}
			default : {
				(*data_bank)[i].data.string_type = (char *) calloc((*data_bank)[i].size+1, sizeof(char));
				fread((*data_bank)[i].data.string_type, sizeof(char), (*data_bank)[i].size, fp_src);
				printf("%s: %s\n", (*data_bank)[i].field_name, (*data_bank)[i].data.string_type);
				free((*data_bank)[i].data.string_type);
				break;
			}
		}
	}
	printf("\n");
}