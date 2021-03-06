#include "ui.h"

static int handle_keys(SDL_Event event, Asset_t * assets, Floor_t * floor, Player_t * player);

static int attempt_player_move(Asset_t * assets, Floor_t * floor, Player_t * player, int direction);

int attempt_player_move(Asset_t * assets, Floor_t * floor, Player_t * player, int direction)
{
    int move_result;

    move_result = player_move(floor, player, direction, false);
    if (!move_result) {
        Mix_PlayChannel(-1, assets->grunt_sound, 0);
    }
    return move_result;
}

int handle_keys(SDL_Event event, Asset_t * assets, Floor_t * floor, Player_t * player)
{
    // Handle arrow keys
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:
            attempt_player_move(assets, floor, player, PLAYER_UP);
            break;
        case SDLK_DOWN:
            attempt_player_move(assets, floor, player, PLAYER_DOWN);
            break;
        case SDLK_LEFT:
            attempt_player_move(assets, floor, player, PLAYER_LEFT);
            break;
        case SDLK_RIGHT:
            attempt_player_move(assets, floor, player, PLAYER_RIGHT);
            break;
        default:
            break;
        }
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            Mix_PlayChannel(-1, assets->death_sound, 0);
            return 1;
        }
    }
    return 0;
}

int get_input(SDL_Event event, Asset_t * assets, Floor_t * floor, Player_t * player)
{
    int tile_x, tile_y;
    int player_display_x, player_display_y;
    int diff_x, diff_y;

    if (SDL_PollEvent(&event) == 0) {
        // No event, carry on
        return 0;
    }
    if (event.type == SDL_QUIT) {
        // Handle quitting
        return -1;
    }
    if (handle_keys(event, assets, floor, player)) {
        return 1;
    }
    // Handle mouse
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
            tile_x = (event.motion.x - BOARD_OFFSET_X) / TILEW;
            tile_y = (event.motion.y - BOARD_OFFSET_Y) / TILEH;
            if (tile_x >= 0 && tile_x < TILE_DISPLAY_WIDTH &&
                tile_y >= 0 && tile_y < TILE_DISPLAY_HEIGHT) {
                player_display_x = player->x + floor_get_x_offset(player->x, floor->width);
                player_display_y = player->y + floor_get_y_offset(player->y, floor->height);
                diff_x = tile_x - player_display_x;
                diff_y = tile_y - player_display_y;
                if (diff_x == 0 && diff_y < 0) {
                    attempt_player_move(assets, floor, player, PLAYER_UP);
                }
                if (diff_x > 0 && diff_y == 0) {
                    attempt_player_move(assets, floor, player, PLAYER_RIGHT);
                }
                if (diff_x == 0 && diff_y > 0) {
                    attempt_player_move(assets, floor, player, PLAYER_DOWN);
                }
                if (diff_x < 0 && diff_y == 0) {
                    attempt_player_move(assets, floor, player, PLAYER_LEFT);
                }
            }
        }
    }
    return 0;
}

int check_level_complete(Floor_t * floor, Player_t * player)
{
    int current_tile;

    current_tile = floor_get_tile(floor, player->x, player->y);
    if (floor->coins - player->coins <= 0) {
        if (tile_has_flag(current_tile, TILEFLAG_PORTAL)) {
            return (floor->level_number + 1);
        }
    }
    return 0;
}

int check_death(Floor_t * floor, Player_t * player)
{
    int current_tile;
    int *mob = NULL;

    current_tile = floor_get_tile(floor, player->x, player->y);
    if (tile_has_flag(current_tile, TILEFLAG_WATER)) {
        return (floor->level_number);
    }
    if (tile_has_flag(current_tile, TILEFLAG_FIRE) && !player_has_item(player, ITEM_BOOT)) {
        return (floor->level_number);
    }
    // Running into mob
    if (qtree_peek(&(floor->mobs), (Point_t) {player->x, player->y}, (void *) &mob)) {
        if (player_has_item(player, ITEM_SWORD)) {
            qtree_pop(&(floor->mobs), (Point_t) {player->x, player->y}, (void *) &mob);
            return 0;
        }
        return (floor->level_number);
    }
    return 0;
}

void mob_turn(Floor_t * floor, Player_t * player)
{
    Stream_t stream;
    Box_t query;
    TaggedData_t point_mob;
    Mob_t * mob;
    Point_t point;
    int x_offset, y_offset;

    x_offset = floor_get_x_offset(player->x, floor->width);
    y_offset = floor_get_y_offset(player->y, floor->height);
    if (floor->limit_mob_vision) {
        query = (Box_t) {
            (Point_t) {x_offset * -1, y_offset * -1},
            TILE_DISPLAY_WIDTH, TILE_DISPLAY_HEIGHT
        };
    } else {
        query = (Box_t) {
            (Point_t) {0,0}, floor->width, floor->height
        };
    }
    stream = floor_get_mob_stream(floor, query);
    while(!list_stream_is_empty(&stream)) {
        point_mob = **((TaggedData_t **) list_stream_get(&stream));
        mob = (Mob_t *) point_mob.data;
        point = (Point_t) point_mob.point;
        mob_ai(floor, player, point, mob);
    }
    list_destroy(stream.list);

}

void mob_ai(Floor_t * floor, Player_t * player, Point_t point, Mob_t * mob)
{
    Point_t dest;

    if (mob->ai == MOB_FOLLOW) {
        if (abs(point.x - player->x) > abs(point.y - player->y)) {
            if (point.x > player->x) {
                dest = (Point_t) {point.x - 1, point.y};
                if (floor_mob_can_move(floor, dest)) {
                    floor_mob_move(floor, mob, point, dest);
                    return;
                }
            }
            if (point.x < player->x) {
                dest = (Point_t) {point.x + 1, point.y};
                if (floor_mob_can_move(floor, dest)) {
                    floor_mob_move(floor, mob, point, dest);
                    return;
                }
            }
        } else {
            if (point.y < player->y) {
                dest = (Point_t) {point.x, point.y + 1};
                if (floor_mob_can_move(floor, dest)) {
                    floor_mob_move(floor, mob, point, dest);
                    return;
                }
            }
            if (point.y > player->y) {
                dest = (Point_t) {point.x, point.y - 1};
                if (floor_mob_can_move(floor, dest)) {
                    floor_mob_move(floor, mob, point, dest);
                    return;
                }
            }
        }
    }
    if (mob->ai == MOB_SENTRY) {
        switch(mob->direction) {
            case PLAYER_UP:
                dest = (Point_t) {point.x, point.y - 1};
                break;
            case PLAYER_DOWN:
                dest = (Point_t) {point.x, point.y + 1};
                break;
            case PLAYER_LEFT:
                dest = (Point_t) {point.x - 1, point.y};
                break;
            case PLAYER_RIGHT:
                dest = (Point_t) {point.x + 1, point.y};
                break;
        }
        if (floor_mob_can_move(floor, dest)) {
            floor_mob_move(floor, mob, point, dest);
            return;
        } else {
            // Swap directions if blocked
            switch(mob->direction) {
                case PLAYER_UP:
                    dest = (Point_t) {point.x, point.y + 1};
                    break;
                case PLAYER_DOWN:
                    dest = (Point_t) {point.x, point.y - 1};
                    break;
                case PLAYER_LEFT:
                    dest = (Point_t) {point.x + 1, point.y};
                    break;
                case PLAYER_RIGHT:
                    dest = (Point_t) {point.x - 1, point.y};
                    break;
            }
            if (floor_mob_can_move(floor, dest)) {
                floor_mob_move(floor, mob, point, dest);
            }
        }

    }
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
        // Events that automatically move player
        current_tile = floor_get_tile(floor, player->x, player->y);
        if (tile_has_flag(current_tile, TILEFLAG_SLIPPERY) && !player_has_item(player, ITEM_SKATE)) {
            player_move(floor, player, player->direction, true);
        }
        mob_turn(floor, player);
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
