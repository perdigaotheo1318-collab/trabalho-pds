#ifndef JOGADOR_H
#define JOGADOR_H
#include "personagem.h"
#include "baralho.h"

typedef struct{
    personagem dados_base;
    Baralho baralho_inicial;
    Baralho pilha_compra;
    Baralho pilha_descarte;
    Baralho cartas_jogador;
    int energia_turno;

}Jogador;
   
void inicio_jogador(Jogador* jogador);
void inicio_turno(Jogador* jogador);
void comprar_carta(Jogador* jogador, int quantidade);
void fim_turno(Jogador* jogador);

#endif
   

