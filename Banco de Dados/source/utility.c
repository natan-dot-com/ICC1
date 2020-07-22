#include <stdio.h>
#include <stdlib.h>
#include <databank.h>

// Função de ordenação numérica, utilizada no qsort.
int numberSort(const void *n1, const void *n2) {
    if ((*(Idx_File *)n1).key == (*(Idx_File *)n2).key) {return 0;}
    else if ((*(Idx_File *)n1).key > (*(Idx_File *)n2).key) {return 1;}
    else {return -1;}
}

// Função que realiza a busca binária
int binarySearch(Idx_File *search, const int search_counter, const int searched_key, int *error) {
    int min = 0;
    int max = search_counter-1;
    while (min <= max) {
        int guess = (min+max)/2;
        if (search[guess].key == searched_key) {return search[guess].offset;}
        else if (search[guess].key < searched_key) {min = guess+1;}
        else if (search[guess].key > searched_key) {max = guess-1;}
    }
	(*error) = KEY_NOT_FOUND;
	return 0;
}

char *readline(FILE *stream) {
    char *string = NULL;
    char ch;
    int pos = 0;
    do {
        ch = (char) fgetc(stream);
        if (ch != '\r') {
            string = (char *) realloc(string, (pos+1)*sizeof(char));
            string[pos] = ch;
        }
    } while(string[pos++] != '\n' && !feof(stream));
    string[pos-1] = '\0';

    return string;
}