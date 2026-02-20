#ifndef CIDADE_H
#define CIDADE_H

#define MAX_NOME 50
#define MAX_VIZINHOS 10
#define X_MIN -50.0
#define X_MAX 50.0
#define Y_MIN -50.0
#define Y_MAX 50.0
#define DISTANCIA_MINIMA_CIDADES 3.0

typedef struct Cidade Cidade;

struct Cidade{
    int id;
    char nome[MAX_NOME + 1];
    Cidade *vizinhos [MAX_VIZINHOS];
    int vizinho_distancia [MAX_VIZINHOS];
    int num_vizinhos;
};

typedef struct cidade_grid CidadeGrid;

// Representa uma cidade no plano xoy [-50, 50] x [-50, 50]
struct cidade_grid {
    Cidade cidade;
    double x;
    double y;
};

extern int melhor_caminho[32];
extern int melhor_tam;
extern int melhor_dist;
extern int dist_acum[32];

// STRUCT QUE CONTERA O NOME, VIZINHOS, DISTANCIA E NUMERO DOS VIZINHOS DA CIDADE

void conectarCidades(Cidade *origem, Cidade *destino, int distancia);
void exibirConexoes(Cidade *origem);
void conectarGrafoRaio(CidadeGrid *grid, int tam, double raio);
void garantir_grafo_conexo(CidadeGrid *grid, int tam);
int posicionar_cidades(CidadeGrid *grid, int tam);
void backtrack(int *a, int k, int id_destino, CidadeGrid *grid, int total_cidade);

#endif // CIDADE_H
