#ifndef CIDADE_H
#define CIDADE_H

#define MAX_NOME 50
#define MAX_VIZINHOS 5

typedef struct Cidade Cidade;

struct Cidade{
    char nome [MAX_NOME];
    Cidade vizinhos [MAX_VIZINHOS];
    int vizinho_distancia [MAX_VIZINHOS];
    int num_vizinhos;
};

#endif // CIDADE_H
