#ifndef COMBATE_H
#define COMBATE_H

#include "personagem.h"
#include "jogador.h"
#include "grupoinimigos.h"
#include "inimigo.h"

typedef enum{
    vez_jogador,       //0
    vez_inimigo        //1
} turno_personagem;

typedef struct{
    Jogador jogador;
    grupo_inimigos inimigos;
    turno_personagem vez;
    int num_combates_vencidos;    // tem que chegar à 10 VITÓRIAS
    int num_inimigo;             // inimigo 1 ou 2
    int num_carta;              // qual carta
} Combate;

void inicio_combate(Combate* combate);
void mudar_turno(Combate* combate);
void fim_combate(Combate* combate);
void ataque_jogador(Combate* combate);
void defesa_jogador(Combate* combate);
void dano_gerado(personagem* alvo, int valor_dano);

#endif

