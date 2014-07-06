#include "ui.h"

bool get_input(SDL_Event event, Asset_t * assets, Floor_t * floor, Player_t * player)
{
    int tile_x, tile_y;
    bool move_result;

    if (SDL_PollEvent(&event) == 0) {
        // No event, carry on
        return true;
    }
    if (event.type == SDL_QUIT ||
        (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
        // Handle quitting
        return false;
    }
    // Handle arrow keys
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:
            move_result = player_move(floor, player, PLAYER_UP);
            break;
        case SDLK_DOWN:
            move_result = player_move(floor, player, PLAYER_DOWN);
            break;
        case SDLK_LEFT:
            move_result = player_move(floor, player, PLAYER_LEFT);
            break;
        case SDLK_RIGHT:
            move_result = player_move(floor, player, PLAYER_RIGHT);
            break;
        default:
            break;
        }
        if (!move_result) {
            Mix_PlayChannel(-1, assets->grunt_sound, 0);
        }
    }
    // Handle mouse
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
            tile_x = (event.motion.x - BOARD_OFFSET_X) / TILEW;
            tile_y = (event.motion.y - BOARD_OFFSET_Y) / TILEH;
            if (tile_x >= 0 && tile_x < TILE_DISPLAY_WIDTH &&
                tile_y >= 0 && tile_y < TILE_DISPLAY_HEIGHT) {
                // printf("clicked %d %d\n", tile_x, tile_y);
            }
        }
    }
    return true;
}


int logic(Timer_t * tick_timer, Floor_t * floor, Player_t * player)
{
    player_check_get_item(floor, player);
    floor_increment_time(floor);
    if (floor->coins - player->coins <= 0) {
        return floor->level_number + 1;
    }
    if (timer_tick(tick_timer, TICK_FREQ)) {
        // TODO: Events when game tick occurs
    }
    return 0;
}

int play_loop(Asset_t * assets, Floor_t * floor, Player_t * player)
{
    Timer_t fps_timer, tick_timer;
    SDL_Event event;
    int result;

    timer_init(&fps_timer);
    timer_init(&tick_timer);
    while (get_input(event, assets, floor, player)) {
        result = logic(&tick_timer, floor, player);
        if (result != 0) {
            if (result <= MAX_LEVEL) {
                return result;
            } else {
                return 0;
            }
        }
        render(assets, floor, player);
        timer_wait_fps(&fps_timer);
    }
    return 0;
}
