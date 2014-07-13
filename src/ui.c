#include "ui.h"

int get_input(SDL_Event event, Asset_t * assets, Floor_t * floor, Player_t * player)
{
    int tile_x, tile_y;
    bool move_result;
    int current_tile;

    if (SDL_PollEvent(&event) == 0) {
        // No event, carry on
        return 0;
    }
    if (event.type == SDL_QUIT) {
        // Handle quitting
        return -1;
    }
    // Handle arrow keys
    if (event.type == SDL_KEYDOWN) {
        current_tile = floor_get_tile(floor, player->x, player->y);
        if (!tile_has_flag(current_tile, TILEFLAG_IMMOVABLE)) {
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
            case SDLK_ESCAPE:
                Mix_PlayChannel(-1, assets->death_sound, 0);
                return 1;
                break;
            default:
                move_result = true;
                break;
            }
            if (!move_result) {
                Mix_PlayChannel(-1, assets->grunt_sound, 0);
            }
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
    return 0;
}

int check_level_complete(Floor_t * floor, Player_t * player)
{
    if (floor->coins - player->coins <= 0) {
        if (floor_get_tile(floor, player->x, player->y) == 7) {
            return (floor->level_number + 1);
        }
    }
    return 0;
}

int check_death(Floor_t * floor, Player_t * player)
{
    if (floor_get_tile(floor, player->x, player->y) == 4) {
        return (floor->level_number);
    }
    return 0;
}

int logic(Timer_t * tick_timer, Asset_t * assets, Floor_t * floor, Player_t * player)
{
    int next_level;
    int current_tile;

    player_check_get_item(assets, floor, player);
    if (((next_level = check_level_complete(floor, player)) > 0)) {
        return next_level;
    }
    if (((next_level = check_death(floor, player)) != 0)) {
        Mix_PlayChannel(-1, assets->death_sound, 0);
        return next_level;
    }
    floor_increment_time(floor);
    if (floor->time_left < 0) {
        Mix_PlayChannel(-1, assets->death_sound, 0);
        return floor->level_number;
    }
    if (timer_tick(tick_timer, TICK_FREQ)) {
        current_tile = floor_get_tile(floor, player->x, player->y);
        if (tile_has_flag(current_tile, TILEFLAG_SLIPPERY)) {
            player_move(floor, player, player->direction);
        }

        // TODO: Events when game tick occurs
    }
    return 0;
}

int play_loop(Asset_t * assets, Floor_t * floor, Player_t * player)
{
    Timer_t fps_timer, tick_timer;
    SDL_Event event;
    int input_result;
    int result;

    timer_init(&fps_timer);
    timer_init(&tick_timer);
    while ((input_result = get_input(event, assets, floor, player)) != - 1) {
        if (input_result == 1) {
            return floor->level_number;
        }
        result = logic(&tick_timer, assets, floor, player);
        if (result != 0) {
            Mix_PlayChannel(-1, assets->win_sound, 0);
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
