#ifndef CIDADE_H
#define CIDADE_H

#define MAX_NOME 50
#define MAX_VIZINHOS 5
#define X_MIN -50.0
#define X_MAX 50.0
#define Y_MIN -50.0
#define Y_MAX 50.0
#define DISTANCIA_MINIMA_CIDADES 3.0

typedef struct Cidade Cidade;

struct Cidade{
    char *nome;
    Cidade *vizinhos [MAX_VIZINHOS];
    int vizinho_distancia [MAX_VIZINHOS];
    int num_vizinhos;
};

typedef struct cidade_grid CidadeGrid;

// Representa uma cidade no plano xoy [-50, 50] x [-50, 50]
struct cidade_grid {
    char nome[MAX_NOME + 1];
    double x;
    double y;
};

// STRUCT QUE CONTERA O NOME, VIZINHOS, DISTANCIA E NUMERO DOS VIZINHOS DA CIDADE

void conectarCidades(Cidade *origem, Cidade *destino, int distancia);
void exibirConexoes(Cidade *origem);
void posicionar_cidades(CidadeGrid *grid, int tam);

#endif // CIDADE_H
