#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "constants.h"
#include "renderer.h"
#include "utils.h"
#include "combate.h"
#include "jogador.h"
#include "grupoinimigos.h"
#include "teclado.h"

int main() {
    srand(time(NULL));

    must_init(al_init(), "allegro");
    must_init(al_init_image_addon(), "image");
    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_init_font_addon(), "font");
    must_init(al_init_ttf_addon(), "ttf");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    ALLEGRO_EVENT event;
    unsigned char keys[ALLEGRO_KEY_MAX];
    ClearKeyboardKeys(keys);

    Renderer renderer;
    Combate estado;
    FillRenderer(&renderer);

    inicio_combate(&estado);
    renderer.estado = &estado;

    al_register_event_source(queue, al_get_display_event_source(renderer.display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    while (1) {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;

        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
            keys[event.keyboard.keycode] = GAME_KEY_DOWN | GAME_KEY_SEEN;

        if (event.type == ALLEGRO_EVENT_KEY_UP)
            keys[event.keyboard.keycode] &= ~GAME_KEY_DOWN;

        if (event.type == ALLEGRO_EVENT_TIMER) {
            Processa_Input_e_Acao(&estado, keys);
            ClearKeyboardKeys(keys);
            Render(&renderer);
        }
    }

    ClearRenderer(&renderer);
    return 0;
}
