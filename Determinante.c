/*
*	Aluno: Natan Henrique Sanches (11795680)
*	ICMC/USP - Bacharelado em Ciências de Computação
*	Cálculo de determinantes de matrizes de ordem 1, 2 e 3
*
*	Lógica utilizada para resolução do exercício:
*	Foram criadas funções para o cálculo do determinante de cada tipo de matriz. Recebe-se de entrada as linhas e colunas, respectivamente,
*	da matriz, com a condição de que as matrizes devem ser qde ordem 1, 2 e 3. Em seguida, recebe-se os coeficientes da matriz e logo após é
*	efetuado o cálculo, dado como saída.
*/

#include <stdio.h>
#include <stdlib.h>

// Função det2 para o cálculo do determinante de matrizes 2x2
int det2 (int **matriz) {
	int det = (matriz[0][0]*matriz[1][1])-(matriz[1][0]*matriz[0][1]);
	return det;
}

// Função det3 para o cálculo do determinante de matrizes 3x3
int det3 (int **matriz) {
	int det = ((matriz[0][0]*matriz[1][1]*matriz[2][2]+matriz[0][1]*matriz[1][2]*matriz[2][0]+matriz[1][0]*matriz[2][1]*matriz[0][2])
	- (matriz[0][2]*matriz[1][1]*matriz[2][0]+matriz[1][2]*matriz[2][1]*matriz[0][0]+matriz[0][1]*matriz[1][0]*matriz[2][2]));
	return det;
}

void freeMatrix(int **matrix, int n) {
	for (int i = 0; i < n; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

int main (void) {
	int n1, n2;
	scanf("%d", &n1);
	scanf("%d", &n2);

	// Criação da matriz
	int **matriz = (int **) malloc(n1*sizeof(int));
	for (int i = 0; i < n1; i++) {
		matriz[i] = (int *) malloc(n2*sizeof(int));
	}

	// Condições que invalidam os cálculos
	if (n1 < 1 || n1 > 3 || n2 < 1 || n2 > 3) {
		printf("Entradas invalidas!\n");
	} else if (n1 != n2) {
		printf("A matriz de entrada nao possui determinante!\n");
	} else {
	// Cálculo do determinante
		if (n1 == 1) {
			scanf("%d", &matriz[0][0]);
			printf("O determinante da matriz de entrada equivale a %d e tem ordem %d\n", matriz[0][0], n1);
			freeMatrix(matriz, n1);
		} 
		else if (n1 == 2) {
			for (int i = 0; i < n1; i++) {
				scanf("%d %d", &matriz[i][0], &matriz[i][1]);
			}
			int result = det2(matriz);
			printf("O determinante da matriz de entrada equivale a %d e tem ordem %d\n", result, n1);
			freeMatrix(matriz, n1);
		} 
		else if (n1 == 3) {
			for (int i = 0; i < n1; i++) {
				scanf("%d %d %d", &matriz[i][0], &matriz[i][1], &matriz[i][2]);
			}
			int result = det3(matriz);
			printf("O determinante da matriz de entrada equivale a %d e tem ordem %d\n", result, n1);
			freeMatrix(matriz, n1);
		}
	}

	return 0;
}
