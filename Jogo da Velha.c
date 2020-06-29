/*
*	Aluno: Natan Henrique Sanches (11795680)
*	ICMC/USP - Bacharelado em Ciências da Computação
*	Criação de um jogo da velha em C
*
*	Lógica utilizada para a criação do exercício:
*	Foram criadas quatro funções diferentes para a execução do jogo: a criação da matriz inicial com os números de 1 a 9; a impressão da
*	mesma matriz; a substituição do número pela letra que representa o jogador (X ou O) e a análise das vitórias. Na função main, as funções
*	foram organizadas de maneira linear a prosseguir o jogo e essa ordem será descrita posteriormente.
*/

#include <stdio.h>
#include <stdlib.h>

// printTabela = Função para impressão da tabela do jogo.
void printTabela (char **tabela) {
	for (int i = 0; i < 3; i++) {
		printf("%c   |%c   |%c   |\n", tabela[i][0], tabela[i][1], tabela[i][2]);
		printf("----+----+-----\n");
	}
}

// **inicio = Função para a criação de um ponteiro duplo que originará a matriz em que os dados serão armazenados, com números de 1 a 9.
char **inicio() {
	char **tab = (char **) malloc(3*sizeof(char*));
	for (int i = 0; i < 3; i++) {
		tab[i] = (char *) malloc(3*sizeof(char));
	}
	for (int i = 0, aux = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++, aux++) {
			tab[i][j] = 49+aux;
		}
	}
	return tab;
}

// jogada = Função que executa a jogada, substituindo o número da casa pela letra que representa o jogador da rodada. A
// função também é responsável por notificar o jogador de que a casa escolhida pode estar ocupada.
void jogada(char const jogador, char **tabela) {
	int teste = 0;
	int posicao;
	while (teste == 0) {
		scanf("%d", &posicao);
		if (posicao >= 1 && posicao <= 3) {
			if (tabela[0][posicao-1] == 'X' || tabela[0][posicao-1] == 'O') {
				printf("Jogada inválida! Escolha outra casa.\n");
			} else {
				tabela[0][posicao-1] = jogador;
				teste++;
			}
		} else if (posicao > 3 && posicao <= 6) {
			if (tabela[1][posicao-4] == 'X' || tabela[1][posicao-4] == 'O') {
				printf("Jogada inválida! Escolha outra casa.\n");
			} else {
				tabela[1][posicao-4] = jogador;
				teste++;
			}
		} else if (posicao > 6 && posicao <= 9) {
			if (tabela[2][posicao-7] == 'X' || tabela[2][posicao-7] == 'O') {
				printf("Jogada inválida! Escolha outra casa.\n");
			} else {
				tabela[2][posicao-7] = jogador;
				teste++;
			}
		}
	}
}

// check_vitoria = Função responsável por checar se há alguma combinação de vitória. Se sim, a função retornará 1 (que será usado
// para o teste lógico definir se o jogo acaba ou não). Caso contrário, retorna 0.
int check_vitoria(char **tabela) {
	for (int i = 0; i < 3; i++) {
		if (tabela[i][0] == tabela[i][1] && tabela[i][1] == tabela[i][2]) {
			printf("Fim do jogo! O jogador %c venceu!\n", tabela[i][0]);
			return 1;
		} else if (tabela[0][i] == tabela[1][i] && tabela[1][i] == tabela[2][i]) {
			printf("Fim do jogo! O jogador %c venceu!\n", tabela[0][i]);
			return 1;
		}
	}
	if (tabela[0][0] == tabela[1][1] && tabela[1][1] == tabela[2][2]) {
		printf("Fim do jogo! O jogador %c venceu!\n", tabela[0][0]);
		return 1;
	} else if (tabela[0][2] == tabela[1][1] && tabela[1][1] == tabela[2][0]) {
		printf("Fim do jogo! O jogador %c venceu!\n", tabela[0][2]);
		return 1;
	}
	return 0;
}

// Função para desalocar a matriz tabela da memória ao finalizar a execução
void freeTabela(char **tabela) {
	for (int i = 0; i < 3; i++) {
		free(tabela[i]);
	}
	free(tabela);
}

/*
*	Ordem que o jogo acontece dentro do laço de repetição:
*	- O jogador da rodada é definido pelo resto da divisão entre o número da rodada por 2 (Constante alternância entre 0 e 1).
*	- É feita a substituição do número escolhido pela letra que representa o jogador (X ou O), através da função jogada().
*	- É feita a impressão da tabela para mostrar aos jogadores como ela está pela função printTabela().
*	- É checado se há alguma vitória no tabuleiro depois da rodada. Se sim, a função check_vitoria() retorna 1, interrompendo o laço.
*	- Caso não haja vitória depois de 9 rodadas (rodada = 9), o laço é interrompido, pois o jogo resultou num empate.
*/
int main(void) {
	// Criação da matriz inicial, junto com a impressão do "cabeçalho" do jogo
	char **tabela = inicio();
	printTabela(tabela);
	printf("Jogo da velha!\n");

	// rodada = Número de rodadas percorridas.
	// jogador = Variável que definirá qual jogador terá direito de jogar naquela rodada.
	// check = Variável que executa o teste lógico para saber se há vitórias.
	int rodada;
	char jogador;
	char check = 0;

	// Laço que dá origem às rodadas
	for (rodada = 0; rodada < 9 && check == 0; rodada++) {
		int div = rodada%2;
		if (div == 0) {
			jogador = 'X';
		} else if (div == 1) {
			jogador = 'O';
		}

		printf("Escolha um número para jogar, jogador %c!\n", jogador);
		jogada(jogador, tabela);
		printTabela(tabela);
		check = check_vitoria(tabela);
	}
	if (rodada == 9 && check == 0) {
		printf("Deu velha!\n");
	}
	
	freeTabela(tabela);
	return 0;
}
