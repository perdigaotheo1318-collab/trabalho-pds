#include "renderer.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "constants.h"
#include "utils.h"
#include "combate.h"

#include <stdio.h>

void FillRenderer(Renderer* r) {
    r->display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    must_init(r->display, "display");

    r->display_buffer = al_create_bitmap(DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT);
    must_init(r->display_buffer, "buffer");

    r->font = al_load_ttf_font("arial.ttf", 26, 0);
    if (!r->font) r->font = al_create_builtin_font();

    // === carrega imagens ===
    r->background          = al_load_bitmap("assets/background.png");
    r->sprite_heroi        = al_load_bitmap("assets/heroi.png");
    r->sprite_inimigo_fraco  = al_load_bitmap("assets/inimigo_fraco.png");
    r->sprite_inimigo_forte  = al_load_bitmap("assets/inimigo_forte.png");

    if (!r->background || !r->sprite_heroi ||
        !r->sprite_inimigo_fraco || !r->sprite_inimigo_forte)
    {
        printf("ERRO: não conseguiu carregar sprites!\n");
        exit(1);
    }

    r->estado = NULL;
}

void RenderBackground(Renderer* r) {
    al_draw_scaled_bitmap(
        r->background,
        0, 0,
        al_get_bitmap_width(r->background),
        al_get_bitmap_height(r->background),
        0, 0,
        DISPLAY_BUFFER_WIDTH,
        DISPLAY_BUFFER_HEIGHT,
        0
    );
}

void RenderPlayer(Renderer* r) {
    float px = 220;
    float py = 400;

    al_draw_scaled_bitmap(
        r->sprite_heroi,
        0, 0,
        al_get_bitmap_width(r->sprite_heroi),
        al_get_bitmap_height(r->sprite_heroi),
        px, py,
        280, 480,
        0
    );

    Jogador* j = &r->estado->jogador;
    char vida[32];
    sprintf(vida, "%d/%d", j->dados_base.vida_atual, j->dados_base.vida_maxima);

    al_draw_text(r->font, al_map_rgb(255,255,255), px+140, py+450, ALLEGRO_ALIGN_CENTER, vida);
}

void RenderEnemies(Renderer* r) {
    grupo_inimigos* G = &r->estado->inimigos;

    float base_x = 800;
    float base_y = 380;

    for (int i=0; i < G->inimigos_vivos; i++)
    {
        inimigo* in = &G->inimigos[i];

        ALLEGRO_BITMAP* sprite =
            (in->tipo == 1) ? r->sprite_inimigo_forte : r->sprite_inimigo_fraco;

        al_draw_scaled_bitmap(
            sprite,
            0, 0,
            al_get_bitmap_width(sprite),
            al_get_bitmap_height(sprite),
            base_x + i*260,
            base_y,
            240, 420,
            0
        );

        char vida[32];
        sprintf(vida, "%d/%d", in->dados_base.vida_atual, in->dados_base.vida_maxima);

        al_draw_text(
            r->font,
            al_map_rgb(255,255,255),
            base_x + i*260 + 120,
            base_y + 420,
            ALLEGRO_ALIGN_CENTER,
            vida
        );
    }
}

void RenderHud(Renderer* r) {
    Jogador* j = &r->estado->jogador;

    char buf[32];
    sprintf(buf, "Energia: %d", j->energia_turno);

    al_draw_text(r->font, al_map_rgb(255,255,255), 20, 20, 0, buf);
}

void RenderPlayerHand(Renderer* r) {
    Baralho* mao = &r->estado->jogador.cartas_jogador;

    float start_x = 200;
    float y = 830;

    for (int i=0; i<mao->num_cartas; i++) {
        float x = start_x + i * 220;

        al_draw_filled_rounded_rectangle(x, y, x+200, y+280, 14, 14, al_map_rgb(255,255,255));
        al_draw_rounded_rectangle(x, y, x+200, y+280, 14, 14, al_map_rgb(10,10,10), 4);

        carta* c = &mao->cartas[i];

        char topo[64];
        sprintf(topo, "%s - C%d",
            c->tipo == ATAQUE ? "ATK" :
            c->tipo == DEFESA ? "DEF" : "ESP",
            c->custoE
        );

        al_draw_text(r->font, al_map_rgb(0,0,0), x+100, y+10, ALLEGRO_ALIGN_CENTER, topo);

        char valor[16];
        sprintf(valor, "%d", c->efeitoX);

        al_draw_text(r->font, al_map_rgb(0,0,0), x+100, y+130, ALLEGRO_ALIGN_CENTER, valor);

        if (r->estado->num_carta == i)
            al_draw_rounded_rectangle(x-6, y-6, x+206, y+286, 14,14, al_map_rgb(255,255,0), 6);
    }
}

void Render(Renderer* r) {

    al_set_target_bitmap(r->display_buffer);

    RenderBackground(r);
    RenderHud(r);
    RenderPlayer(r);
    RenderEnemies(r);
    RenderPlayerHand(r);

    al_set_target_backbuffer(r->display);
    al_draw_scaled_bitmap(
        r->display_buffer,
        0,0,
        DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT,
        0,0,
        DISPLAY_WIDTH, DISPLAY_HEIGHT,
        0
    );
    al_flip_display();
}

void ClearRenderer(Renderer* r) {
    al_destroy_bitmap(r->background);
    al_destroy_bitmap(r->sprite_heroi);
    al_destroy_bitmap(r->sprite_inimigo_fraco);
    al_destroy_bitmap(r->sprite_inimigo_forte);

    al_destroy_bitmap(r->display_buffer);
    al_destroy_display(r->display);
    al_destroy_font(r->font);
}
