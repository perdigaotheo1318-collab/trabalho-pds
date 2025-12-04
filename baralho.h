#ifndef BARALHO_H
#define BARALHO_H

typedef enum {
    ATAQUE,  //0
    DEFESA,
    ESPECIAL  //2

}tipo_carta;        //cartas de ataque, cartas de defesa, cartas especiais

typedef struct{      // caracteristicas das cartas
    int custoE;
    int efeitoX;
    tipo_carta tipo;

}carta;


typedef struct{
    carta cartas[20]; // aloc
    int num_cartas;
} Baralho;

void preencher_baralho_inicial(Baralho *baralho);
void embaralhar_baralho(Baralho *baralho);
void mover_carta(Baralho* origem, Baralho* destino);
#endif
