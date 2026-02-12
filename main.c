#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidade.h"

int main()
{
    // INICIALIZO CADA CIDADE
    Cidade a, b, c, d;

    strncpy(a.nome,"Aracaju", MAX_NOME);
    strncpy(b.nome,"Itabaiana", MAX_NOME);
    strncpy(c.nome,"Lagarto", MAX_NOME);
    strncpy(d.nome,"Laranjeiras", MAX_NOME);
    // PONHO O NOME DE CADA CIDADE

    a.num_vizinhos = 0;
    b.num_vizinhos = 0;
    c.num_vizinhos = 0;
    d.num_vizinhos = 0;
    // DEFINO A QNT DE VIZINHOS COMO 0

    conectarCidades(&a, &d, 20);
    // ARACAJU><LARANJEIRAS
    conectarCidades(&b, &a, 55);
    // ITABAIANA><ARACAJU
    conectarCidades(&b, &c, 37);
    // ITABAIANA><LAGARTO
    conectarCidades(&b, &d, 50);
    // ITABAIANA><LARANJEIRAS
    conectarCidades(&d, &c, 70);
    // LARANJEIRAS><LAGARTO
    // CONEXÃO ENTRE AS CIDADES, BIDIRECIONAL

    exibirConexoes(&a);
    exibirConexoes(&b);
    exibirConexoes(&c);
    exibirConexoes(&d);
}
