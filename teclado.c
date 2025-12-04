#include "teclado.h"
#include "constants.h"
#include "jogador.h"
#include "grupoinimigos.h"
#include "combate.h"
#include "baralho.h"
#include "personagem.h"

#include <allegro5/allegro5.h>
#include <stdio.h>

/* navega cartas (wrap) */
static void nav_cartas(Combate* c, int dir) {
    int n = c->jogador.cartas_jogador.num_cartas;
    if (n <= 0) {
        c->num_carta = 0;
        return;
    }
    if (dir < 0) {
        c->num_carta--;
        if (c->num_carta < 0) c->num_carta = n - 1;
    } else {
        c->num_carta++;
        if (c->num_carta >= n) c->num_carta = 0;
    }
}

/* navega inimigo (0/1) pulando mortos */
static void nav_inimigo(Combate* c, int dir) {
    int start = c->num_inimigo;
    int attempts = 0;
    while (attempts < 2) {
        if (dir < 0) start = (start + 1) % 2;
        else start = (start + 1) % 2;
        if (c->inimigos.inimigos[start].dados_base.vida_atual > 0) {
            c->num_inimigo = start;
            return;
        }
        attempts++;
    }
}

/* aplica carta especial: descarta mão e compra 5 */
static void aplicar_especial(Combate* c) {
    while (c->jogador.cartas_jogador.num_cartas > 0) {
        mover_carta(&c->jogador.cartas_jogador, &c->jogador.pilha_descarte);
        c->jogador.cartas_jogador.num_cartas--;
    }
    comprar_carta(&c->jogador, 5);
}

void Processa_Input_e_Acao(Combate* combate, unsigned char keyboard_keys[]) {
    if (!combate) return;

    /* Q: sair do jogo (marca jogador morto para o loop terminar) */
    if (keyboard_keys[ALLEGRO_KEY_Q] & GAME_KEY_DOWN) {
        combate->jogador.dados_base.vida_atual = 0;
        return;
    }

    /* ESPAÇO: reduzir vida de todos os inimigos a 0 (não encerra o jogo imediatamente) */
    if (keyboard_keys[ALLEGRO_KEY_SPACE] & GAME_KEY_DOWN) {
        for (int i = 0; i < 2; i++) {
            combate->inimigos.inimigos[i].dados_base.vida_atual = 0;
        }
        combate->inimigos.inimigos_vivos = 0;
        return;
    }

    /* X: reduzir vida do jogador para 1 */
    if (keyboard_keys[ALLEGRO_KEY_X] & GAME_KEY_DOWN) {
        combate->jogador.dados_base.vida_atual = 1;
        return;
    }

    /* ESC: encerrar turno do jogador (descarta + muda para inimigo) */
    if (keyboard_keys[ALLEGRO_KEY_ESCAPE] & GAME_KEY_DOWN) {
        fim_turno(&combate->jogador);
        combate->vez = vez_inimigo;
        return;
    }

    /* Setas: navegar cartas (esquerda/direita) */
    if (keyboard_keys[ALLEGRO_KEY_LEFT] & GAME_KEY_DOWN) {
        nav_cartas(combate, -1);
        return;
    }
    if (keyboard_keys[ALLEGRO_KEY_RIGHT] & GAME_KEY_DOWN) {
        nav_cartas(combate, +1);
        return;
    }

    /* ENTER: jogar a carta selecionada / confirmar seleção */
    if (keyboard_keys[ALLEGRO_KEY_ENTER] & GAME_KEY_DOWN) {
        int carta_idx = combate->num_carta;
        int mao_n = combate->jogador.cartas_jogador.num_cartas;
        if (carta_idx < 0 || carta_idx >= mao_n) return;

        carta carta_sel = combate->jogador.cartas_jogador.cartas[carta_idx];

        /* especial */
        if (carta_sel.tipo == ESPECIAL) {
            aplicar_especial(combate);
            /* colocar a especial no descarte (simulação simples) */
            combate->jogador.pilha_descarte.cartas[combate->jogador.pilha_descarte.num_cartas++] = carta_sel;
            return;
        }

        /* ataque */
        if (carta_sel.tipo == ATAQUE) {
            /* garante alvo vivo */
            if (combate->inimigos.inimigos[combate->num_inimigo].dados_base.vida_atual <= 0) {
                if (combate->inimigos.inimigos[0].dados_base.vida_atual > 0) combate->num_inimigo = 0;
                else if (combate->inimigos.inimigos[1].dados_base.vida_atual > 0) combate->num_inimigo = 1;
                else return;
            }
            ataque_jogador(combate);

            /* atualiza contagem vivos */
            int vivos = 0;
            for (int i = 0; i < 2; i++) if (combate->inimigos.inimigos[i].dados_base.vida_atual > 0) vivos++;
            combate->inimigos.inimigos_vivos = vivos;
            return;
        }

        /* defesa */
        if (carta_sel.tipo == DEFESA) {
            defesa_jogador(combate);
            return;
        }
    }
}
