/*
*	Aluno: Natan Henrique Sanches (11795680)
*	ICMC/USP - Bacharelado em Ciências da Computação
*	Sistema de gerenciamento de uma biblioteca
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Buffer da função readline.
#define READLINE_BUFFER 4096

// Macros para manipulação do aluguel dos livros.
#define RENTED 1
#define NOT_RENTED 0

// Estrutura para categorização dos livros, contendo seu nome (*name, quantos dias poderá ser alugado (rent_days), seu status de aluguel
// (rent_status) e sua posição em que foi alugado (rent_position).
typedef struct {
	char *name;
	int rent_days;
	int rent_status;
	int rent_position;
} infoBooks;

// Função para leitura de strings longas e armazenamento com alocação dinâmica.
char *readline(FILE *stream) {
	char *string = NULL;
	int pos = 0;
	int q = pos/READLINE_BUFFER + 1;
	
	scanf(" ");
	do {
		if (pos % READLINE_BUFFER == 0) {
			string = (char *) realloc(string, q*READLINE_BUFFER);
		}
		string[pos] = (char) fgetc(stream);
	} while (string[pos++] != '\n' && !feof(stream));
	
	string[pos-1] = '\0';
	if (pos >= 2 && string[pos-2] == '\r') {
		string[pos-2] = '\0';
	}

	string = (char *) realloc(string, pos);
	return string;
}

// Função que aloca dinamicamente as estruturas necessárias para armazenar os livros.
infoBooks **libraryCreate(const int n_lib_books) {
	infoBooks **books = (infoBooks **) malloc(n_lib_books*sizeof(infoBooks *));
	for (int i = 0; i < n_lib_books; i++) { 
		books[i] = (infoBooks *) malloc(sizeof(infoBooks));
	}

	return books;
}

// Função para leitura das informações de cada livro. O nome e os dias de aluguel serão dados de entrada, enquanto o restante será
// padronizado inicialmente: Todos os livros iniciarão na posição de aluguel 0 e com status de NOT_RENTED.
infoBooks **libraryRead(infoBooks **books, const int n_lib_books) {
	for (int i = 0; i < n_lib_books; i++) {
		char *temp = readline(stdin);
		books[i]->name = (char *) malloc((strlen(temp)+1)*sizeof(char));
		strcpy(books[i]->name, temp);
		free(temp);

		scanf("%d", &books[i]->rent_days);
		books[i]->rent_status = NOT_RENTED;
		books[i]->rent_position = 0;
	}

	return books;
}

// Função que atualiza a ordem dos livros alugados após a devolução de algum deles (função necessária para preservar a organização).
void BooksRentRefresh(infoBooks **books, const int rented_books, const int n_lib_books, const int temp) {
	for (int i = temp+1; i <= rented_books; i++) {
		for (int j = 0; j < n_lib_books; j++) {
			if (books[j]->rent_position == i) {
				books[j]->rent_position -= 1;
			}
		}
	}
	return;
}

// Função que executa a operação 1: Alugar um livro.
void operation1(infoBooks **books, int *rented_books, const int n_lib_books) {
	printf("Digite o livro que voce procura:\n");
	// *search: Recebe o nome do livro que o usuário procura.
	char *search = readline(stdin);
	// flag: Variável temporária que servirá na procura do livro solicitado.
	int flag;
	for (int i = 0; i < n_lib_books; i++) {
		flag = strcmp(search, books[i]->name);
		if (flag == 0) {
			// Se o usuário já alugou 10 livros.
			if (*(rented_books) == 10) {
				printf("Voce ja tem 10 livros alugados\n");
				free(search);
				return;
			}
			// Se o livro solicitado já estiver alugado.
			if (books[i]->rent_status == RENTED) {
				printf("Livro ja alugado\n");
				free(search);
				return;
			}
			// Caso o livro possa ser alugado:
			// O status do livro é alterado para RENTED;
			printf("%s alugado com sucesso\n", books[i]->name);
			books[i]->rent_status = RENTED;

			// O número de livros alugados aumenta em 1 e a posição de aluguel desse livro será relativa ao número de livros alugados.
			// Dessa maneira, os livros poderão ser ordenados na ordem em que foram alugados.
			*(rented_books) += 1;
			books[i]->rent_position = *(rented_books);
			free(search);
			return;
		}
	}
	// Se não for acessada nenhuma das condições, o livro solicitado não existe.
	printf("Livro nao encontrado na biblioteca\n");
	free(search);
	return;
}

// Função que executa a operação 2: Exibe os livros alugados pelo usuário, na ordem em que foram alugados.
void operation2(infoBooks **books, const int rented_books, const int n_lib_books) {
	if (rented_books == 0) {
		printf("Voce nao tem livros alugados\n");
		return;
	} else {
		printf("Voce tem %d livro(s) alugado(s)\n", rented_books);
		// A procura dos livros da operação 2 é feita com base no valor de rent_position. Como é relativo a quantidade de livros alugados,
		// isso preserva a ordem em que os livros foram alugados e se possibilita de ser feita a procura com organização.
		for (int i = 1; i <= rented_books; i++) {
			for(int j = 0; j < n_lib_books; j++) {
				if (books[j]->rent_position == i) {
					printf("Livro nome: %s\n", books[j]->name);
					printf("Devolve-lo daqui %d dias\n", books[j]->rent_days);
				}
			}
		}
		return;
	}
}

// Função que executa a operação 3: Devolução de um livro alugado
void operation3(infoBooks **books, int *rented_books, const int n_lib_books) {
	printf("Digite o livro que deseja devolver:\n");
	// *search = Recebe o nome do livro a ser procurado.
	char *search = readline(stdin);
	// flag = Variável temporária que servirá na procura do livro solicitado.
	int flag;
	for (int i = 0; i < n_lib_books; i++) {
		flag = strcmp(search, books[i]->name);
		if (flag == 0) {
			// Se o livro solicitado para devolução não estiver alugado pelo usuário.
			if (books[i]->rent_status == NOT_RENTED) {
				printf("Voce nao possui esse livro\n");
				free(search);
				return;
			}
			// Caso o livro solicitado esteja alugado:
			// O status do livro é alterado para NOT_RENTED;
			printf("Livro %s foi devolvido com sucesso\n", books[i]->name);
			books[i]->rent_status = NOT_RENTED;
			
			// As posições de alugel de todos os livros alugaos são reajustadas de acordo com a devolução do livro em questão;
			int temp = books[i]->rent_position;
			books[i]->rent_position = 0;
			BooksRentRefresh(books, *(rented_books), n_lib_books, temp);
			
			// O número de livros alugados diminui em 1 e o programa finaliza.
			*(rented_books) -= 1;
			free(search);
			return;
		}
	}
	printf("Voce nao possui esse livro\n");
	free(search);
	return;
}

// Função para desalocar todos os dados relacionados a struct infoBooks.
void freeStruct(infoBooks **books, int n_lib_books) {
	for (int i = 0; i < n_lib_books; i++) {
		free(books[i]->name);
		free(books[i]);
	}
	free(books);
}

int main(int argc, char *argv[]) {
	// n_lib_books: Número de livros que serão colocados na biblioteca.
	int n_lib_books;
	scanf("%d", &n_lib_books);

	// Criação e leitura das estruturas que caracterização os livros.
	infoBooks **books = libraryCreate(n_lib_books);
	books = libraryRead(books, n_lib_books);

	// rented_books: Representa o número de livros que o usuário tem alugado. Seu valor é alterado dentro das operações 1 e 3.
	int rented_books = 0;
	// operation: Define a operação a ser feita pelo usuário.
	int operation;
	do {
		scanf("%d", &operation);
		switch (operation) {
			case 1:
				operation1(books, &rented_books, n_lib_books);
				break;
			case 2:
				operation2(books, rented_books, n_lib_books);
				break;
			case 3:
				operation3(books, &rented_books, n_lib_books);
				break;

		}
	} while(operation != 4);
	printf("Programa finalizado\n");

	freeStruct(books, n_lib_books);
	return 0;
}
