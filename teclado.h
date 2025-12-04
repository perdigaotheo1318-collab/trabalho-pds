#ifndef TECLADO_H
#define TECLADO_H

#include "combate.h"

/*
 Processa todas as ações do jogador com base no estado atual do teclado.
 keyboard_keys é o array usado no main (tamanho ALLEGRO_KEY_MAX) onde as
 teclas pressionadas tem GAME_KEY_SEEN | GAME_KEY_DOWN.
*/
void Processa_Input_e_Acao(Combate* combate, unsigned char keyboard_keys[]);

#endif
