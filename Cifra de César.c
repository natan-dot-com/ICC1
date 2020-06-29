/*
*   Aluno: Natan Henrique Sanches (11795680)
*   ICMC/USP - Bacharelado em Ciências de Computação
*   Conversor para Cifra de César
*/

#include <stdio.h>
#include <string.h>

int main (void) {
    // op = Operador (1. Encrypt / 2. Decrypt)
    // key = Chave para a criptografia
    int op;
    int key;
    scanf("%d", &op);
    scanf("%d", &key);

    // entrada[101] = String de 100 caracteres + '\0' para entrada da frase/palavra
    // tamanho = Tamanho da string de entrada
    char entrada[101];
    int tamanho;
    scanf(" %[^\n]", entrada);
    tamanho = strlen(entrada);
    
    // saida[tamanho+1] = String de (tamanho) caracteres + '\0' para saida da frase/palavra
    char saida[tamanho+1];
    for (int i = 0; i < tamanho; i++) {
        // 1.Encrypt
        if (op == 1) {
            // Para letras minúsculas
            if (entrada[i] >= 'a' && entrada[i] <= 'z') {
                saida[i] = entrada[i] + key;
                // Caso particular em que a soma excede o número de letras do alfabeto
                if (saida[i] > 'z') {
                    saida[i] -= 26;
                }
            }
            // Para letras maiúsculas
            else if (entrada[i] >= 'A' && entrada[i] <= 'Z') {
                saida[i] = entrada[i] + key;
                // Caso particular em que a soma excede o número de letras do alfabeto
                if (saida[i] > 'Z') {
                    saida[i] -= 26;
                }
            }
            // Para símbolos e espaços
            else {
                saida[i] = entrada[i];
            }
        }
        // 2.Decrypt
        if (op == 2) {
            // Para letras minúsculas
            if (entrada[i] >= 'a' && entrada[i] <= 'z') {
                saida[i] = entrada[i] - key;
                // Caso particular em que a soma excede o número de letras do alfabeto
                if (saida[i] < 'a') {
                    saida[i] += 26;
                }
            }
            // Para letras maiúsculas
            else if (entrada[i] >= 'A' && entrada[i] <= 'Z') {
                saida[i] = entrada[i] - key;
                // Caso particular em que a soma excede o número de letras do alfabeto
                if (saida[i] < 'A') {
                    saida[i] += 26;
                }
            }
            // Para símbolos e espaços
            else {
                saida[i] = entrada[i];
            }
        }
    }
    // Inserção do terminador de string na última posição do array
    saida[tamanho] = '\0';
    
    printf("%s\n", saida);
    return 0;
}
