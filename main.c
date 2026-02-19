#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cidade.h"

void inicializarCidade(CidadeGrid *grid, int qtd) {
    FILE *f = fopen("cidades.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo cidades.txt. Certifique-se que ele existe.\n");
        exit(1);
    }

    for (int i = 0; i < qtd; i++) {
        if (fgets(grid[i].cidade.nome, MAX_NOME + 1, f) == NULL) {
            printf("Erro: Arquivo cidades.txt nao possui cidades suficientes (precisa de %d).\n", qtd);
            fclose(f);
            exit(1);
        }

        int len = strlen(grid[i].cidade.nome);
        if (len > 0 && grid[i].cidade.nome[len-1] == '\n') {
            grid[i].cidade.nome[len-1] = '\0';
        }

        grid[i].cidade.num_vizinhos = 0;
        grid[i].cidade.id = i;
        memset(grid[i].cidade.vizinho_distancia, 0, MAX_VIZINHOS * sizeof(int));
        memset(grid[i].cidade.vizinhos, 0, MAX_VIZINHOS * sizeof(int));
    }
    fclose(f);
}

/* Pede ao usuario a cidade de origem e destino e calcula a menor rota */
int buscar_cidade(CidadeGrid *grid, int qtd, const char *nome) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(grid[i].cidade.nome, nome) == 0)
            return i;
    }
    return -1;
}

void ler_string(char *buffer, int tamanho) {
    fgets(buffer, tamanho, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

void escolher_rota(CidadeGrid *grid, int qtd) {
    char nome[MAX_NOME + 1];
    int id_origem, id_destino;

    // ORIGEM
    do {
        printf("\nDigite o nome da cidade de ORIGEM: ");
        ler_string(nome, sizeof(nome));

        id_origem = buscar_cidade(grid, qtd, nome);

        if (id_origem == -1)
            printf("Cidade nao encontrada. Tente novamente.\n");

    } while (id_origem == -1);

    // DESTINO
    do {
        printf("Digite o nome da cidade de DESTINO: ");
        ler_string(nome, sizeof(nome));

        id_destino = buscar_cidade(grid, qtd, nome);

        if (id_destino == -1)
            printf("Cidade nao encontrada. Tente novamente.\n");
        else if (id_destino == id_origem) {
            printf("Destino deve ser diferente da origem. Tente novamente.\n");
            id_destino = -1;
        }

    } while (id_destino == -1);

    int *caminho = calloc(qtd, sizeof(int));
    caminho[0] = id_origem;

    backtrack(caminho, 0, id_destino, grid, qtd);

    free(caminho);
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

    escolher_rota(cidadeGrid, qtd);

    return 0;
}
