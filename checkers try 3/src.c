#include "display.h"
#include "logic.h"
#include "config.h"
#include "event_handler.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/mouse.h>



int main() {
    if (init_allegro() != 0) {
        return -1;
    }
    mainLoop();
}


int init_allegro() {
    if (!al_init())
    {
        printf("couldn't initialize allegro\n");
        return 1;
    }

    if (!al_install_keyboard())
    {
        printf("couldn't initialize keyboard\n");
        return 1;
    }
    if (!al_init_image_addon())
    {
        printf("couldn't initialize images\n");
        return 1;
    }
    if (!al_init_primitives_addon()) {
        printf("couldn't initialize primitives\n");
        return 1;
    }

    if (!al_install_mouse()) {
        printf("couldn't initialize mouse\n");
        return 1;
    }

    if (!al_init_ttf_addon()) {
        printf("couldn't initialize ttf");
        return 1;
    }    

    return 0;
}

int check_utils(ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_DISPLAY* disp,
                ALLEGRO_FONT* font, ALLEGRO_BITMAP** textures, struct game* game)
{
    if (!timer)
    {
        printf("couldn't initialize timer\n");
        return 1;
    }

    if (!queue)
    {
        printf("couldn't initialize queue\n");
        return 1;
    }

    if (!disp)
    {
        printf("couldn't initialize display\n");
        return 1;
    }

    if (!font)
    {
        printf("couldn't initialize font\n");
        return 1;
    }

    if (!textures) {
        printf("couldn't load textures");
        return 1;
    }
    if (!game) {
        printf("couldn't initialize game");
        return 1;
    }

    return 0;
}

int mainLoop()
{
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(WINDOW_SIZE, WINDOW_SIZE);
    ALLEGRO_FONT* font = al_load_ttf_font(FONT_PATH, 20, 0);
    ALLEGRO_BITMAP** textures = loadTextures();
    struct game* game = makeGame(BOARD_SIZE);
    if (check_utils(timer, queue, disp, font, textures, game) != 0)
        return -1;
    initPawns(game);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    int done = 0;
    bool redraw = true;
    ALLEGRO_EVENT event;
    int scene = scene_menu;
    int hover = 0;
    int x = 0, y = 0;
    int active = -1;
    int mustMove = -1;
    int win = -1;

    al_start_timer(timer);
    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {

        case ALLEGRO_EVENT_TIMER:
            if (hover != 0) 
                al_set_system_mouse_cursor(disp, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
            else
                al_set_system_mouse_cursor(disp,1);

            switch (scene) {
            case scene_menu:
                drawMenu(font, hover);
                break;
            case scene_board:
                drawBoard(game, textures, font, hover);
                break;
            case scene_results:
                drawResults(font, win, hover);
                break;
            }
            redraw = true;
            break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if (event.mouse.button == 1) {
                x = event.mouse.x;
                y = event.mouse.y;
                moveHandler(&active,&mustMove, game, &scene, x, y, &win);
                scene = clickHandler(x, y, scene, game, &done);
                hover = hoverHandler(x, y, scene);


            }
            break;

        case ALLEGRO_EVENT_MOUSE_AXES:
            x = event.mouse.x;
            y = event.mouse.y;
            hover = hoverHandler(x, y, scene);
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        
        }

        if (done)
            break;

        if (redraw && al_is_event_queue_empty(queue))
        {
            redraw = false;
        }


    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    free(textures);
    destructGame(game);
    return 0;
 }

