#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "renderer.h"
#include "utils.h"
#include "combate.h"
#include "jogador.h"
#include "grupoinimigos.h"
#include "teclado.h"

int main() {
    srand((unsigned)time(NULL));

    must_init(al_init(), "allegro");
    must_init(al_init_image_addon(), "allegro_image");
    must_init(al_init_primitives_addon(), "allegro_primitives");
    must_init(al_init_font_addon(), "allegro_font");
    must_init(al_init_ttf_addon(), "allegro_ttf");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    ALLEGRO_EVENT event;
    unsigned char keyboard_keys[ALLEGRO_KEY_MAX];
    ClearKeyboardKeys(keyboard_keys);

    Combate estado_do_jogo;
    Renderer renderer;

    FillRenderer(&renderer);
    renderer.estado = &estado_do_jogo;

    al_register_event_source(queue, al_get_display_event_source(renderer.display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    inicio_combate(&estado_do_jogo);

    al_start_timer(timer);

    bool player_turn_started = false;
    while (estado_do_jogo.jogador.dados_base.vida_atual > 0 &&
           estado_do_jogo.num_combates_vencidos < 10) {

        al_wait_for_event(queue, &event);
        bool done = false;
        bool redraw = false;

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;

                if (estado_do_jogo.vez == vez_jogador) {
                    if (!player_turn_started) {
                        inicio_turno(&estado_do_jogo.jogador);
                        player_turn_started = true;
                    }
                    Processa_Input_e_Acao(&estado_do_jogo, keyboard_keys);
                    if (estado_do_jogo.vez != vez_jogador) {
                        player_turn_started = false;
                    }
                } else {
                    turno_inimigos(&estado_do_jogo.inimigos, &estado_do_jogo.jogador);
                    mudar_turno(&estado_do_jogo);
                    player_turn_started = false;
                }

                // atalho sair
                if (keyboard_keys[ALLEGRO_KEY_Q] & GAME_KEY_DOWN) {
                    done = true;
                }

                ClearKeyboardKeys(keyboard_keys);
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                keyboard_keys[event.keyboard.keycode] = GAME_KEY_SEEN | GAME_KEY_DOWN;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                keyboard_keys[event.keyboard.keycode] &= ~GAME_KEY_DOWN;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
            default:
                break;
        }

        if (done) break;

        if (redraw) {
            fim_combate(&estado_do_jogo);
            Render(&renderer);
        }
    }

    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    ClearRenderer(&renderer);
    return 0;
}
