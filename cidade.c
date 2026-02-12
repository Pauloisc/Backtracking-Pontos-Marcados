#include <stdio.h>
#include "cidade.h"

void conectarCidades (Cidade *origem, Cidade *destino, int distancia){
    int i = origem->num_vizinhos;
    origem->vizinhos[i] = destino;
    origem->vizinho_distancia[i] = distancia;
    origem->num_vizinhos++;
    i = destino->num_vizinhos;
    destino->vizinhos[i] = origem;
    destino->vizinho_distancia[i] = distancia;
    destino->num_vizinhos++;
}
// "i" recebe o valor de num_vizinhos da origem
// Origem colocará o destino como vizinho e a sua distancia no indice indicado

void exibirConexoes (Cidade *origem){
    for (int i = 0; i < origem->num_vizinhos; i++){
        printf("%s -> %s - %d KM\n", origem->nome, origem->vizinhos[i]->nome, origem->vizinho_distancia[i]);
    }
}

// Mostra todas as conexões de uma cidade
