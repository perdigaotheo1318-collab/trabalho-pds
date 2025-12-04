#include "renderer.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include "constants.h"
#include "utils.h"

static void RenderHealthBar(float x_begin, float x_end, float y_down_left,
                            ALLEGRO_FONT* font, int vida_atual, int vida_max) {
    float largura = x_end - x_begin;
    float altura = HEALTH_BAR_HEIGHT;
    float percent = (vida_max > 0) ? ((float)vida_atual / (float)vida_max) : 0.0f;
    if (percent < 0) percent = 0;
    if (percent > 1) percent = 1;
    float filled = largura * percent;

    al_draw_filled_rounded_rectangle(x_begin, y_down_left - altura, x_end, y_down_left, 4, 4, al_map_rgb(90, 90, 90));
    al_draw_filled_rounded_rectangle(x_begin, y_down_left - altura, x_begin + filled, y_down_left, 4, 4, al_map_rgb(0, 180, 0));
    char buf[32];
    snprintf(buf, sizeof(buf), "%d/%d", vida_atual, vida_max);
    al_draw_text(font, al_map_rgb(255,255,255), (x_begin + x_end)/2.0f, y_down_left - altura + 2, ALLEGRO_ALIGN_CENTRE, buf);
}

void FillRenderer(Renderer* r) {
    r->display = al_create_display((int)DISPLAY_WIDTH, (int)DISPLAY_HEIGHT);
    must_init(r->display, "display");
    r->display_buffer = al_create_bitmap((int)DISPLAY_BUFFER_WIDTH, (int)DISPLAY_BUFFER_HEIGHT);
    must_init(r->display_buffer, "display buffer");

    r->font = al_load_ttf_font("assets/arial.ttf", 18, 0);
    if (!r->font) {
        r->font = al_create_builtin_font();
    }

    // carrega bitmaps -- ajusta nomes se necessário
    r->background = al_load_bitmap("assets/background.png");
    r->sprite_heroi = al_load_bitmap("assets/heroi.png");
    r->sprite_inimigo_forte = al_load_bitmap("assets/inimigoforte.png");
    r->sprite_inimigo_fraco = al_load_bitmap("assets/inimigofraco.png");

    // opcional: verificar load
    if (!r->background) {
        fprintf(stderr, "Erro: nao carregou background.png\n");
    }
    if (!r->sprite_heroi) {
        fprintf(stderr, "Aviso: nao carregou heroi.png — usará placeholder\n");
    }
    if (!r->sprite_inimigo_forte) {
        fprintf(stderr, "Aviso: nao carregou inimigoforte.png\n");
    }
    if (!r->sprite_inimigo_fraco) {
        fprintf(stderr, "Aviso: nao carregou inimigofraco.png\n");
    }

    r->estado = NULL;
}

void RenderBackground(Renderer* r) {
    al_set_target_bitmap(r->display_buffer);
    if (r->background) {
        al_draw_scaled_bitmap(r->background,
            0, 0,
            al_get_bitmap_width(r->background),
            al_get_bitmap_height(r->background),
            0, 0,
            DISPLAY_BUFFER_WIDTH,
            DISPLAY_BUFFER_HEIGHT,
            0);
    } else {
        al_clear_to_color(al_map_rgb(40,40,40));
    }
}

void RenderPlayer(Renderer* r) {
    if (!r->estado) return;
    Jogador* j = &r->estado->jogador;
    float cx = PLAYER_BEGIN_X;
    float cy = PLAYER_BEGIN_Y;
    if (r->sprite_heroi) {
        int w = al_get_bitmap_width(r->sprite_heroi);
        int h = al_get_bitmap_height(r->sprite_heroi);
        al_draw_scaled_bitmap(r->sprite_heroi,
            0,0,w,h,
            cx - w/2, cy - h/2,
            w, h, 0);
    } else {
        al_draw_filled_circle(cx, cy, PLAYER_RADIUS, al_map_rgb(255,255,255));
    }
    RenderHealthBar(cx - 80, cx + 80, cy + PLAYER_RADIUS + 30,
                    r->font,
                    j->dados_base.vida_atual, j->dados_base.vida_maxima);
}

void RenderEnemies(Renderer* r) {
    if (!r->estado) return;
    grupo_inimigos* g = &r->estado->inimigos;
    float base_x = DISPLAY_BUFFER_WIDTH * 0.65f;
    float spacing = 220.0f;
    for (int i = 0; i < 2; i++) {
        inimigo* in = &g->inimigos[i];
        if (in->dados_base.vida_atual <= 0) continue;
        float x = base_x + i * spacing;
        float y = PLAYER_BEGIN_Y;
        ALLEGRO_BITMAP* bmp = (in->tipo == FORTE ? r->sprite_inimigo_forte : r->sprite_inimigo_fraco);
        if (bmp) {
            int w = al_get_bitmap_width(bmp);
            int h = al_get_bitmap_height(bmp);
            al_draw_scaled_bitmap(bmp,
                0,0,w,h,
                x - w/2, y - h/2,
                w, h, 0);
        } else {
            al_draw_filled_rectangle(x - 70, y - 100, x + 70, y + 40, al_map_rgb(200,200,200));
        }
        // barra de vida
        RenderHealthBar(x - 70, x + 70, y + 40 + 20,
                        r->font,
                        in->dados_base.vida_atual, in->dados_base.vida_maxima);
    }
}

void RenderPlayerHand(Renderer* r) {
    if (!r->estado) return;
    Baralho* mao = &r->estado->jogador.cartas_jogador;
    int n = mao->num_cartas;
    if (n <= 0) return;

    float total_width = n * (CARD_WIDTH + 24);
    float start_x = (DISPLAY_BUFFER_WIDTH - total_width) / 2.0f + 20;
    float y = HAND_BEGIN_Y;
    for (int i = 0; i < n; i++) {
        float x = start_x + i * (CARD_WIDTH + 24);
        al_draw_filled_rounded_rectangle(x, y, x + CARD_WIDTH, y + CARD_HEIGHT, 8, 8, al_map_rgb(255,255,255));
        al_draw_rounded_rectangle(x, y, x + CARD_WIDTH, y + CARD_HEIGHT, 8, 8, al_map_rgb(120,120,120), 3);
        carta *c = &mao->cartas[i];
        const char *tipo = (c->tipo == ATAQUE) ? "ATK" : (c->tipo == DEFESA) ? "DEF" : "ESP";
        char top[64];
        snprintf(top, sizeof(top), "%s C:%d X:%d", tipo, c->custoE, c->efeitoX);
        al_draw_text(r->font, al_map_rgb(0,0,0), x + 8, y + 8, 0, top);
        char val[16];
        snprintf(val, sizeof(val), "%d", c->efeitoX);
        al_draw_text(r->font, al_map_rgb(0,0,0), x + CARD_WIDTH/2, y + CARD_HEIGHT/2, ALLEGRO_ALIGN_CENTRE, val);
        if (i == r->estado->num_carta) {
            al_draw_rounded_rectangle(x-6, y-6, x + CARD_WIDTH+6, y + CARD_HEIGHT+6, 12, 12, al_map_rgb(255,215,0), 6);
        }
    }
}
// --- FUNÇÃO QUE FALTAVA ---
void RenderDeck(Renderer* renderer, int x_left, int y_top) {
    al_set_target_bitmap(renderer->display_buffer);

    // desenha a caixinha da pilha de compra
    al_draw_filled_rectangle(
        x_left, y_top,
        x_left + DECK_WIDTH,
        y_top + DECK_HEIGHT,
        al_map_rgb(255, 255, 255)
    );

    al_draw_rectangle(
        x_left, y_top,
        x_left + DECK_WIDTH,
        y_top + DECK_HEIGHT,
        al_map_rgb(200, 0, 0),
        3
    );

    if (renderer->estado) {
        int qtd = renderer->estado->jogador.pilha_compra.num_cartas;
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", qtd);

        al_draw_text(
            renderer->font,
            al_map_rgb(0, 0, 0),
            x_left + DECK_WIDTH + 8,
            y_top + DECK_HEIGHT / 2 - 8,
            0,
            buf
        );
    }
}

void RenderHud(Renderer* r) {
    if (!r->estado) return;
    Jogador* j = &r->estado->jogador;
    char buf[32];
    snprintf(buf, sizeof(buf), "Energia: %d", j->energia_turno);
    al_draw_filled_rectangle(10, 10, 200, 50, al_map_rgb(30,30,30));
    al_draw_rectangle(10, 10, 200, 50, al_map_rgb(255,255,255), 2);
    al_draw_text(r->font, al_map_rgb(255,255,255), 20, 18, 0, buf);

    // pilha de compra
    RenderDeck(r, DRAW_DECK_X, DRAW_DECK_Y);

    int discard = r->estado->jogador.pilha_descarte.num_cartas;
    char d[32];
    snprintf(d, sizeof(d), "Desc: %d", discard);
    al_draw_text(r->font, al_map_rgb(255,255,255), DRAW_DECK_X, DRAW_DECK_Y + DECK_HEIGHT + 8, 0, d);
}

void Render(Renderer* r) {
    if (!r || !r->estado) return;
    al_set_target_bitmap(r->display_buffer);
    RenderBackground(r);
    RenderPlayer(r);
    RenderEnemies(r);
    RenderHud(r);
    RenderPlayerHand(r);

    al_set_target_backbuffer(r->display);
    al_draw_scaled_bitmap(r->display_buffer,
                          0, 0,
                          (int)DISPLAY_BUFFER_WIDTH, (int)DISPLAY_BUFFER_HEIGHT,
                          0, 0,
                          (int)DISPLAY_WIDTH, (int)DISPLAY_HEIGHT,
                          0);
    al_flip_display();
}

void ClearRenderer(Renderer* r) {
    if (!r) return;
    if (r->background) al_destroy_bitmap(r->background);
    if (r->sprite_heroi) al_destroy_bitmap(r->sprite_heroi);
    if (r->sprite_inimigo_forte) al_destroy_bitmap(r->sprite_inimigo_forte);
    if (r->sprite_inimigo_fraco) al_destroy_bitmap(r->sprite_inimigo_fraco);
    if (r->font) al_destroy_font(r->font);
    if (r->display_buffer) al_destroy_bitmap(r->display_buffer);
    if (r->display) al_destroy_display(r->display);
}
