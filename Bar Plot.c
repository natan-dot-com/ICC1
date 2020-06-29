/*
*   Aluno: Natan Henrique Sanches (11795680)
*   ICMC/USP - Bacharelado em Ciências de Computação
*   Bar Plot
*
*	Lógica utilizada para resolução do exercício:
*	Após a coleta de todos os dados, foi feito um laço para a contabilização de cada dado informado, seguido da impressão de # de
*	acordo com a contagem.
*/

#include <stdio.h>
#include <stdlib.h>

int main (void) {
	// barras = Número de barras que serão feitas
	int barras;
	scanf("%d", &barras);
	// dados = Número de quantos tipos diferentes de dados há
	int dados;
	scanf("%d", &dados);
	// *numeros = Array para a coleta dos dados
	int *numeros = (int *) malloc(dados*sizeof(int));

	for (int i = 0; i < dados; i++) {
		scanf("%d", &numeros[i]);
	}

	// Impressão de cada barra
	for (int i = 0, aux = 0; i < barras; i++, aux = 0) {
		// aux = Variável auxiliar que efetuará a contagem de cada tipo de dado
		// Contagem de dados
		for (int j = 0; j < dados; j++) {
			if (numeros[j] == i) {
				aux++;
			}
		}
		// Impressão
		printf("%d - ", i);
		for (int k = 0; k < aux; k++) {
			printf("#");
		}
		printf("\n");
	}

	free(numeros);
	return 0;
}
