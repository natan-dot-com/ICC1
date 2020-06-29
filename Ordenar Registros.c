/*
*	Aluno: Natan Henrique Sanches (11795680)
*	ICMC/USP - Bacharelado em Ciências da Computação
*	Ordenação de Registros
*
*	Lógica utilizada para resolução do exercício:
*	Os dados de entrada foram armazenados primeiramente em um array único do tipo char (chama-se de "string de base" ao longo dos comentários)
* 	e logo após, duas funções foram responsáveis por separar o "nome" do número inteiro. Esses dados foram armazenados numa struct, devidamente 
*	separados. Para a ordenação, foi utilizada a função qsort.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Buffer da função readline.
#define READLINE_BUFFER 4096

typedef struct {
	char *name;
	int integer;
} info;

// Função responsável por ler a string com espaços e alocar dinamicamente o espaço para armazená-la.
char *readline(FILE *stream) {
	char *string = NULL;
	int pos = 0;
	scanf(" ");
	do {
		if (pos % READLINE_BUFFER == 0) {
			int q = pos/READLINE_BUFFER + 1;
			string = (char *) realloc(string, q*READLINE_BUFFER);
		}
		string[pos] = fgetc(stream);
	} while (string[pos++] != '\n' && !feof(stdin));
	string[pos-1] = '\0';
	string = (char *) realloc(string, pos);

	return string;
}

// Função responsável por inverter a ordem de um array de caracteres. Foi utilizado na função findDigit, cujo array é lido
// de trás para frente.
char *reverseString(const char *string) {
	char *n_string = (char *) malloc((strlen(string)+1)*sizeof(char));
	int pos;
	for (pos = 0; string[pos] != '\0'; pos++) {
		n_string[pos] = string[strlen(string)-1-pos];
	}
	n_string[strlen(string)] = '\0';

	return n_string;
}

// Função responsável por encontrar o dígito no final da string de base, onde o nome e o dígito estão misturados. Para tal, a
// função lê a string de base de trás para frente, armazenando todos os dígitos encontrados no caminho (incluindo o sinal de negativo),
// até encontrar um caractere. Feito isso, o array em que os dígitos foram armazenados é invertido (pois foi lido de trás pra frente)
// e transformado em inteiro.
int findDigit(const char *string) {
	char *number = NULL;
	int pos = strlen(string)-1;
	int counter = 0;
	do {
		if (isdigit(string[pos]) || string[pos] == '-') {
			number = (char *) realloc(number, (counter+2)*sizeof(char));
			number[counter] = string[pos];
			counter++;
		}
	} while(!isalpha(string[pos--]));
	number[counter] = '\0';

	char *num_string = reverseString(number);
	free(number);

	int num = atoi(num_string);
	free(num_string);

	return num;
}

// Função responsável por encontrar o "nome" na string de base. Para tal, a função lê a string da maneira convencional, armazenando
// todos os caracteres (incluindo espaços) até encontrar um dígito.
char *findString(const char *string) {
	char *new_string = NULL;
	int pos = 0;
	int counter = 0;
	do {
		new_string = (char *) realloc(new_string, (counter+2)*sizeof(char));
		new_string[counter] = string[pos];
		counter++;
	} while(!isdigit(string[++pos]));
	
	// Condição necessária para que o sinal dos números negativos não seja armazenado junto com o "nome".
	if(string[pos-1] == '-') {
		new_string[counter-2] = '\0';
	} else {
		new_string[counter-1] = '\0';
	}

	return new_string;
}

// Função responsável por "transformar" a string de base numa struct com o nome e o número devidamente separados.
void createStructure(info ***pointer, int *counter) {	
	do {
		char *base_string = readline(stdin);
		int int_part = findDigit(base_string);
		char *string_part = findString(base_string);

		*pointer = (info **) realloc(*pointer, ((*counter)+1)*sizeof(info *));
		(*pointer)[*counter] = (info *) malloc(sizeof(info));
		(*pointer)[*counter]->name = (char *) malloc((strlen(string_part)+1)*sizeof(char));
		
		strcpy((*pointer)[*counter]->name, string_part);
		(*pointer)[*counter]->integer = int_part;

		*counter += 1;	
		free(base_string);
		free(string_part);
	} while(!feof(stdin));
}

// Função responsável por imprimir os dados da estrutura formatados.
void printStructure(info **pointer, int const counter) {
	for (int i = 0; i < counter; i++) {
		printf("%d\t%s\n", pointer[i]->integer, pointer[i]->name);
	}
}

// Função responsável por liberar a memória alocada da estrutura.
void freeStructure(info **pointer, int const counter) {
	for (int i = 0; i < counter; i++) {
		free(pointer[i]->name);
		free(pointer[i]);
	}
	free(pointer);
}

// Função responsável pela ordenação a partir dos números em ordem crescente. Utilizada na função qsort.
int numberCompare(const void *a, const void *b) {
	info *struct1 = *(info**) a;
	info *struct2 = *(info**) b;

	if (struct1->integer == struct2->integer)  {return 0;}
	else if (struct1->integer < struct2->integer) {return -1;}
	else {return 1;}
}

// Função responsável pela ordenação a partir dos nomes em ordem alfabética. Utilizada na função qsort.
int stringCompare(const void *a, const void *b) {
	info *struct1 = *(info**) a;
	info *struct2 = *(info**) b;
	return strcmp(struct1->name, struct2->name);
}

int main(int argc, char *argv[]) {
	int operator;
	scanf("%d", &operator);

	int counter = 0;
	info **pointer = NULL;
	createStructure(&pointer, &counter);

	switch(operator) {
		case 1: {
			qsort(pointer, counter, sizeof(info *), stringCompare);
			break;
		}
		case 2: {
			qsort(pointer, counter, sizeof(info *), numberCompare);
			break;
		}	
	}

	printStructure(pointer, counter);
	freeStructure(pointer, counter);
	return 0;
}
