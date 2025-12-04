#include <stdlib.h> 
#include <time.h>   
#include "baralho.h"
#include "utils.h"

void embaralhar_baralho(Baralho *baralho) {
    
    if (baralho->num_cartas <= 1) 
    return;

    for (int i = baralho->num_cartas - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        carta temp = baralho->cartas[i];
        baralho->cartas[i] = baralho->cartas[j];
        baralho->cartas[j] = temp;
    }
}

void setar_stats_aleatorios(carta *c, int custo) {
    c->custoE = custo;
    if (custo == 0) {
        c->efeitoX = random_int_in_range(1, 5);
    } else if (custo == 1) {
        c->efeitoX = random_int_in_range(5, 10);
    } else if (custo == 2) {
        c->efeitoX = random_int_in_range(10, 15);
    } else {
        c->efeitoX = random_int_in_range(15, 30);
    }
}

void preencher_baralho_inicial(Baralho *baralho) {
    int i = 0;
    int custo_aleatorio;

// 10 cartas de ataque, min 6 fixas
baralho->cartas[i].tipo = ATAQUE; setar_stats_aleatorios(&baralho->cartas[i++], 0);
baralho->cartas[i].tipo = ATAQUE; setar_stats_aleatorios(&baralho->cartas[i++], 1);
baralho->cartas[i].tipo = ATAQUE; setar_stats_aleatorios(&baralho->cartas[i++], 1);
baralho->cartas[i].tipo = ATAQUE; setar_stats_aleatorios(&baralho->cartas[i++], 1);
baralho->cartas[i].tipo = ATAQUE; setar_stats_aleatorios(&baralho->cartas[i++], 2);
baralho->cartas[i].tipo = ATAQUE; setar_stats_aleatorios(&baralho->cartas[i++], 3);

// restante aleatório 4 cartas
for (int j = 0; j < 4; j++) {
    custo_aleatorio = random_int_in_range(0, 3);
    baralho->cartas[i].tipo = ATAQUE;
    setar_stats_aleatorios(&baralho->cartas[i], custo_aleatorio);
    i++;
}

// 8 cartas de defesa min 6 fixas
baralho->cartas[i].tipo = DEFESA; setar_stats_aleatorios(&baralho->cartas[i++], 0);
baralho->cartas[i].tipo = DEFESA; setar_stats_aleatorios(&baralho->cartas[i++], 1);
baralho->cartas[i].tipo = DEFESA; setar_stats_aleatorios(&baralho->cartas[i++], 1);
baralho->cartas[i].tipo = DEFESA; setar_stats_aleatorios(&baralho->cartas[i++], 1);
baralho->cartas[i].tipo = DEFESA; setar_stats_aleatorios(&baralho->cartas[i++], 2);
baralho->cartas[i].tipo = DEFESA; setar_stats_aleatorios(&baralho->cartas[i++], 3);

// restante aleatório 2 cartas
for (int j = 0; j < 2; j++) {
    custo_aleatorio = random_int_in_range(0, 2);
    baralho->cartas[i].tipo = DEFESA;
    setar_stats_aleatorios(&baralho->cartas[i], custo_aleatorio);
    i++;
}


    // CARTAS ESPECIAIS 2
    for (int j = 0; j < 2; j++) {
        baralho->cartas[i].tipo = ESPECIAL;
        baralho->cartas[i].custoE = 0;
        baralho->cartas[i].efeitoX = 0;
        i++;
    }
    
    baralho->num_cartas = 20;
}

void mover_carta(Baralho* origem, Baralho* destino){
    if(origem->num_cartas == 0){
        return;
    }
    carta carta_movida = origem->cartas[origem->num_cartas - 1]; // pega carta do topo e coloca em destino
    destino->cartas[destino->num_cartas] = carta_movida;
    destino->num_cartas++;
}