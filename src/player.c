#include "player.h"

bool player_load_level(Floor_t * floor, Player_t * player, unsigned int level_number)
{
    char filename[500];
    FILE* infile;

    snprintf(filename, sizeof(filename), "data/maps/floor%u.dat", level_number);
    if ((infile = fopen(filename, "r")) == NULL) {
        printf("Can't open file\n");
        return false;
    } else {
        floor_init(floor, infile);
        floor->level_number = level_number;
        fclose(infile);
    }
    player->x = floor->player_start_x;
    player->y = floor->player_start_y;
    player->direction = PLAYER_DOWN;
    player->item_flags = 0;
    player->time = 0;
    player->coins = 0;
    timer_reset(&floor->timer);
    return true;
}

bool player_move(Floor_t * floor, Player_t * player, int player_direction)
{
    int target_tile, next_tile;

    switch (player_direction) {
    case PLAYER_UP:
        player->direction = PLAYER_UP;
        if (player->y > 0) {
            target_tile = floor_get_tile(floor, player->x, player->y - 1);
            if (!tile_has_flag(target_tile, TILEFLAG_SOLID)) {
                player->y = player->y - 1;
                return true;
            } else if (player->y > 1 && tile_has_flag(target_tile, TILEFLAG_PUSHABLE)) {
                next_tile = floor_get_tile(floor, player->x, player->y - 2);
                if (!tile_has_flag(next_tile, TILEFLAG_SOLID)) {
                    if (!tile_has_flag(next_tile, TILEFLAG_FILLABLE)) {
                        floor_set_tile(floor, player->x, player->y - 2, 2);
                        floor_set_tile(floor, player->x, player->y - 1, 0);
                        player->y = player->y - 1;
                        return true;
                    } else if (tile_has_flag(next_tile, TILEFLAG_SLIPPERY)) {
                        floor_set_tile(floor, player->x, player->y - 2, 2);
                        floor_set_tile(floor, player->x, player->y - 1, 0);
                        player->y = player->y - 1;
                        return true;
                    } else {
                        floor_set_tile(floor, player->x, player->y - 2, 0);
                        floor_set_tile(floor, player->x, player->y - 1, 0);
                        player->y = player->y - 1;
                        return true;
                    }
               }
            }
        }
        break;
    case PLAYER_DOWN:
        player->direction = PLAYER_DOWN;
        if (player->y < floor->height - 1) {
            target_tile = floor_get_tile(floor, player->x, player->y + 1);
            if (!tile_has_flag(target_tile, TILEFLAG_SOLID)) {
                player->y = player->y + 1;
                return true;
            } else if (player->y < floor->height - 2 && tile_has_flag(target_tile, TILEFLAG_PUSHABLE)) {
                next_tile = floor_get_tile(floor, player->x, player->y + 2);
                if (!tile_has_flag(next_tile, TILEFLAG_SOLID)) {
                    if (!tile_has_flag(next_tile, TILEFLAG_FILLABLE)) {
                        floor_set_tile(floor, player->x, player->y + 2, 2);
                        floor_set_tile(floor, player->x, player->y + 1, 0);
                        player->y = player->y + 1;
                        return true;
                    } else {
                        floor_set_tile(floor, player->x, player->y + 2, 0);
                        floor_set_tile(floor, player->x, player->y + 1, 0);
                        player->y = player->y + 1;
                        return true;
                    }
                }
            }
        }
        break;
    case PLAYER_LEFT:
        player->direction = PLAYER_LEFT;
        if (player->x > 0) {
            target_tile = floor_get_tile(floor, player->x - 1, player->y);
            if (!tile_has_flag(target_tile, TILEFLAG_SOLID)) {
                player->x = player->x - 1;
                return true;
            } else if(player->x > 1 && tile_has_flag(target_tile, TILEFLAG_PUSHABLE))  {
                next_tile = floor_get_tile(floor, player->x - 2, player->y);
                if (!tile_has_flag(next_tile, TILEFLAG_SOLID)) {
                    if (!tile_has_flag(next_tile, TILEFLAG_FILLABLE)) {
                        floor_set_tile(floor, player->x - 2, player->y, 2);
                        floor_set_tile(floor, player->x - 1, player->y, 0);
                        player->x = player->x - 1;
                        return true;
                    } else {
                        floor_set_tile(floor, player->x - 2, player->y, 0);
                        floor_set_tile(floor, player->x - 1, player->y, 0);
                        player->x = player->x - 1;
                        return true;
                    }
                }
            }
        }
        break;
    case PLAYER_RIGHT:
        player->direction = PLAYER_RIGHT;
        if (player->x < floor->width - 1) {
            target_tile = floor_get_tile(floor, player->x + 1, player->y);
            if (!tile_has_flag(target_tile, TILEFLAG_SOLID)) {
                player->x = player->x + 1;
                return true;
            } else if(player->x < floor->width - 2 && tile_has_flag(target_tile, TILEFLAG_PUSHABLE)) {
                next_tile = floor_get_tile(floor, player->x + 2, player->y);
                if (!tile_has_flag(next_tile, TILEFLAG_SOLID)) {
                    if (!tile_has_flag(next_tile, TILEFLAG_FILLABLE)) {
                        floor_set_tile(floor, player->x + 2, player->y, 2);
                        floor_set_tile(floor, player->x + 1, player->y, 0);
                        player->x = player->x + 1;
                        return true;
                    } else if (tile_has_flag(next_tile, TILEFLAG_SLIPPERY)) {
                        floor_set_tile(floor, player->x + 2, player->y, 2);
                        floor_set_tile(floor, player->x + 1, player->y, 0);
                        player->y = player->y - 1;
                        return true;
                    } else {
                        floor_set_tile(floor, player->x + 2, player->y, 0);
                        floor_set_tile(floor, player->x + 1, player->y, 0);
                        player->x = player->x + 1;
                        return true;
                    }
                }
            }
        }
        break;
    default:
        break;
    }
    return false;
}

bool player_check_get_item(Asset_t * assets, Floor_t * floor, Player_t * player)
{
    Point_t position;
    int * item;

    position = (Point_t) {player->x, player->y};
    if (qtree_pop(&(floor->items), position, (void *) &item)) {
        if (*item != ITEM_COIN) {
            player_add_item(player, *item);
            return true;
        } else {
            player->coins++;
            Mix_PlayChannel(-1, assets->coin_sound, 0);
            return true;
        }
    }
    return false;
}
