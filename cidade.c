#include <stdio.h>
#include <stdlib.h>
#include "cidade.h"
#include <math.h>

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

double gerar_ponto_intervalo_inclusivo(double min, double max) {
    return min + (max - min) * ((double) rand() / RAND_MAX);
}

/**
 * O objetivo aqui é determinar as células utilizadas para inserir cada cidade.
 * O processo é feito aleatoriamente, mas garantindo que cada célula seja
 * adjacente à anterior ou exatamente na mesma célula. O interesse são as
 * células e não a posição.
 */
void definir_celulas(int *celulas_grid, int tam, int quadrados) {
    int total_celulas = quadrados * quadrados;
    int num_aleatorio, ant = -1, lin, col;

    for (int i = 0; i < tam; i++) {
        num_aleatorio = rand() % total_celulas;
        lin = num_aleatorio / quadrados;
        col = num_aleatorio - lin * quadrados;
        if (ant != -1) {
            int lin_ant = celulas_grid[ant] / quadrados;
            int col_ant = celulas_grid[ant] - lin_ant * quadrados;
            while (abs(lin_ant - lin) > 1 || abs(col_ant - col) > 1) {
                num_aleatorio = rand() % total_celulas;
                lin = num_aleatorio / quadrados;
                col = num_aleatorio - lin * quadrados;
            }
        }
        celulas_grid[i] = num_aleatorio;
        ant = i;
    }
}

/**
 * A ideia por trás é dividr um plano cartesiano xoy, com x e y variando de
 * -50 a 50, em várias celulas, de modo a formar uma grid para posicionar as
 * cidades numa mesma célula ou em células adjacentes.
 *
 * Assumimos que quantidade total de cidades (tam) é um quadrado composto por
 * vários quadrados (que seriam as cidade individuais). A ideia é descobrir
 * a quantidade de quadrados que formam o lado do quadrado maior para poder
 * descobrir o tamanho de seu lado para encaixá-los nas células do plano.
 */
void posicionar_cidades(CidadeGrid *grid, int tam) {
    int quadrados = ceil(sqrt(tam));
    double tam_celula = (X_MAX - X_MIN) / quadrados;
    int celulas_grid[tam];

    definir_celulas(celulas_grid, tam, quadrados);

    for (int i = 0; i < tam; i++) {
        int lin = celulas_grid[i] / quadrados;
        int col = celulas_grid[i] - lin * quadrados;
        double x_lim_1 = X_MAX - col * tam_celula;
        double x_lim_2 = x_lim_1 - tam_celula;
        double y_lim_1 = Y_MAX - lin * tam_celula;
        double y_lim_2 = y_lim_1 - tam_celula;
        grid[i].x = gerar_ponto_intervalo_inclusivo(x_lim_1, x_lim_2);
        grid[i].y = gerar_ponto_intervalo_inclusivo(y_lim_1, y_lim_2);
    }
}

// Mostra todas as conexões de uma cidade
