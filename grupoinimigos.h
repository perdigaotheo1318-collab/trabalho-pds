#ifndef GRUPOINIMIGOS_H
#define GRUPOINIMIGOS_H
#include "personagem.h"
#include "baralho.h"
#include "inimigo.h"

typedef struct{
    inimigo inimigos[2];
    int inimigos_vivos;
}grupo_inimigos;

void inimigos_iniciais(grupo_inimigos* grupo);  // sorteia e inicializa os 2 inimigos no inicio do combate
void turno_inimigos(grupo_inimigos* grupo, Jogador* jogador);

#endif