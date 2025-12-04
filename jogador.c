#include <stdio.h>
#include "jogador.h"
#include "personagem.h"
#include "baralho.h"
#include "utils.h"
#include <stdlib.h>

void inicio_jogador(Jogador* jogador){  // inicializar jogador
   jogador->dados_base.vida_maxima = 100;
   jogador->dados_base.vida_atual = 100;
   jogador->dados_base.escudo = 0;
   jogador->energia_turno = 3;

   preencher_baralho_inicial(&jogador->baralho_inicial); // preenche o baralho com 10 ataques 8 defesas 2 especiais

   jogador->pilha_compra.num_cartas = jogador->baralho_inicial.num_cartas;
    for (int i = 0; i < jogador->baralho_inicial.num_cartas; i++) {
       jogador->pilha_compra.cartas[i] = jogador->baralho_inicial.cartas[i];
   } 

   embaralhar_baralho(&jogador->pilha_compra);  // embaralha a pilha de compra

   jogador->pilha_descarte.num_cartas = 0;
   jogador->cartas_jogador.num_cartas = 0;
}

void comprar_carta(Jogador* jogador, int quantidade){ // Caso não haja cartas suficientes na pilha de compras, o jogador deve comprar toda a pilha e, em seguida, embaralhar sua pilha de descarte e formar uma nova pilha de compras.
    for (int i = 0; i < quantidade; i++) {
        if (jogador->pilha_compra.num_cartas == 0) {
           for (int j= 0; j < jogador->pilha_descarte.num_cartas; j++) { // Move o conteúdo do descarte para a pilha de compra
                jogador->pilha_compra.cartas[j] = jogador->pilha_descarte.cartas[j];
            }
            jogador->pilha_compra.num_cartas = jogador->pilha_descarte.num_cartas; // Atualiza o contador da compra e zera o descarte
            jogador->pilha_descarte.num_cartas = 0; 
            
            embaralhar_baralho(&jogador->pilha_compra);  // Embaralha as cartas que voltaram para a compra
        }
        if (jogador->pilha_compra.num_cartas > 0) { // se tiver uma carta na pilha de copras
            mover_carta(&jogador->pilha_compra, &jogador->cartas_jogador); // move a carta do topo da pilha_Compra para a mao
            jogador->pilha_compra.num_cartas--; // Reduz o contador da pilha de compra
            
        } else {
            break; 
        }
        if (jogador->cartas_jogador.num_cartas >= 10) { // max 10 cartas na mao
            break; 
        }
    }
}

void inicio_turno(Jogador* jogador){
   jogador->dados_base.escudo = 0; //escudo = 0
   jogador->energia_turno = 3;   // energia = 3
   comprar_carta(jogador, 5);  // ganha 5 cartas na mao

}

void fim_turno(Jogador* jogador){
    while (jogador->cartas_jogador.num_cartas > 0) {
        mover_carta(&jogador->cartas_jogador, &jogador->pilha_descarte); // precisa do while pois mover_carta move somente 1 carta
        jogador->cartas_jogador.num_cartas--; //reduzir até chegar à 0
    }   
    jogador->dados_base.escudo = 0;
    jogador->energia_turno = 0; 
}

