#include <stdio.h>
#include <stdlib.h>
#include "cidade.h"
#include <math.h>
#include <string.h>
#include <float.h>

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
int posicionar_cidades(CidadeGrid *grid, int tam) {
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

    return tam_celula;
}

void conectar_cidade_mais_proxima(CidadeGrid *grid, Cidade *cidade, int tam, int indice) {
    int menor_dist = INT_MAX;
    int mais_proxima = -1;

    for (int i = 0; i < tam; i++) {
        if (&grid[i].cidade == cidade) continue;
        int temp = distancia_euclidiana(grid[i].x, grid[indice].x,
                                        grid[i].y, grid[indice].y);
        if (temp < menor_dist && grid[i].cidade.num_vizinhos < MAX_VIZINHOS) {
            menor_dist = temp;
            mais_proxima = i;
        }
    }
    if (mais_proxima != -1)
        conectarCidades(cidade, &grid[mais_proxima].cidade, menor_dist);
}

void conectarOrfao(CidadeGrid *grid, int tam, Cidade *cidade, int indiceOrfao) {
    conectar_cidade_mais_proxima(grid, cidade, tam, indiceOrfao);
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
            conectarOrfao(grid,tam,&grid[i].cidade,i);
        }
    }
}

void desconectarCidades(Cidade *a, Cidade *b) {
    for (int i = 0; i < a->num_vizinhos; i++) {
        if (a->vizinhos[i]->id == b->id) {
            for (int j = i; j < a->num_vizinhos; j++) {
                a->vizinhos[j] = a->vizinhos[j+1];
                a->vizinho_distancia[j] = a->vizinho_distancia[j+1];
            }
            a->num_vizinhos--;
            break;
        }
    }
    for (int i = 0; i < b->num_vizinhos; i++) {
        if (b->vizinhos[i]->id == a->id) {
            for (int j = i; j < b->num_vizinhos; j++) {
                b->vizinhos[j] = b->vizinhos[j+1];
                b->vizinho_distancia[j] = b->vizinho_distancia[j+1];
            }
            b->num_vizinhos--;
            break;
        }
    }
}

void expulsar_vizinho_mais_distante(Cidade *c) {
    int maior_dist = -1;
    int remover = -1;

    for (int i = 0; i < c->num_vizinhos; i++) {
        if (c->vizinho_distancia[i] > maior_dist) {
            maior_dist = c->vizinho_distancia[i];
            remover = i;
        }
    }

    if (remover != -1) {
        desconectarCidades(c, c->vizinhos[remover]);
    }
}

void garantir_grafo_conexo(CidadeGrid *grid, int tam) {
    int *visitado = (int*) calloc(tam, sizeof(int));
    int *fila = (int*) malloc(tam * sizeof(int));
    int inicio = 0, fim = 0, count = 0;

    fila[fim++] = 0;
    visitado[0] = 1;
    count++;

    while (inicio < fim) {
        int atual_id = fila[inicio++];
        Cidade *c_atual = &grid[atual_id].cidade;
        for (int i = 0; i < c_atual->num_vizinhos; i++) {
            int vizinho_id = c_atual->vizinhos[i]->id;
            if (!visitado[vizinho_id]) {
                visitado[vizinho_id] = 1;
                fila[fim++] = vizinho_id;
                count++;
            }
        }
    }

    if (count < tam) {
        double menor_dist = DBL_MAX;
        int melhor_orfao = -1;
        int melhor_visitado = -1;

        for (int i = 0; i < tam; i++) {
            if (visitado[i]) continue;
            for (int j = 0; j < count; j++) {
                int v_id = fila[j];
                double d = distancia_euclidiana(grid[i].x, grid[v_id].x, grid[i].y, grid[v_id].y);
                if (d < menor_dist) {
                    menor_dist = d;
                    melhor_orfao = i;
                    melhor_visitado = v_id;
                }
            }
        }

        if (melhor_orfao != -1) {
            Cidade *c_orfao = &grid[melhor_orfao].cidade;
            Cidade *c_conect = &grid[melhor_visitado].cidade;

            if (c_orfao->num_vizinhos >= MAX_VIZINHOS)
                expulsar_vizinho_mais_distante(c_orfao);
            if (c_conect->num_vizinhos >= MAX_VIZINHOS)
                expulsar_vizinho_mais_distante(c_conect);

            conectarCidades(c_orfao, c_conect, (int)menor_dist);
        }

        free(visitado);
        free(fila);
        garantir_grafo_conexo(grid, tam);
        return;
    }

    free(visitado);
    free(fila);
}