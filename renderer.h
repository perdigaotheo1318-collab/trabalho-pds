#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "combate.h"

typedef struct {
    ALLEGRO_DISPLAY* display;
    ALLEGRO_BITMAP* display_buffer;
    ALLEGRO_FONT* font;

    // Sprites / imagens
    ALLEGRO_BITMAP* background;
    ALLEGRO_BITMAP* sprite_heroi;
    ALLEGRO_BITMAP* sprite_inimigo_forte;
    ALLEGRO_BITMAP* sprite_inimigo_fraco;

    Combate* estado;
} Renderer;

void FillRenderer(Renderer* renderer);
void ClearRenderer(Renderer* renderer);
void Render(Renderer* renderer);

void RenderBackground(Renderer* renderer);
void RenderPlayer(Renderer* renderer);
void RenderEnemies(Renderer* renderer);
void RenderPlayerHand(Renderer* renderer);
void RenderHud(Renderer* renderer);
void RenderDeck(Renderer* renderer, int x_left, int y_top);

#endif
