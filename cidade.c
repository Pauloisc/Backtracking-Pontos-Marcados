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
// Origem colocar� o destino como vizinho e a sua distancia no indice indicado

void exibirConexoes (Cidade *origem){
    for (int i = 0; i < origem->num_vizinhos; i++){
        printf("%s -> %s - %d KM\n", origem->nome, origem->vizinhos[i]->nome, origem->vizinho_distancia[i]);
    }
}

double gerar_ponto_intervalo_inclusivo(double min, double max) {
    return min + (max - min) * ((double) rand() / RAND_MAX);
}

double distancia_euclidiana(double xf, double xi, double yf, double yi) {
    double x_diff = xf - xi;
    double y_diff = yf - yi;

    return sqrt(x_diff * x_diff + y_diff * y_diff);
}

int checar_cidade_muito_proxima(CidadeGrid *grid, int p, int qtd) {
    for (int i = 0; i < qtd; i++) {
        if (distancia_euclidiana(grid[i].x, grid[p].x, grid[i].y, grid[p].y) < DISTANCIA_MINIMA_CIDADES)
            return 1;
    }
    return 0;
}

/**
 * O objetivo aqui � determinar as c�lulas utilizadas para inserir cada cidade.
 * O processo � feito aleatoriamente, mas garantindo que cada c�lula seja
 * adjacente � anterior ou exatamente na mesma c�lula. O interesse s�o as
 * c�lulas e n�o a posi��o.
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
 * A ideia por tr�s � dividr um plano cartesiano xoy, com x e y variando de
 * -50 a 50, em v�rias celulas, de modo a formar uma grid para posicionar as
 * cidades numa mesma c�lula ou em c�lulas adjacentes.
 *
 * Assumimos que quantidade total de cidades (tam) � um quadrado composto por
 * v�rios quadrados (que seriam as cidade individuais). A ideia � descobrir
 * a quantidade de quadrados que formam o lado do quadrado maior para poder
 * descobrir o tamanho de seu lado para encaix�-los nas c�lulas do plano.
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
        do {
            grid[i].x = gerar_ponto_intervalo_inclusivo(x_lim_1, x_lim_2);
            grid[i].y = gerar_ponto_intervalo_inclusivo(y_lim_1, y_lim_2);
        } while (checar_cidade_muito_proxima(grid, i, i));

    }
}

void conectarOrfao(CidadeGrid *grid, int tam, Cidade cidade, int indiceOrfao) {
    int mais_proxima = -1;
    double menor_distancia = INT_MAX;
    for (int i = 0; i < tam; i++) {
        if (i == indiceOrfao) continue;
        if (grid[i].cidade.num_vizinhos >= MAX_VIZINHOS) continue;
        double dist = distancia_euclidiana(grid[indiceOrfao].x, grid[i].x, grid[indiceOrfao].y, grid[i].y);
        if (dist < menor_distancia) {
            menor_distancia = dist;
            mais_proxima = i;
        }
    }
    if (mais_proxima != -1) {
        conectarCidades(&cidade, &grid[mais_proxima].cidade, (int)menor_distancia);
    }
}

int orfao(Cidade *cidade) {
    return cidade->num_vizinhos == 0;
}

void conectarGrafoRaio(CidadeGrid *grid, int tam, double raio) {
    for (int i = 0; i < tam; i++) {
        for (int j = i + 1; j < tam; j++) {
            double dist = distancia_euclidiana(grid[i].x, grid[j].x, grid[i].y, grid[j].y);

            if (dist <= raio) {
                if (grid[i].cidade.num_vizinhos < MAX_VIZINHOS &&
                    grid[j].cidade.num_vizinhos < MAX_VIZINHOS) {
                    conectarCidades(&grid[i].cidade, &grid[j].cidade, (int)dist);
                } else {
                    printf("max vizinhos\n");
                }
            }
        }
        if(orfao(&grid[i].cidade)){
            conectarOrfao(grid,tam,grid[i].cidade,i);
        }
    }
}

// Mostra todas as conex�es de uma cidade
