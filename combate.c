#include <stdio.h>
#include <stdlib.h>
#include "combate.h"
#include "grupoinimigos.h"
#include "jogador.h"
#include "utils.h"

void inicio_combate(Combate* combate){
    inicio_jogador(&combate->jogador); //chama a função jogador
    inimigos_iniciais(&combate->inimigos); // chama a função inimigo para criar jogador e inimigos iniciais no combate
    combate->vez = vez_jogador; // jogador começa 
    combate->num_inimigo = 0;
    combate->num_carta = 0;
    combate->num_combates_vencidos = 0;   // contadores para ir atualizado
}
void mudar_turno(Combate* combate){  // mudança de turnos entre o jogador e inimigo, no main.c so ocorre depois que as ações do jogador são concluidas e vice-versa
   if (combate->vez == vez_jogador) {
        combate->vez = vez_inimigo;
    } else {
        combate->vez = vez_jogador;
    }
}
void dano_gerado(personagem* alvo, int valor_dano){
    if(valor_dano <=0){
        return;
    }
    if(alvo->escudo >= valor_dano){
        alvo->escudo = alvo->escudo - valor_dano;  // escudo aguenta
    }
    else if(alvo->escudo < valor_dano){
    valor_dano = valor_dano - alvo->escudo;
    alvo->escudo = 0;
    alvo->vida_atual = alvo->vida_atual - valor_dano;
    }
}

void ataque_jogador(Combate* combate){ // jogador jogar carta
    carta carta_selecionada = combate->jogador.cartas_jogador.cartas[combate->num_carta];
    inimigo *alvo = &combate->inimigos.inimigos[combate->num_inimigo];

    if (combate->jogador.energia_turno >= carta_selecionada.custoE && carta_selecionada.tipo == ATAQUE) { // verifica se tem energia e se a carta é do tipo ATAQUE
        dano_gerado(&alvo->dados_base, carta_selecionada.efeitoX); // aplica dano no alvo
        combate->jogador.energia_turno = combate->jogador.energia_turno - carta_selecionada.custoE; // tira a energia que a carta consumiu
        carta carta_descartada = carta_selecionada;  // salva a carta e leva para descarte
        
        
        for (int i = combate->num_carta; i < combate->jogador.cartas_jogador.num_cartas - 1; i++) { // move as cartas depois da carta jogada1 posição para a esquerda
             combate->jogador.cartas_jogador.cartas[i] = combate->jogador.cartas_jogador.cartas[i + 1];
        }
        combate->jogador.cartas_jogador.num_cartas--; //  Diminui o contador da mão

        combate->jogador.pilha_descarte.cartas[combate->jogador.pilha_descarte.num_cartas] = carta_descartada;
        combate->jogador.pilha_descarte.num_cartas++; // coloca a carta salva em descarte
        
        if (alvo->dados_base.vida_atual <= 0) { // atualiza o numero de inimigos vivos
            combate->inimigos.inimigos_vivos--;
        }
    }
}
void defesa_jogador(Combate* combate){
    carta *carta_jogada = &combate->jogador.cartas_jogador.cartas[combate->num_carta]; // carta que o jogador jogou
    if (combate->jogador.energia_turno >= carta_jogada->custoE && carta_jogada->tipo == DEFESA) { // energia disponivel > energia da carta jogada
        combate->jogador.dados_base.escudo = combate->jogador.dados_base.escudo + carta_jogada->efeitoX; // escudo no jogador
        combate->jogador.energia_turno = combate->jogador.energia_turno - carta_jogada->custoE; // tira a energia usada

        carta carta_descartada = *carta_jogada; // para descartar a carta
       for (int i = combate->num_carta; i < combate->jogador.cartas_jogador.num_cartas - 1; i++) { // move as cartas depois da carta jogada1 posição para a esquerda
             combate->jogador.cartas_jogador.cartas[i] = combate->jogador.cartas_jogador.cartas[i + 1];
        }
        combate->jogador.cartas_jogador.num_cartas--; //  Diminui o contador da mão
        combate->jogador.pilha_descarte.cartas[combate->jogador.pilha_descarte.num_cartas] = carta_descartada;
        combate->jogador.pilha_descarte.num_cartas++; // coloca a carta salva em descarte
}
}


void fim_combate(Combate* combate){
    if(combate->jogador.dados_base.vida_atual <=0){
        printf("Derrota!\n");
        return;
    }
    else if(combate->inimigos.inimigos_vivos <= 0){
    combate->num_combates_vencidos++;
    printf("Você venceu o combate!\n");
     if(combate->num_combates_vencidos >= 10){
        printf("Vitória!\n");
     }
     return;
}
else{
    printf("Jogo em progresso!\n");
}
}