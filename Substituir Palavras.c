/*
*	Aluno: Natan Henrique Sanches (11795680)
*	ICMC/USP - Bacharelado em Ciências de Computação
*	Substituir palavras em uma frase
*
*	Lógica utilizada para a criação do código.
*	De maneira direta, a frase foi lida e alocada pela função readline em um array simples. Logo após, através de outra função,
*	transferimos esse array para uma matriz, de maneira a manter o que denominei de "caracteres especiais" (espaços, números, pon-
*	tuação) separados das palavras (em linhas diferentes da matriz). Isso possibilita a manipulação das palavras sem risco de sobreposição
*	com o que é constante na frase.
*	Em seguida, utilizamos duas funções: uma para procurar a palavra a ser substituida na matriz, e outra que realiza a substituição.
*
*	Os defines listados representam, respectivamente:
*	READLINE_BUFFER: Buffer da função readline, onde serão armazenados palavras/frases de entrada.
*	END_WORD_POSITION: Representará o finalizador da sequência de números. Tem papel semelhante ao '\0' das strings.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define READLINE_BUFFER 4096
#define END_WORD_POSITION -1

//
//	Funções para manipulação de frase
//

// substituteWord = Função que recebe as linhas da matriz que estão as palavras a serem substituidas e realiza
// essa mesma substituição
void substituteWord(char **matrix, char *substitute, int *word_position) {
	int substitute_lengh = strlen(substitute);
	int value;

	for (int k = 0; word_position[k] != END_WORD_POSITION; k++) {
		value = word_position[k];
		strcpy(matrix[value], substitute);

		for (int j = substitute_lengh; j < 20; j++) {
			matrix[value][j] = 0;
		}
	}
}

// wordFinder = Função responsável por encontrar as linhas da matriz onde está a palavra a ser substituida.
int *wordFinder(char **matrix, char *word, int const counter) {
	int *word_position = NULL;
	int word_lengh = strlen(word);
	int flag;
	int pos = 0;

	for (int i = 0; i < counter; i++) {
		flag = 0;
		if (matrix[i][0] == word[0]) {
			for (int j = 0; j < word_lengh; j++) {
				if (matrix[i][j] == word[j])
					flag++;
			}
			if (matrix[i][word_lengh] == 0) {
				flag++;
			}
			if (flag == word_lengh+1) {
				word_position = (int *) realloc(word_position, (pos+1)*sizeof(int));
				word_position[pos++] = i;
			}
		}
	}
	word_position = (int *) realloc(word_position, (pos+1)*sizeof(int));
	word_position[pos] = END_WORD_POSITION;

	return word_position;
}

//
//	Funções para categorização
//

// isEndOfString = Define se aquele caractere corresponde ao último da string.
int isEndOfString(char const character) {
	if (character == '\0') {return 0;}
	else {return 1;}
}

// isSpecialCharacter = Define se aquele caractere é uma letra ou um caractere especial (como números e pontuações).
int isSpecialCharacter(char const character) {
	if (character == ' ') {return 0;}
	else if (character == '.') {return 0;}
	else if (character == ',') {return 0;}
	else if (character == '!') {return 0;}
	else if (character == '?') {return 0;}
	else if (character == ';') {return 0;}
	else if (character >= '0' && character <= '9') {return 0;}
	else {return 1;}
}

//
// Funções de manipulação de arrays e matrizes
//

// ArrayToMatrix = Função responsável por converter um array em uma matriz. Foi utilizado o critério de que caracteres
// especiais (pontuações, números, espaço) e letras não coexistirão na mesma linha. Dessa forma, a função separa as palavras
// em linhas diferentes dos caracteres especiais (que serão constantes) das frases.
void ArrayToMatrix (char *string, char **matrix) {
	int pos = 0;
	int i = 0, j = 0;
	int flag, flag2, flag3;
	do {
		flag = isEndOfString(string[pos+1]);
		flag2 = isSpecialCharacter(string[pos]);
		flag3 = isSpecialCharacter(string[pos+1]);
		if (flag == 0) {
			if (flag2 == 0) {
				i++;
				j = 0;
				matrix[i][j] = string[pos];
				return;
			} else {
				matrix[i][j] = string[pos];
				return;
			}
		} else {
			if (flag2 == 0) {
				if (flag3 == 0) {
					i++;
					j = 0;
					matrix[i][j] = string[pos];
				} else {
					i++;
					j = 0;
					matrix[i][j] = string[pos];
					i++;
				}
			} else {
				matrix[i][j] = string[pos];
				j++;
			}
		}
	} while (string[pos++] != '\0');
}

// matrixLineCounter = Função que determinará quantas linhas a matriz deverá ter, levando em consideração os
// elementos da frase. O critério utilizado para a elaboração foi:
// Uma frase, tipicamente, possui (número_palavras) = (número_espaços)+1. Dessa forma, a contagem de espaços terá de ser
// feita separadamente em relação aos demais caracteres especiais. O número de linhas da matriz será determinado
// por "(número_caracteres_especiais) + (número_espaços) + (número_palavras)".
void matrixLineCounter(char *string, int *counter) {
	int pos = 0;
	int spaces = 0;
	do {
		if (string[pos] == ' ') {spaces++;}
		else if (string[pos] == '.') {(*counter)++;}
		else if (string[pos] == ',') {(*counter)++;}
		else if (string[pos] == '!') {(*counter)++;}
		else if (string[pos] == '?') {(*counter)++;}
		else if (string[pos] == ';') {(*counter)++;}
	} while (string[pos++] != '\0');
	(*counter) = (*counter) + 2*spaces + 1;
}

// matrixCreate = Função para criação dinâmica da matriz que armazenará o array. Como o segundo índice
// da matriz representará o número de caracteres de cada linha, por conveniência tomaremos 20 índices.
char **matrixCreate(int const line, int const column) {
	char **matrix = (char **) calloc(line, sizeof(char *));
	for (int i = 0; i < line; i++) {
		matrix[i] = (char *) calloc(column, sizeof(char));
	}

	return matrix;
}

// freeMatrix = Função para liberação da memória ocupada pela matriz criada.
void freeMatrix(char **matrix, int const line) {
	for (int i = 0; i < line; i++) {
		free(matrix[i]);
	}

	free(matrix);
}

// readline = Função para leitura do buffer do teclado. Utilizado para leitura das palavras e da frase.
char *readline(FILE *stream) {
	char *string = NULL;
	int pos = 0;
	do {
		if (pos % READLINE_BUFFER == 0) {
			int q = pos / READLINE_BUFFER + 1;
			string = (char *) realloc(string, q*READLINE_BUFFER);
		}
		string[pos] = fgetc(stream);
	} while(string[pos++] != '\n' && !feof(stream));
	string[pos-1] = '\0';
	string = (char *) realloc(string, pos);

	return string;
}


int main(int argc, char *argv[]) {
	// word = Palavra que será substituida
	// substitute = Nova palavra
	// sentence = Frase en que será aplicada
	char *word = readline(stdin);
	char *substitute = readline(stdin);
	char *sentence = readline(stdin);
	
	// counter = Contador de quantas linhas a matriz deverá ter, com base na função matrixLineCounter
	int counter = 0;
	matrixLineCounter(sentence, &counter);

	char **matrix = matrixCreate(counter, 20);

	ArrayToMatrix(sentence, matrix);

	// word_position = Armazenará as linhas da matriz onde estão presentes a palavra referida em word
	int *word_position = wordFinder(matrix, word, counter);
	substituteWord(matrix, substitute, word_position);

	for (int i = 0; i < counter; i++) {
		printf("%s", matrix[i]);
	}
	printf("\n");

	free(word);
	free(substitute);
	free(sentence);
	free(word_position);
	freeMatrix(matrix, counter);
	return 0;
}