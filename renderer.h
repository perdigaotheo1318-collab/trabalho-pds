#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#include "combate.h"

typedef struct {
    ALLEGRO_DISPLAY* display;
    ALLEGRO_BITMAP* display_buffer;
    ALLEGRO_FONT* font;

    // Sprites
    ALLEGRO_BITMAP* background;
    ALLEGRO_BITMAP* sprite_heroi;
    ALLEGRO_BITMAP* sprite_inimigo_fraco;
    ALLEGRO_BITMAP* sprite_inimigo_forte;

    Combate* estado;

} Renderer;

void FillRenderer(Renderer* renderer);
void Render(Renderer* renderer);
void ClearRenderer(Renderer* renderer);

// internas
void RenderBackground(Renderer* r);
void RenderPlayer(Renderer* r);
void RenderEnemies(Renderer* r);
void RenderPlayerHand(Renderer* r);
void RenderHud(Renderer* r);

#endif
