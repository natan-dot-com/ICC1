/*
*   Aluno: Natan Henrique Sanches (11795680)
*   ICMC/USP - Bacharelado em Ciências de Computação
*   Conversor de número binário para número decimal
*/

#include <stdio.h>
#include <math.h>
#include <string.h>

int main (void) {
    // bin[31] = Número binário em formato de caractere (string)
    // tamanho = Tamanho da string
    // decf = Número decimal final
    char bin[31];
    int tamanho;
    int decf = 0;
    
    // Entrada do número binário
    scanf("%s", bin);
    //Determinação do tamanho da string 
    tamanho = strlen(bin);
    // bin2[tamanho] = Conversão dos caracteres da string para números inteiros
    int bin2[tamanho];

    //Conversão para decimal
    for (int i = 0; i < tamanho; i++) {
        // Caractere -> Inteiro
        bin2[i] = bin[tamanho-i-1] - '0';
        // Cálculo do termo com base na potência de 2
        int dec1;
        dec1 = bin2[i]*pow(2,i);
        decf = decf + dec1;
    }

    //Número decimal final
    printf("%d\n", decf);
    return 0;
}