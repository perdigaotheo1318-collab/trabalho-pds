#ifndef INIMIGO_H
#define INIMIGO_H
#include "personagem.h"
#include "baralho.h"

typedef enum{
    FRACO, // 0
    FORTE  // 1

}tipo_inimigo;


typedef struct{
    int atkoudef;  // ataque ou defesa
    int valor_dano;
    int valor_escudo;
}IA_inimigo;

typedef struct{
    personagem dados_base;
    tipo_inimigo tipo;
    IA_inimigo acoes[3];
    int proxima_acao;
    int num_acoes_ia;

}inimigo;

inimigo criar_inimigo(tipo_inimigo tipo);

#endif