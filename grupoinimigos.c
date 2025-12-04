#include <stdio.h>
#include "jogador.h"
#include "personagem.h"
#include "baralho.h"
#include "inimigo.h"
#include "combate.h"
#include "grupoinimigos.h"
#include <stdlib.h>
#include "utils.h"

IA_inimigo criar_acao_ia(int max_custo) {
    IA_inimigo acao;
    int custo_acao = random_int_in_range(0, max_custo);
    
    // 50% de chance de ser ATAQUE, 50% de DEFESA
   if (rand() % 2 == 0) {
    acao.atkoudef = ATAQUE;
} else {
    acao.atkoudef = DEFESA;
}
    // Logica simples para definir o valor do efeito (X)
    if (custo_acao == 0) {
        acao.valor_dano = random_int_in_range(1, 5);
        acao.valor_escudo = random_int_in_range(1, 5);
    } else if (custo_acao == 1) {
        acao.valor_dano = random_int_in_range(5, 10);
        acao.valor_escudo = random_int_in_range(5, 10);
    } else { // Simplificado para custo 2 ou 3
        acao.valor_dano = random_int_in_range(10, 30);
        acao.valor_escudo = random_int_in_range(10, 30);
    }
    // Zera o campo que não será usado 
    if (acao.atkoudef == ATAQUE) {
        acao.valor_escudo = 0;
    } else {
        acao.valor_dano = 0;
    }

    return acao;
}

inimigo criar_inimigo(tipo_inimigo tipo) {
    inimigo inimigo_novo;
    int max_acoes;
    int max_custo;
    int tem_ataque = 0;

    // Definindo Stats de Vida e Limites da IA
    inimigo_novo.tipo = tipo;
    if (tipo == FORTE) {
        inimigo_novo.dados_base.vida_maxima = random_int_in_range(40, 100);
        max_acoes = random_int_in_range(2, 3);
        max_custo = 3;
    } else { // FRACO
        inimigo_novo.dados_base.vida_maxima = random_int_in_range(10, 30);
        max_acoes = random_int_in_range(1, 2);
        max_custo = 1;
        inimigo_novo.num_acoes_ia = max_acoes;
    }

    // Gerando o Ciclo de Ações da IA 
    for (int i = 0; i < max_acoes; i++) {
        // Usa a função auxiliar para criar a ação
        inimigo_novo.acoes[i] = criar_acao_ia(max_custo);
        if (inimigo_novo.acoes[i].atkoudef == ATAQUE) {
            tem_ataque = 1;
        }
    }
    
    //  GARANTE PELO MENOS 1 ATAQUE
    if (!tem_ataque) {
        // Se não gerou ataque, força o primeiro a ser ataque.
        inimigo_novo.acoes[0].atkoudef = ATAQUE;
        inimigo_novo.acoes[0].valor_escudo = 0; // Garante que o escudo seja zero no ataque.
    }

    inimigo_novo.dados_base.vida_atual = inimigo_novo.dados_base.vida_maxima;
    inimigo_novo.dados_base.escudo = 0;
    inimigo_novo.proxima_acao = 0; // Sempre começa na primeira ação do ciclo.
    return inimigo_novo;
}

void inimigos_iniciais(grupo_inimigos* grupo) {
    grupo->inimigos_vivos = 2; // Inicia com 2 inimigos vivos
    int forte_presente = 0;     // numero de fortes
    float chance_forte = 0.05; // 5% de chance

    // --- INIMIGO 1 (Índice 0) ---
    if ((float)rand() / RAND_MAX <= chance_forte) {
        grupo->inimigos[0] = criar_inimigo(FORTE);
        forte_presente = 1;
    } else {
        grupo->inimigos[0] = criar_inimigo(FRACO);
    }

    // --- INIMIGO 2 (Índice 1) ---
    if (forte_presente) {
        // Se o primeiro é forte, o segundo DEVE ser fraco.
        grupo->inimigos[1] = criar_inimigo(FRACO);
    } else {
        // Se o primeiro é fraco, o segundo ainda tem 5% de chance.
        if ((float)rand() / RAND_MAX <= chance_forte) {
             grupo->inimigos[1] = criar_inimigo(FORTE);
        } else {
             grupo->inimigos[1] = criar_inimigo(FRACO);
        }
    }
}

void turno_inimigos(grupo_inimigos* grupo, Jogador* jogador){
        for (int i = 0; i < 2; i++) {
        inimigo* inimigo_atual = &grupo->inimigos[i];

        // Pular inimigos que já estão mortos
        if (inimigo_atual->dados_base.vida_atual <= 0) {
            continue;
        }

        inimigo_atual->dados_base.escudo = 0; // zerar escudo no inicio do turno
        int acao_ind = inimigo_atual->proxima_acao;
        IA_inimigo acao = inimigo_atual->acoes[acao_ind];

        if (acao.atkoudef == ATAQUE) {
            dano_gerado(&jogador->dados_base, acao.valor_dano);
        } else {
            inimigo_atual->dados_base.escudo += acao.valor_escudo;
        }

        // Avança para a próxima ação (com wrap)
        if (inimigo_atual->num_acoes_ia > 0) {
            inimigo_atual->proxima_acao = (inimigo_atual->proxima_acao + 1) % inimigo_atual->num_acoes_ia;
        } else {
            inimigo_atual->proxima_acao = 0;
        }
    }
}