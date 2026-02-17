#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cidade.h"

void inicializarCidade(CidadeGrid *grid, int qtd) {
    char nome[MAX_NOME+1];
    for (int i = 0; i < qtd; i++) {
        printf("Insira o nome da cidade %d: ", i+1);
        fgets(grid[i].cidade.nome, MAX_NOME + 1, stdin);
        int len = strlen(grid[i].cidade.nome);
        grid[i].cidade.nome[len-1] = '\0';
        grid[i].cidade.num_vizinhos = 0;
        grid[i].cidade.id = i;
        memset(grid[i].cidade.vizinho_distancia, 0, MAX_VIZINHOS * sizeof(int));
        memset(grid[i].cidade.vizinhos, 0, MAX_VIZINHOS * sizeof(int));
    }
}

int main()
{
    srand(time(NULL));
    int qtd, tam_celula;

    do {
        printf("Insira a quantidade de cidades (mininmo 9 e maximo 32): ");
        scanf("%d", &qtd);
    } while (qtd > 32 || qtd < 9);
    getchar();
    CidadeGrid cidadeGrid[qtd];
    
    inicializarCidade(cidadeGrid, qtd);
    tam_celula = posicionar_cidades(cidadeGrid, qtd);
    conectarGrafoRaio(cidadeGrid, qtd, 0.8 * tam_celula);
    garantir_grafo_conexo(cidadeGrid, qtd);

    for (int i = 0; i < qtd; i++)
        exibirConexoes(&cidadeGrid[i].cidade);

    return 0;
}
